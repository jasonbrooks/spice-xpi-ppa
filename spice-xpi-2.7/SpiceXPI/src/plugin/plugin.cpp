/* ***** BEGIN LICENSE BLOCK *****
 *   Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 *   The contents of this file are subject to the Mozilla Public License Version
 *   1.1 (the "License"); you may not use this file except in compliance with
 *   the License. You may obtain a copy of the License at
 *   http://www.mozilla.org/MPL/
 *
 *   Software distributed under the License is distributed on an "AS IS" basis,
 *   WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 *   for the specific language governing rights and limitations under the
 *   License.
 *
 *   Copyright 2009-2011, Red Hat Inc.
 *   Based on mozilla.org's scriptable plugin example
 *
 *   The Original Code is mozilla.org code.
 *
 *   The Initial Developer of the Original Code is
 *   Netscape Communications Corporation.
 *   Portions created by the Initial Developer are Copyright (C) 1998
 *   the Initial Developer. All Rights Reserved.
 *
 *   Contributor(s):
 *   Uri Lublin
 *   Martin Stransky
 *   Peter Hatina
 *
 *   Alternatively, the contents of this file may be used under the terms of
 *   either the GNU General Public License Version 2 or later (the "GPL"), or
 *   the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 *   in which case the provisions of the GPL or the LGPL are applicable instead
 *   of those above. If you wish to allow use of your version of this file only
 *   under the terms of either the GPL or the LGPL, and not to allow others to
 *   use your version of this file under the terms of the MPL, indicate your
 *   decision by deleting the provisions above and replace them with the notice
 *   and other provisions required by the GPL or the LGPL. If you do not delete
 *   the provisions above, a recipient may use your version of this file under
 *   the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <sys/types.h>
#include <sys/wait.h>

#include <stdlib.h>
#include <dlfcn.h>
#include <errno.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include <signal.h>

#include "nsCOMPtr.h"

// for plugins
#ifndef XP_UNIX
#include <nsString.h>
#include <nsIDOMNavigator.h>
#include <nsIDOMPluginArray.h>
#include <nsIDOMPlugin.h>
#include <nsIDOMMimeType.h>
#include <nsIObserverService.h>
#include <plugin/nsIPluginHost.h>

static NS_DEFINE_CID(kPluginManagerCID, NS_PLUGINMANAGER_CID);
#endif

#include <nsIServiceManager.h>
#include <nsISupportsUtils.h> // some usefule macros are defined here


#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/BasicConfigurator.hh>

#include <fstream>
#include <set>
#include "config.h"
#include "controller.h"
#include "debug.h"
#include "plugin.h"
#include "nsScriptablePeer.h"

DECLARE_NPOBJECT_CLASS_WITH_BASE(ScriptablePluginObject,
                                 AllocateScriptablePluginObject);

namespace {
    const std::string ver(PACKAGE_VERSION);
    const std::string PLUGIN_NAME = "Spice Firefox Plugin " + ver;
    const std::string MIME_TYPES_DESCRIPTION = "application/x-spice:qsc:" + PLUGIN_NAME;
    const std::string PLUGIN_DESCRIPTION = PLUGIN_NAME + " Spice Client wrapper for firefox";
    const std::string LOGGER_CONFIG = "/usr/share/spice/logger.ini";

    // helper function for string copy
    char *stringCopy(const std::string &src)
    {
        char *dest = static_cast<char *>(NPN_MemAlloc(src.length() + 1));
        if (dest)
            strcpy(dest, src.c_str());

        return dest;
    }
}

#ifdef NPAPI_USE_CONSTCHARS
const char *NPP_GetMIMEDescription(void)
{
    return const_cast<char *>(MIME_TYPES_DESCRIPTION.c_str());
}
#else
char *NPP_GetMIMEDescription(void)
{
    return strdup(MIME_TYPES_DESCRIPTION.c_str());
}
#endif

//////////////////////////////////////
//
// general initialization and shutdown
//
NPError NS_PluginInitialize()
{
    return NPERR_NO_ERROR;
}

void NS_PluginShutdown()
{
}

// get values per plugin
NPError NS_PluginGetValue(NPPVariable aVariable, void *aValue)
{
    switch (aVariable)
    {
    case NPPVpluginNameString:
        *(static_cast<char **>(aValue)) = const_cast<char *>(PLUGIN_NAME.c_str());
        break;

    case NPPVpluginDescriptionString:
        *(static_cast<char **>(aValue)) = const_cast<char *>(PLUGIN_DESCRIPTION.c_str());
        break;

    default:
        return NPERR_INVALID_PARAM;
    }

    return NPERR_NO_ERROR;
}


/////////////////////////////////////////////////////////////
//
// construction and destruction of our plugin instance object
//
nsPluginInstanceBase *NS_NewPluginInstance(nsPluginCreateData *aCreateDataStruct)
{
    if(!aCreateDataStruct)
        return NULL;

    nsPluginInstance *plugin = new nsPluginInstance(aCreateDataStruct->instance);

    // now is the time to tell Mozilla that we are windowless
    NPN_SetValue(aCreateDataStruct->instance, NPPVpluginWindowBool, NULL);

    return plugin;
}

void NS_DestroyPluginInstance(nsPluginInstanceBase *aPlugin)
{
    if (aPlugin)
        delete static_cast<nsPluginInstance *>(aPlugin);
}

////////////////////////////////////////
//
// nsPluginInstance class implementation
//

std::map<pid_t, nsPluginInstance *> nsPluginInstance::s_children;

nsPluginInstance::nsPluginInstance(NPP aInstance):
    nsPluginInstanceBase(),
    m_instance(aInstance),
    m_initialized(PR_FALSE),
    m_scriptable_peer(NULL)
{
    // create .spicec directory in $HOME
    m_home_dir = getenv("HOME");
    m_home_dir += "/.spicec";
    mkdir(m_home_dir.c_str(), S_IRWXU);

    // create temporary directory in /tmp
    char tmp_dir[] = "/tmp/spicec-XXXXXX";
    m_tmp_dir = mkdtemp(tmp_dir);

    // configure log4cpp
    std::ifstream log_init(LOGGER_CONFIG.c_str());
    if (log_init.good())
    {
        log_init.close();
        log4cpp::PropertyConfigurator::configure(LOGGER_CONFIG.c_str());
    }
    else
    {
        log4cpp::BasicConfigurator::configure();
    }

    m_connected_status = -2;

    struct sigaction chld;
    chld.sa_sigaction = SigchldRoutine;
    chld.sa_flags = SA_NOCLDSTOP | SA_RESTART | SA_SIGINFO;
    sigaction(SIGCHLD, &chld, NULL);
}

nsPluginInstance::~nsPluginInstance()
{
    // m_scriptable_peer may be also held by the browser
    // so releasing it here does not guarantee that it is over
    // we should take precaution in case it will be called later
    // and zero its m_plugin member
    if (m_scriptable_peer)
        NPN_ReleaseObject(m_scriptable_peer);

    // delete the temporary directory used for a client socket
    rmdir(m_tmp_dir.c_str());
}

NPBool nsPluginInstance::init(NPWindow *aWindow)
{
    m_initialized = PR_TRUE;

    m_host_ip.clear();
    m_port.clear();
    m_password.clear();
    m_secure_port.clear();
    m_cipher_suite.clear();
    m_ssl_channels.clear();
    m_trust_store.clear();
    m_host_subject.clear();
    m_title.clear();
    m_dynamic_menu.clear();
    m_number_of_monitors.clear();
    m_guest_host_name.clear();

    m_fullscreen = PR_FALSE;
    m_admin_console = PR_FALSE;
    m_no_taskmgr_execution = PR_FALSE;
    m_send_ctrlaltdel = PR_FALSE;

    return m_initialized;
}

NPError nsPluginInstance::SetWindow(NPWindow *aWindow)
{
    // keep window parameters
    m_window = aWindow;
    return NPERR_NO_ERROR;
}

void nsPluginInstance::shut()
{
    m_initialized = PR_FALSE;
}

NPBool nsPluginInstance::isInitialized()
{
    return m_initialized;
}

/* attribute string hostIP; */
char *nsPluginInstance::GetHostIP() const
{
    return stringCopy(m_host_ip);
}

void nsPluginInstance::SetHostIP(const char *aHostIP)
{
    m_host_ip = aHostIP;
}

/* attribute string port; */
char *nsPluginInstance::GetPort() const
{
    return stringCopy(m_port);
}

void nsPluginInstance::SetPort(const char *aPort)
{
    m_port = aPort;
}

/* attribute string SecurePort; */
char *nsPluginInstance::GetSecurePort() const
{
    return stringCopy(m_secure_port);
}

void nsPluginInstance::SetSecurePort(const char *aSecurePort)
{
    m_secure_port = aSecurePort;
}

/* attribute string Password; */
char *nsPluginInstance::GetPassword() const
{
    return stringCopy(m_password);
}

void nsPluginInstance::SetPassword(const char *aPassword)
{
    m_password = aPassword;
}

/* attribute string CipherSuite; */
char *nsPluginInstance::GetCipherSuite() const
{
    return stringCopy(m_cipher_suite);
}

void nsPluginInstance::SetCipherSuite(const char *aCipherSuite)
{
    m_cipher_suite = aCipherSuite;
}

/* attribute string SSLChannels; */
char *nsPluginInstance::GetSSLChannels() const
{
    return stringCopy(m_ssl_channels);
}

void nsPluginInstance::SetSSLChannels(const char *aSSLChannels)
{
    m_ssl_channels = aSSLChannels;
}

//* attribute string TrustStore; */
char *nsPluginInstance::GetTrustStore() const
{
    return stringCopy(m_trust_store);
}

void nsPluginInstance::SetTrustStore(const char *aTrustStore)
{
    m_trust_store = aTrustStore;
}

/* attribute string HostSubject; */
char *nsPluginInstance::GetHostSubject() const
{
    return stringCopy(m_host_subject);
}

void nsPluginInstance::SetHostSubject(const char *aHostSubject)
{
    m_host_subject = aHostSubject;
}

/* attribute boolean fullScreen; */
PRBool nsPluginInstance::GetFullScreen() const
{
    return m_fullscreen;
}

void nsPluginInstance::SetFullScreen(PRBool aFullScreen)
{
    m_fullscreen = aFullScreen;
}

/* attribute string Title; */
char *nsPluginInstance::GetTitle() const
{
    return stringCopy(m_title);
}

void nsPluginInstance::SetTitle(const char *aTitle)
{
    m_title = aTitle;
}

/* attribute string dynamicMenu; */
char *nsPluginInstance::GetDynamicMenu() const
{
    return stringCopy(m_dynamic_menu);
}

void nsPluginInstance::SetDynamicMenu(const char *aDynamicMenu)
{
    m_dynamic_menu = aDynamicMenu;
}

/* attribute string NumberOfMonitors; */
char *nsPluginInstance::GetNumberOfMonitors() const
{
    return stringCopy(m_number_of_monitors);
}

void nsPluginInstance::SetNumberOfMonitors(const char *aNumberOfMonitors)
{
    m_number_of_monitors = aNumberOfMonitors;
}

/* attribute boolean AdminConsole; */
PRBool nsPluginInstance::GetAdminConsole() const
{
    return m_admin_console;
}

void nsPluginInstance::SetAdminConsole(PRBool aAdminConsole)
{
    m_admin_console = aAdminConsole;
}

/* attribute string GuestHostName; */
char *nsPluginInstance::GetGuestHostName() const
{
    return stringCopy(m_guest_host_name);
}

void nsPluginInstance::SetGuestHostName(const char *aGuestHostName)
{
    m_guest_host_name = aGuestHostName;
}

/* attribute string HotKey; */
char *nsPluginInstance::GetHotKeys() const
{
    return stringCopy(m_hot_keys);
}

void nsPluginInstance::SetHotKeys(const char *aHotKeys)
{
    m_hot_keys = aHotKeys;
}

/* attribute boolean NoTaskMgrExecution; */
PRBool nsPluginInstance::GetNoTaskMgrExecution() const
{
    return m_no_taskmgr_execution;
}

void nsPluginInstance::SetNoTaskMgrExecution(PRBool aNoTaskMgrExecution)
{
    m_no_taskmgr_execution = aNoTaskMgrExecution;
}

/* attribute boolean SendCtrlAltdelete; */
PRBool nsPluginInstance::GetSendCtrlAltdelete() const
{
    return m_send_ctrlaltdel;
}

void nsPluginInstance::SetSendCtrlAltdelete(PRBool aSendCtrlAltdelete)
{
    m_send_ctrlaltdel = aSendCtrlAltdelete;
}

/* attribute unsigned short UsbListenPort; */
unsigned short nsPluginInstance::GetUsbListenPort() const
{
    // this method exists due to RHEVM 2.2
    // and should be removed some time in future,
    // when fixed in RHEVM
    return 0;
}

void nsPluginInstance::SetUsbListenPort(unsigned short aUsbPort)
{
    // this method exists due to RHEVM 2.2
    // and should be removed some time in future,
    // when fixed in RHEVM
}

/* attribute boolean UsbAutoShare; */
PRBool nsPluginInstance::GetUsbAutoShare() const
{
    // this method exists due to RHEVM 2.2
    // and should be removed some time in future,
    // when fixed in RHEVM
    return false;
}

void nsPluginInstance::SetUsbAutoShare(PRBool aUsbAutoShare)
{
    // this method exists due to RHEVM 2.2
    // and should be removed some time in future,
    // when fixed in RHEVM
}

void nsPluginInstance::WriteToPipe(const void *data, uint32_t size)
{
    m_external_controller.Write(data, size);
}

void nsPluginInstance::SendInit()
{
    ControllerInit msg = { {CONTROLLER_MAGIC, CONTROLLER_VERSION, sizeof(msg)},
                           0, CONTROLLER_FLAG_EXCLUSIVE };
    WriteToPipe(&msg, sizeof(msg));
}

void nsPluginInstance::SendMsg(uint32_t id)
{
    ControllerMsg msg = {id, sizeof(msg)};
    WriteToPipe(&msg, sizeof(msg));
}

void nsPluginInstance::SendValue(uint32_t id, uint32_t value)
{
    if (!value)
        return;

    ControllerValue msg = { {id, sizeof(msg)}, value };
    WriteToPipe(&msg, sizeof(msg));
}

void nsPluginInstance::SendStr(uint32_t id, const char *str)
{
    if (!strlen(str))
        return;

    size_t size = sizeof(ControllerData) + strlen(str) + 1;
    ControllerData *msg = static_cast<ControllerData *>(malloc(size));
    msg->base.id = id;
    msg->base.size = size;
    strcpy(reinterpret_cast<char *>(msg->data), str);
    WriteToPipe(msg, size);
    free(msg);
}

void nsPluginInstance::SendWStr(uint32_t id, const wchar_t *str)
{
    size_t len = wcslen(str);
    if (!len)
        return;

    size_t size = sizeof(ControllerData) + (len + 1) * sizeof(wchar_t);
    ControllerData *msg = static_cast<ControllerData *>(malloc(size));
    msg->base.id = id;
    msg->base.size = size;
    wcscpy(reinterpret_cast<wchar_t *>(msg->data), str);
    WriteToPipe(msg, size);
    free(msg);
}

void nsPluginInstance::Connect()
{
    std::string socket_file(m_tmp_dir);
    socket_file += "/spice-xpi";
    if (setenv("SPICE_XPI_SOCKET", socket_file.c_str(), 1))
    {
        LOG_ERROR ("could not set SPICE_XPI_SOCKET env variable");
        return;
    }

    pid_t child = fork();
    LOG_DEBUG("child pid: " << child);
    if (child == 0)
    {
        execl("/usr/libexec/spice-xpi-client", "/usr/libexec/spice-xpi-client", NULL);
        LOG_ERROR("ERROR failed to run spice-xpi-client");

        // TODO: temporary fallback for backward compatibility
        execl("/usr/bin/spicec", "/usr/bin/spicec", "--controller", NULL);
        LOG_ERROR("ERROR failed to run spicec fallback");

        exit(1);
    }
    else
    {
        m_external_controller.SetFilename(socket_file);

        if (m_external_controller.Connect(10) != 0)
        {
            LOG_ERROR ("could not connect to spice client controller");
            return;
        }

        // create trust store filename
        FILE *fp;
        int fd = -1;
        char trust_store_template[] = "/tmp/truststore.pem-XXXXXX";
        fd = mkstemp(trust_store_template);
        m_trust_store_file = trust_store_template;

        if (fd != -1)
        {
            fp = fdopen(fd,"w+");
            if (fp != NULL)
            {
                fputs(m_trust_store.c_str(), fp);
                fflush(fp);
                fsync(fd);
                fclose(fp);
            }
            else
            {
                LOG_ERROR ("could not open truststore temp file");
                close(fd);
                unlink(m_trust_store_file.c_str());
                m_trust_store_file.clear();
                return;
            }
        }
        else
        {
            LOG_ERROR ("could not create truststore temp file. errno=" << errno);
            return;
        }

        SendInit();
        SendStr(CONTROLLER_HOST, m_host_ip.c_str());
        SendValue(CONTROLLER_PORT, atoi(m_port.c_str()));
        SendValue(CONTROLLER_SPORT, atoi(m_secure_port.c_str()));
        SendValue(CONTROLLER_FULL_SCREEN,
                   (m_fullscreen == PR_TRUE ? CONTROLLER_SET_FULL_SCREEN : 0) |
                   (m_admin_console == PR_FALSE ? CONTROLLER_AUTO_DISPLAY_RES : 0));
        SendStr(CONTROLLER_PASSWORD, m_password.c_str());
        SendStr(CONTROLLER_TLS_CIPHERS, m_cipher_suite.c_str());
        SendStr(CONTROLLER_SET_TITLE, m_title.c_str());

        /*
         * HACK -- remove leading s from m_SSLChannels, e.g. "main" not "smain"
         * RHEL5 uses 'smain' and 'sinpusts
         * RHEL6 uses 'main'  and 'inputs'
         */
        std::size_t found = 0;
        while ((found = m_ssl_channels.find("smain", found)) != std::string::npos)
            m_ssl_channels.replace(found, 5, "main");

        found = 0;
        while ((found = m_ssl_channels.find("sinputs", found)) != std::string::npos)
            m_ssl_channels.replace(found, 7, "inputs");
        /* HACK */

        SendStr(CONTROLLER_SECURE_CHANNELS, m_ssl_channels.c_str());
        SendStr(CONTROLLER_CA_FILE, m_trust_store_file.c_str());
        SendStr(CONTROLLER_HOST_SUBJECT, m_host_subject.c_str());
        SendStr(CONTROLLER_HOTKEYS, m_hot_keys.c_str());
        SendMsg(CONTROLLER_CONNECT);
        SendMsg(CONTROLLER_SHOW);

        // set connected status
        m_connected_status = -1;

        s_children[child] = this;
    }
}

void nsPluginInstance::Show()
{
    LOG_DEBUG("sending show message");
    SendMsg(CONTROLLER_SHOW);
}

void nsPluginInstance::Disconnect()
{
    for (std::map<pid_t, nsPluginInstance *>::iterator it = s_children.begin();
         it != s_children.end(); ++it)
    {
        if (it->second == this)
        {
            kill(it->first, SIGTERM);
            break;
        }
    }
}

void nsPluginInstance::ConnectedStatus(PRInt32 *retval)
{
    *retval = m_connected_status;
}

void nsPluginInstance::SetLanguageStrings(const char *aSection, const char *aLanguage)
{
    if (aSection != NULL && aLanguage != NULL)
    {
        if (strlen(aSection) > 0 && strlen(aLanguage) > 0)
            m_language[aSection] = aLanguage;

    }
}

void nsPluginInstance::SetUsbFilter(const char *aUsbFilter)
{
    // this method exists due to RHEVM 2.2
    // and should be removed some time in future,
    // when fixed in RHEVM
}

void nsPluginInstance::CallOnDisconnected(int code)
{
    NPObject *window = NULL;
    if (NPN_GetValue(m_instance, NPNVWindowNPObject, &window) != NPERR_NO_ERROR)
    {
        LOG_ERROR("could not get browser window, when trying to call OnDisconnected");
        return;
    }

    // get OnDisconnected callback
    NPIdentifier id_on_disconnected = NPN_GetStringIdentifier("OnDisconnected");
    if (!id_on_disconnected)
    {
        LOG_ERROR("could not find OnDisconnected identifier");
        return;
    }

    NPVariant var_on_disconnected;
    if (!NPN_GetProperty(m_instance, window, id_on_disconnected, &var_on_disconnected))
    {
        LOG_ERROR("could not get OnDisconnected function");
        return;
    }

    if (!NPVARIANT_IS_OBJECT(var_on_disconnected))
    {
        LOG_ERROR("OnDisconnected is not object");
        return;
    }

    NPObject *call_on_disconnected = NPVARIANT_TO_OBJECT(var_on_disconnected);

    // call OnDisconnected
    NPVariant arg;
    NPVariant void_result;
    INT32_TO_NPVARIANT(code, arg);
    NPVariant args[] = { arg };

    if (NPN_InvokeDefault(m_instance, call_on_disconnected, args, sizeof(args) / sizeof(args[0]), &void_result))
    {
        LOG_DEBUG("OnDisconnected successfuly called");
    }
    else
    {
        LOG_ERROR("could not call OnDisconnected");
    }

    // cleanup
    NPN_ReleaseObject(window);
    NPN_ReleaseVariantValue(&var_on_disconnected);
}

void nsPluginInstance::SigchldRoutine(int sig, siginfo_t *info, void *uap)
{
    LOG_DEBUG("child finished, pid: " << info->si_pid);

    int exit_code;
    waitpid(info->si_pid, &exit_code, 0);

    nsPluginInstance *fake_this = s_children[info->si_pid];
    fake_this->CallOnDisconnected(exit_code);
    fake_this->m_external_controller.Disconnect();
    s_children.erase(info->si_pid);
}

// ==============================
// ! Scriptability related code !
// ==============================
//
// here the plugin is asked by Mozilla to tell if it is scriptable
// we should return a valid interface id and a pointer to 
// nsScriptablePeer interface which we should have implemented
// and which should be defined in the corressponding *.xpt file
// in the bin/components folder
NPError nsPluginInstance::GetValue(NPPVariable aVariable, void *aValue)
{
    // Here we indicate that the plugin is scriptable. See this page for details:
    // https://developer.mozilla.org/en/Gecko_Plugin_API_Reference/Scripting_plugins
    if (aVariable == NPPVpluginScriptableNPObject)
    {
        // addref happens in getter, so we don't addref here
        *(static_cast<NPObject **>(aValue)) = GetScriptablePeer();
    }
    return NPERR_NO_ERROR;
}

// ==============================
// ! Scriptability related code !
// ==============================
//
// this method will return the scriptable object (and create it if necessary)
NPObject *nsPluginInstance::GetScriptablePeer()
{
    if (!m_scriptable_peer)
        m_scriptable_peer = NPN_CreateObject(m_instance, GET_NPOBJECT_CLASS(ScriptablePluginObject));

    if (m_scriptable_peer)
        NPN_RetainObject(m_scriptable_peer);

    return m_scriptable_peer;
}
