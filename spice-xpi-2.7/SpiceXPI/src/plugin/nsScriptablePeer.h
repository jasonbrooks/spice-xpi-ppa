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
//
// nsScriptablePeer - xpconnect scriptable peer
//

#ifndef NS_SCRIPTABLE_PEER_H
#define NS_SCRIPTABLE_PEER_H

#include "nsScriptablePeerBase.h"

// Our scriptable object
// Unset methods are handled by ScriptablePluginObjectBase
class ScriptablePluginObject: public ScriptablePluginObjectBase
{
public:
    ScriptablePluginObject(NPP npp);
    virtual ~ScriptablePluginObject();

    virtual bool HasMethod(NPIdentifier name);
    virtual bool HasProperty(NPIdentifier name);
    virtual bool GetProperty(NPIdentifier name, NPVariant *result);
    virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
    virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                        uint32_t argCount, NPVariant *result);
    virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                               NPVariant *result);

private:
    void Init();

private:
    nsPluginInstance *m_plugin;

    static bool m_id_set;
    static NPIdentifier m_id_host_ip;
    static NPIdentifier m_id_port;
    static NPIdentifier m_id_secure_port;
    static NPIdentifier m_id_password;
    static NPIdentifier m_id_cipher_suite;
    static NPIdentifier m_id_ssl_channels;
    static NPIdentifier m_id_trust_store;
    static NPIdentifier m_id_host_subject;
    static NPIdentifier m_id_fullscreen;
    static NPIdentifier m_id_admin_console;
    static NPIdentifier m_id_title;
    static NPIdentifier m_id_dynamic_menu;
    static NPIdentifier m_id_num_of_monitors;
    static NPIdentifier m_id_guest_hostname;
    static NPIdentifier m_id_hotkey;
    static NPIdentifier m_id_no_taskmgr_execution;
    static NPIdentifier m_id_send_ctrlaltdel;
    static NPIdentifier m_id_usb_listen_port;
    static NPIdentifier m_id_usb_auto_share;
    static NPIdentifier m_id_connect;
    static NPIdentifier m_id_show;
    static NPIdentifier m_id_disconnect;
    static NPIdentifier m_id_set_language_strings;
    static NPIdentifier m_id_set_usb_filter;
    static NPIdentifier m_id_connect_status;
    static NPIdentifier m_id_plugin_instance;
};

#define DECLARE_NPOBJECT_CLASS_WITH_BASE(_class, ctor)                        \
static NPClass s##_class##_NPClass = {                                        \
    NP_CLASS_STRUCT_VERSION_CTOR,                                             \
    ctor,                                                                     \
    ScriptablePluginObjectBase::_Deallocate,                                  \
    ScriptablePluginObjectBase::_Invalidate,                                  \
    ScriptablePluginObjectBase::_HasMethod,                                   \
    ScriptablePluginObjectBase::_Invoke,                                      \
    ScriptablePluginObjectBase::_InvokeDefault,                               \
    ScriptablePluginObjectBase::_HasProperty,                                 \
    ScriptablePluginObjectBase::_GetProperty,                                 \
    ScriptablePluginObjectBase::_SetProperty,                                 \
    ScriptablePluginObjectBase::_RemoveProperty,                              \
    ScriptablePluginObjectBase::_Enumerate,                                   \
    ScriptablePluginObjectBase::_Construct                                    \
}

#define GET_NPOBJECT_CLASS(_class) &s##_class##_NPClass

NPObject *AllocateScriptablePluginObject(NPP npp, NPClass *aClass);

//void NPIdentifiersInit();

#endif // NS_SCRIPTABLE_PEER_H
