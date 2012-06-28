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

// ==============================
// ! Scriptability related code !
// ==============================

/////////////////////////////////////////////////////
//
// This file implements the nsScriptablePeer object
// The native methods of this class are supposed to
// be callable from JavaScript
//

#include <nsError.h>
#include <string.h>
#include <sstream>
#include "plugin.h"
#include "common.h"
#include "nsScriptablePeer.h"

bool ScriptablePluginObject::m_id_set = false;
NPIdentifier ScriptablePluginObject::m_id_host_ip;
NPIdentifier ScriptablePluginObject::m_id_port;
NPIdentifier ScriptablePluginObject::m_id_secure_port;
NPIdentifier ScriptablePluginObject::m_id_password;
NPIdentifier ScriptablePluginObject::m_id_cipher_suite;
NPIdentifier ScriptablePluginObject::m_id_ssl_channels;
NPIdentifier ScriptablePluginObject::m_id_trust_store;
NPIdentifier ScriptablePluginObject::m_id_host_subject;
NPIdentifier ScriptablePluginObject::m_id_fullscreen;
NPIdentifier ScriptablePluginObject::m_id_admin_console;
NPIdentifier ScriptablePluginObject::m_id_title;
NPIdentifier ScriptablePluginObject::m_id_dynamic_menu;
NPIdentifier ScriptablePluginObject::m_id_num_of_monitors;
NPIdentifier ScriptablePluginObject::m_id_guest_hostname;
NPIdentifier ScriptablePluginObject::m_id_hotkey;
NPIdentifier ScriptablePluginObject::m_id_no_taskmgr_execution;
NPIdentifier ScriptablePluginObject::m_id_send_ctrlaltdel;
NPIdentifier ScriptablePluginObject::m_id_usb_listen_port;
NPIdentifier ScriptablePluginObject::m_id_usb_auto_share;
NPIdentifier ScriptablePluginObject::m_id_connect;
NPIdentifier ScriptablePluginObject::m_id_show;
NPIdentifier ScriptablePluginObject::m_id_disconnect;
NPIdentifier ScriptablePluginObject::m_id_set_language_strings;
NPIdentifier ScriptablePluginObject::m_id_set_usb_filter;
NPIdentifier ScriptablePluginObject::m_id_connect_status;
NPIdentifier ScriptablePluginObject::m_id_plugin_instance;

NPObject *AllocateScriptablePluginObject(NPP npp, NPClass *aClass)
{
    NS_UNUSED(aClass);
    return new ScriptablePluginObject(npp);
}

ScriptablePluginObject::ScriptablePluginObject(NPP npp):
    ScriptablePluginObjectBase(npp)
{
    m_plugin = static_cast<nsPluginInstance *>(npp->pdata);
    Init();
}

ScriptablePluginObject::~ScriptablePluginObject()
{
}

void ScriptablePluginObject::Init()
{
    if(m_id_set)
        return;

    m_id_host_ip = NPN_GetStringIdentifier("hostIP");
    m_id_port = NPN_GetStringIdentifier("port");
    m_id_secure_port = NPN_GetStringIdentifier("SecurePort");
    m_id_password = NPN_GetStringIdentifier("Password");
    m_id_cipher_suite = NPN_GetStringIdentifier("CipherSuite");
    m_id_ssl_channels = NPN_GetStringIdentifier("SSLChannels");
    m_id_trust_store = NPN_GetStringIdentifier("TrustStore");
    m_id_host_subject = NPN_GetStringIdentifier("HostSubject");
    m_id_fullscreen = NPN_GetStringIdentifier("fullScreen");
    m_id_admin_console = NPN_GetStringIdentifier("AdminConsole");
    m_id_title = NPN_GetStringIdentifier("Title");
    m_id_dynamic_menu = NPN_GetStringIdentifier("dynamicMenu");
    m_id_num_of_monitors = NPN_GetStringIdentifier("NumberOfMonitors");
    m_id_guest_hostname = NPN_GetStringIdentifier("GuestHostName");
    m_id_hotkey = NPN_GetStringIdentifier("HotKey");
    m_id_no_taskmgr_execution = NPN_GetStringIdentifier("NoTaskMgrExecution");
    m_id_send_ctrlaltdel = NPN_GetStringIdentifier("SendCtrlAltdelete");
    m_id_usb_listen_port = NPN_GetStringIdentifier("UsbListenPort");
    m_id_usb_auto_share = NPN_GetStringIdentifier("UsbAutoShare");
    m_id_connect = NPN_GetStringIdentifier("connect");
    m_id_show = NPN_GetStringIdentifier("show");
    m_id_disconnect = NPN_GetStringIdentifier("disconnect");
    m_id_set_language_strings = NPN_GetStringIdentifier("SetLanguageStrings");
    m_id_set_usb_filter = NPN_GetStringIdentifier("SetUsbFilter");
    m_id_connect_status = NPN_GetStringIdentifier("ConnectedStatus");
    m_id_plugin_instance = NPN_GetStringIdentifier("PluginInstance");
    m_id_set = true;
}

bool ScriptablePluginObject::HasMethod(NPIdentifier name)
{
    return(name == m_id_connect ||
           name == m_id_show ||
           name == m_id_disconnect ||
           name == m_id_set_language_strings ||
           name == m_id_set_usb_filter ||
           name == m_id_connect_status);
}

bool ScriptablePluginObject::HasProperty(NPIdentifier name)
{
    return(name == m_id_host_ip ||
           name == m_id_port ||
           name == m_id_secure_port ||
           name == m_id_password ||
           name == m_id_cipher_suite ||
           name == m_id_ssl_channels ||
           name == m_id_trust_store ||
           name == m_id_host_subject ||
           name == m_id_fullscreen ||
           name == m_id_admin_console ||
           name == m_id_title ||
           name == m_id_dynamic_menu ||
           name == m_id_num_of_monitors ||
           name == m_id_guest_hostname ||
           name == m_id_hotkey ||
           name == m_id_no_taskmgr_execution ||
           name == m_id_send_ctrlaltdel ||
           name == m_id_usb_listen_port ||
           name == m_id_usb_auto_share);
}

bool ScriptablePluginObject::GetProperty(NPIdentifier name, NPVariant *result)
{
    VOID_TO_NPVARIANT(*result);

    if (!m_plugin)
        return false;

    if (name == m_id_host_ip)
        STRINGZ_TO_NPVARIANT(m_plugin->GetHostIP(), *result);
    else if (name == m_id_port)
        STRINGZ_TO_NPVARIANT(m_plugin->GetPort(), *result);
    else if (name == m_id_secure_port)
        STRINGZ_TO_NPVARIANT(m_plugin->GetSecurePort(), *result);
    else if (name == m_id_password)
        STRINGZ_TO_NPVARIANT(m_plugin->GetPassword(), *result);
    else if (name == m_id_cipher_suite)
        STRINGZ_TO_NPVARIANT(m_plugin->GetCipherSuite(), *result);
    else if (name == m_id_ssl_channels)
        STRINGZ_TO_NPVARIANT(m_plugin->GetSSLChannels(), *result);
    else if (name == m_id_trust_store)
        STRINGZ_TO_NPVARIANT(m_plugin->GetTrustStore(), *result);
    else if (name == m_id_host_subject)
        STRINGZ_TO_NPVARIANT(m_plugin->GetHostSubject(), *result);
    else if (name == m_id_fullscreen)
        BOOLEAN_TO_NPVARIANT(m_plugin->GetFullScreen(), *result);
    else if (name == m_id_admin_console)
        BOOLEAN_TO_NPVARIANT(m_plugin->GetAdminConsole(), *result);
    else if (name == m_id_title)
        STRINGZ_TO_NPVARIANT(m_plugin->GetTitle(), *result);
    else if (name == m_id_dynamic_menu)
        STRINGZ_TO_NPVARIANT(m_plugin->GetDynamicMenu(), *result);
    else if (name == m_id_num_of_monitors)
        STRINGZ_TO_NPVARIANT(m_plugin->GetNumberOfMonitors(), *result);
    else if (name == m_id_guest_hostname)
        STRINGZ_TO_NPVARIANT(m_plugin->GetGuestHostName(), *result);
    else if (name == m_id_hotkey)
        STRINGZ_TO_NPVARIANT(m_plugin->GetHotKeys(), *result);
    else if (name == m_id_no_taskmgr_execution)
        BOOLEAN_TO_NPVARIANT(m_plugin->GetNoTaskMgrExecution(), *result);
    else if (name == m_id_send_ctrlaltdel)
        BOOLEAN_TO_NPVARIANT(m_plugin->GetSendCtrlAltdelete(), *result);
    else if (name == m_id_usb_listen_port)
        INT32_TO_NPVARIANT(m_plugin->GetUsbListenPort(), *result);
    else if (name == m_id_usb_auto_share)
        BOOLEAN_TO_NPVARIANT(m_plugin->GetUsbAutoShare(), *result);
    else
        return false;

    return true;
}

bool ScriptablePluginObject::SetProperty(NPIdentifier name, const NPVariant *value)
{
    if (!m_plugin)
        return false;

    std::string str;
    PRBool boolean = false;
    unsigned short val = -1;

    if (NPVARIANT_IS_STRING(*value))
    {
        str = NPVARIANT_TO_STRING(*value).UTF8Characters;
    }
    else if (NPVARIANT_IS_BOOLEAN(*value))
    {
        boolean = NPVARIANT_TO_BOOLEAN(*value);
    }
    else if (NPVARIANT_IS_INT32(*value))
    {
        val = NPVARIANT_TO_INT32(*value);

        std::stringstream ss;
        ss << val;
        ss >> str;
    }
    else
    {
        return false;
    }

    if (name == m_id_host_ip)
        m_plugin->SetHostIP(str.c_str());
    else if (name == m_id_port)
        m_plugin->SetPort(str.c_str());
    else if (name == m_id_secure_port)
        m_plugin->SetSecurePort(str.c_str());
    else if (name == m_id_password)
        m_plugin->SetPassword(str.c_str());
    else if (name == m_id_cipher_suite)
        m_plugin->SetCipherSuite(str.c_str());
    else if (name == m_id_ssl_channels)
        m_plugin->SetSSLChannels(str.c_str());
    else if (name == m_id_trust_store)
        m_plugin->SetTrustStore(str.c_str());
    else if (name == m_id_host_subject)
        m_plugin->SetHostSubject(str.c_str());
    else if (name == m_id_fullscreen)
        m_plugin->SetFullScreen(boolean);
    else if (name == m_id_admin_console)
        m_plugin->SetAdminConsole(boolean);
    else if (name == m_id_title)
        m_plugin->SetTitle(str.c_str());
    else if (name == m_id_dynamic_menu)
        m_plugin->SetDynamicMenu(str.c_str());
    else if (name == m_id_num_of_monitors)
        m_plugin->SetNumberOfMonitors(str.c_str());
    else if (name == m_id_guest_hostname)
        m_plugin->SetGuestHostName(str.c_str());
    else if (name == m_id_hotkey)
        m_plugin->SetHotKeys(str.c_str());
    else if (name == m_id_no_taskmgr_execution)
        m_plugin->SetNoTaskMgrExecution(boolean);
    else if (name == m_id_send_ctrlaltdel)
        m_plugin->SetSendCtrlAltdelete(boolean);
    else if (name == m_id_usb_listen_port)
        m_plugin->SetUsbListenPort(val);
    else if (name == m_id_usb_auto_share)
        m_plugin->SetUsbAutoShare(boolean);
    else
        return false;

    return true;
}

bool ScriptablePluginObject::Invoke(NPIdentifier name, const NPVariant *args,
                                    uint32_t argCount, NPVariant *result)
{
    if (!m_plugin)
        return false;

    if (name == m_id_connect)
    {
        m_plugin->Connect();
        return true;
    }
    else if (name == m_id_show)
    {
        m_plugin->Show();
        return true;
    }
    else if (name == m_id_disconnect)
    {
        m_plugin->Disconnect();
        return true;
    }
    else if (name == m_id_set_language_strings)
    {
        if(argCount < 2)
            return false;

        const char *aSection = NPVARIANT_TO_STRING(args[0]).UTF8Characters;
        const char *aLanguage = NPVARIANT_TO_STRING(args[1]).UTF8Characters;
        m_plugin->SetLanguageStrings(aSection, aLanguage);
        return true;
    }
    else if (name == m_id_set_usb_filter)
    {
        if(argCount < 1)
            return false;

        const char *aUsbFilter = NPVARIANT_TO_STRING(args[0]).UTF8Characters;
        m_plugin->SetUsbFilter(aUsbFilter);
        return true;
    }
    else if (name == m_id_connect_status)
    {
        PRInt32 ret;
        m_plugin->ConnectedStatus(&ret);
        INT32_TO_NPVARIANT(ret, *result);
        return true;
    }

    return false;
}

bool ScriptablePluginObject::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                           NPVariant *result)
{
    NS_UNUSED(args);
    NS_UNUSED(argCount);
    NS_UNUSED(result);
    return true;
}
