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

#ifndef PLUGIN_H
#define PLUGIN_H

#include <map>
#include <string>

#include <prtypes.h>
#include <npapi.h>
#include <npruntime.h>

extern "C" {
#include <pthread.h>
#include <signal.h>
}

#include "pluginbase.h"
#include "controller.h"
#include "common.h"

class nsPluginInstance: public nsPluginInstanceBase
{
public:
    nsPluginInstance(NPP aInstance);
    virtual ~nsPluginInstance();
    
    NPBool init(NPWindow *aWindow);
    void shut();
    NPBool isInitialized();
    
    NPError	GetValue(NPPVariable variable, void *value);
    NPError SetWindow(NPWindow *aWindow);
    
    // locals
    void Connect();
    void Disconnect();
    void Show();
    void ConnectedStatus(PRInt32 *retval);
    void SetLanguageStrings(const char *aSection, const char *aLanguage);
    void SetUsbFilter(const char *aUsbFilter);
    
    /* attribute ing Host; */
    char *GetHostIP() const;
    void SetHostIP(const char *aHostIP);
    
    /* attribute ing Port; */
    char *GetPort() const;
    void SetPort(const char *aPort);
    
    /* attribute ing Password; */
    char *GetPassword() const;
    void SetPassword(const char *aPassword);
    
    /* attribute ing SecurePort; */
    char *GetSecurePort() const;
    void SetSecurePort(const char *aSecurePort);
    
    /* attribute ing Port; */
    char *GetCipherSuite() const;
    void SetCipherSuite(const char *aCipherSuite);
    
    /* attribute ing Port; */
    char *GetSSLChannels() const;
    void SetSSLChannels(const char *aSSLChannels);
    
     /* attribute ing TrustStore; */
    char *GetTrustStore() const;
    void SetTrustStore(const char *aTrustStore);
    
     /* attribute ing HostSubject; */
    char *GetHostSubject() const;
    void SetHostSubject(const char *aHostSubject);
    
    /* attribute ing FullScreen; */
    PRBool GetFullScreen() const;
    void SetFullScreen(PRBool aFullScreen);
    
    /* attribute ing Port; */
    char *GetTitle() const;
    void SetTitle(const char * aTitle);
    
    /* attribute ing Port; */
    char *GetDynamicMenu() const;
    void SetDynamicMenu(const char *aDynamicMenu);
    
    /* attribute ing Port; */
    char *GetNumberOfMonitors() const;
    void SetNumberOfMonitors(const char *aNumberOfMonitors);
    
    /* attribute ing AdminConsole; */
    PRBool GetAdminConsole() const;
    void SetAdminConsole(PRBool aAdminConsole);
    
    /* attribute ing GuestHostName; */
    char *GetGuestHostName() const;
    void SetGuestHostName(const char *aGuestHostName);
    
    /* attribute ing HotKeys; */
    char *GetHotKeys() const;
    void SetHotKeys(const char *aHotKeys);
    
    /* attribute ing NoTaskMgrExecution; */
    PRBool GetNoTaskMgrExecution() const;
    void SetNoTaskMgrExecution(PRBool aNoTaskMgrExecution);
    
    /* attribute ing SendCtrlAltdelete; */
    PRBool GetSendCtrlAltdelete() const;
    void SetSendCtrlAltdelete(PRBool aSendCtrlAltdelete);
    
    /* attribute unsigned short UsbListenPort; */
    unsigned short GetUsbListenPort() const;
    void SetUsbListenPort(unsigned short aUsbPort);
    
    /* attribute boolean UsbAutoShare; */
    PRBool GetUsbAutoShare() const;
    void SetUsbAutoShare(PRBool aUsbAutoShare);
    
    NPObject *GetScriptablePeer();
    
private:
    static void SigchldRoutine(int sig, siginfo_t *info, void *uap);
    void WriteToPipe(const void *data, uint32_t size);
    void SendInit();
    void SendMsg(uint32_t id);
    void SendValue(uint32_t id, uint32_t value);
    void SendStr(uint32_t id, const char *str);
    void SendWStr(uint32_t id, const wchar_t *str);
    void CallOnDisconnected(int code);
  
private:
    static std::map<pid_t, nsPluginInstance *> s_children;
    PRInt32 m_connected_status;
    SpiceController m_external_controller;

    NPP m_instance;
    NPBool m_initialized;
    
    NPWindow *m_window;
    PRBool m_fIPC;
    std::string m_host_ip;
    std::string m_port;
    std::string m_password;
    std::string m_secure_port;
    std::string m_cipher_suite;
    std::string m_ssl_channels;
    std::string m_trust_store;
    std::string m_host_subject;
    PRBool m_fullscreen;
    PRBool m_admin_console;
    std::string m_title;
    std::string m_dynamic_menu;
    std::string m_number_of_monitors;
    std::string m_guest_host_name;
    std::string m_hot_keys;
    PRBool m_no_taskmgr_execution;
    PRBool m_send_ctrlaltdel;
    std::map<std::string, std::string> m_language;
    
    NPObject *m_scriptable_peer;
    std::string m_home_dir;
    std::string m_tmp_dir;
    std::string m_trust_store_file;
};

#endif // PLUGIN_H
