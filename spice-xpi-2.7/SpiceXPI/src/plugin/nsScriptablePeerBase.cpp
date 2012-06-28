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

#include <nsError.h>
#include <prtypes.h>
#include <npapi.h>
#include <npruntime.h>
#include "nsScriptablePeerBase.h"
#include "common.h"

ScriptablePluginObjectBase::ScriptablePluginObjectBase(NPP npp):
    m_npp(npp)
{
}

ScriptablePluginObjectBase::~ScriptablePluginObjectBase()
{
}

void ScriptablePluginObjectBase::Invalidate()
{
}

bool ScriptablePluginObjectBase::HasMethod(NPIdentifier name)
{
    return false;
}

bool ScriptablePluginObjectBase::Invoke(NPIdentifier name, const NPVariant *args,
                                        uint32_t argCount, NPVariant *result)
{
    NS_UNUSED(name);
    NS_UNUSED(args);
    NS_UNUSED(argCount);
    NS_UNUSED(result);
    return false;
}

bool ScriptablePluginObjectBase::InvokeDefault(const NPVariant *args,
                                               uint32_t argCount, NPVariant *result)
{
    NS_UNUSED(args);
    NS_UNUSED(argCount);
    NS_UNUSED(result);
    return false;
}

bool ScriptablePluginObjectBase::HasProperty(NPIdentifier name)
{
    NS_UNUSED(name);
    return false;
}

bool ScriptablePluginObjectBase::GetProperty(NPIdentifier name, NPVariant *result)
{
    NS_UNUSED(name);
    NS_UNUSED(result);
    return false;
}

bool ScriptablePluginObjectBase::SetProperty(NPIdentifier name,
                                             const NPVariant *value)
{
    NS_UNUSED(name);
    NS_UNUSED(value);
    return false;
}

bool ScriptablePluginObjectBase::RemoveProperty(NPIdentifier name)
{
    NS_UNUSED(name);
    return false;
}

bool ScriptablePluginObjectBase::Enumerate(NPIdentifier **identifier,
                                           uint32_t *count)
{
    NS_UNUSED(identifier);
    NS_UNUSED(count);
    return false;
}

bool ScriptablePluginObjectBase::Construct(const NPVariant *args, uint32_t argCount,
                                           NPVariant *result)
{
    NS_UNUSED(args);
    NS_UNUSED(argCount);
    NS_UNUSED(result);
    return false;
}

// static
void ScriptablePluginObjectBase::_Deallocate(NPObject *npobj)
{
    // Call the virtual destructor.
    delete static_cast<ScriptablePluginObjectBase *>(npobj);
}

// static
void ScriptablePluginObjectBase::_Invalidate(NPObject *npobj)
{
    static_cast<ScriptablePluginObjectBase *>(npobj)->Invalidate();
}

// static
bool ScriptablePluginObjectBase::_HasMethod(NPObject *npobj, NPIdentifier name)
{
    return static_cast<ScriptablePluginObjectBase *>(npobj)->HasMethod(name);
}

// static
bool ScriptablePluginObjectBase::_Invoke(NPObject *npobj, NPIdentifier name,
                                         const NPVariant *args, uint32_t argCount,
                                         NPVariant *result)
{
    return static_cast<ScriptablePluginObjectBase *>(npobj)->Invoke(name, args, argCount, result);
}

// static
bool ScriptablePluginObjectBase::_InvokeDefault(NPObject *npobj,
                                                const NPVariant *args,
                                                uint32_t argCount,
                                                NPVariant *result)
{
    return static_cast<ScriptablePluginObjectBase *>(npobj)->InvokeDefault(args, argCount, result);
}

// static
bool ScriptablePluginObjectBase::_HasProperty(NPObject * npobj, NPIdentifier name)
{
    return static_cast<ScriptablePluginObjectBase *>(npobj)->HasProperty(name);
}

// static
bool ScriptablePluginObjectBase::_GetProperty(NPObject *npobj, NPIdentifier name,
                                              NPVariant *result)
{
    return static_cast<ScriptablePluginObjectBase *>(npobj)->GetProperty(name, result);
}

// static
bool ScriptablePluginObjectBase::_SetProperty(NPObject *npobj, NPIdentifier name,
                                              const NPVariant *value)
{
    return static_cast<ScriptablePluginObjectBase *>(npobj)->SetProperty(name, value);
}

// static
bool ScriptablePluginObjectBase::_RemoveProperty(NPObject *npobj, NPIdentifier name)
{
    return static_cast<ScriptablePluginObjectBase *>(npobj)->RemoveProperty(name);
}

// static
bool ScriptablePluginObjectBase::_Enumerate(NPObject *npobj,
                                            NPIdentifier **identifier,
                                            uint32_t *count)
{
    return static_cast<ScriptablePluginObjectBase *>(npobj)->Enumerate(identifier, count);
}

// static
bool ScriptablePluginObjectBase::_Construct(NPObject *npobj, const NPVariant *args,
                                            uint32_t argCount, NPVariant *result)
{
    return static_cast<ScriptablePluginObjectBase *>(npobj)->Construct(args, argCount, result);
}
