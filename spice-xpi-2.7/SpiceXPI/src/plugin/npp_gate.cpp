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


////////////////////////////////////////////////////////////
//
// Implementation of plugin entry points (NPP_*)
//
#include "pluginbase.h"

// here the plugin creates a plugin instance object which 
// will be associated with this newly created NPP instance and 
// will do all the necessary job
NPError NPP_New(NPMIMEType pluginType, NPP instance, uint16_t mode, int16_t argc, char *argn[], char *argv[], NPSavedData *saved)
{
    if (instance == NULL)
        return NPERR_INVALID_INSTANCE_ERROR;

    // create a new plugin instance object
    // initialization will be done when the associated window is ready
    nsPluginCreateData ds;
    ds.instance = instance;
    ds.type     = pluginType;
    ds.mode     = mode;
    ds.argc     = argc;
    ds.argn     = argn;
    ds.argv     = argv;
    ds.saved    = saved;

    nsPluginInstanceBase *plugin = NS_NewPluginInstance(&ds);
    if (plugin == NULL)
        return NPERR_OUT_OF_MEMORY_ERROR;

    // associate the plugin instance object with NPP instance
    instance->pdata = static_cast<void *>(plugin);
    return NPERR_NO_ERROR;
}

// here is the place to clean up and destroy the nsPluginInstance object
NPError NPP_Destroy(NPP instance, NPSavedData **save)
{
    if (instance == NULL)
        return NPERR_INVALID_INSTANCE_ERROR;

    nsPluginInstanceBase *plugin = static_cast<nsPluginInstanceBase *>(instance->pdata);
    if (plugin != NULL)
    {
        plugin->shut();
        NS_DestroyPluginInstance(plugin);
    }

    return NPERR_NO_ERROR;
}

// during this call we know when the plugin window is ready or
// is about to be destroyed so we can do some gui specific
// initialization and shutdown
NPError NPP_SetWindow(NPP instance, NPWindow *pNPWindow)
{
    if (instance == NULL)
        return NPERR_INVALID_INSTANCE_ERROR;

    if (pNPWindow == NULL)
        return NPERR_GENERIC_ERROR;

    nsPluginInstanceBase *plugin = static_cast<nsPluginInstanceBase *>(instance->pdata);

    if (plugin == NULL)
        return NPERR_GENERIC_ERROR;

    // window just created
    if (!plugin->isInitialized() && pNPWindow->window != NULL)
    {
        if(!plugin->init(pNPWindow))
        {
            NS_DestroyPluginInstance(plugin);
            return NPERR_MODULE_LOAD_FAILED_ERROR;
        }
    }

    // window goes away
    if (pNPWindow->window == NULL && plugin->isInitialized())
        return plugin->SetWindow(pNPWindow);

    // window resized?
    if (pNPWindow->window != NULL && plugin->isInitialized())
        return plugin->SetWindow(pNPWindow);

    // this should not happen, nothing to do
    if (pNPWindow->window == NULL && !plugin->isInitialized())
        return plugin->SetWindow(pNPWindow);

    return NPERR_NO_ERROR;
}

NPError NPP_NewStream(NPP instance, NPMIMEType type, NPStream *stream, NPBool seekable, uint16_t *stype)
{
    if (instance == NULL)
        return NPERR_INVALID_INSTANCE_ERROR;

    nsPluginInstanceBase *plugin = static_cast<nsPluginInstanceBase *>(instance->pdata);
    if (plugin == NULL)
        return NPERR_GENERIC_ERROR;

    return plugin->NewStream(type, stream, seekable, stype);
}

int32_t NPP_WriteReady(NPP instance, NPStream *stream)
{
    if (instance == NULL)
        return 0x0fffffff;

    nsPluginInstanceBase *plugin = static_cast<nsPluginInstanceBase *>(instance->pdata);
    if (plugin == NULL)
        return 0x0fffffff;

    return plugin->WriteReady(stream);
}

int32_t NPP_Write(NPP instance, NPStream *stream, int32_t offset, int32_t len, void *buffer)
{
    if(instance == NULL)
        return len;

    nsPluginInstanceBase *plugin = static_cast<nsPluginInstanceBase *>(instance->pdata);
    if (plugin == NULL)
        return len;

    return plugin->Write(stream, offset, len, buffer);
}

NPError NPP_DestroyStream(NPP instance, NPStream *stream, NPError reason)
{
    if (instance == NULL)
        return NPERR_INVALID_INSTANCE_ERROR;

    nsPluginInstanceBase *plugin = static_cast<nsPluginInstanceBase *>(instance->pdata);
    if (plugin == NULL)
        return NPERR_GENERIC_ERROR;

    return plugin->DestroyStream(stream, reason);
}

void NPP_StreamAsFile(NPP instance, NPStream *stream, const char *fname)
{
    if (instance == NULL)
        return;

    nsPluginInstanceBase *plugin = static_cast<nsPluginInstanceBase *>(instance->pdata);
    if (plugin == NULL)
        return;

    plugin->StreamAsFile(stream, fname);
}

void NPP_Print (NPP instance, NPPrint *printInfo)
{
    if (instance == NULL)
        return;

    nsPluginInstanceBase *plugin = static_cast<nsPluginInstanceBase *>(instance->pdata);
    if (plugin == NULL)
        return;

    plugin->Print(printInfo);
}

void NPP_URLNotify(NPP instance, const char *url, NPReason reason, void *notifyData)
{
    if (instance == NULL)
        return;

    nsPluginInstanceBase *plugin = static_cast<nsPluginInstanceBase *>(instance->pdata);
    if (plugin == NULL)
        return;

    plugin->URLNotify(url, reason, notifyData);
}

NPError NPP_GetValue(NPP instance, NPPVariable variable, void *value)
{
    if (instance == NULL)
        return NPERR_INVALID_INSTANCE_ERROR;

    nsPluginInstanceBase *plugin = static_cast<nsPluginInstanceBase *>(instance->pdata);
    if (plugin == NULL)
        return NPERR_GENERIC_ERROR;

    return plugin->GetValue(variable, value);
}

NPError NPP_SetValue(NPP instance, NPNVariable variable, void *value)
{
    if (instance == NULL)
        return NPERR_INVALID_INSTANCE_ERROR;

    nsPluginInstanceBase *plugin = static_cast<nsPluginInstanceBase *>(instance->pdata);
    if (plugin == NULL)
        return NPERR_GENERIC_ERROR;

    return plugin->SetValue(variable, value);
}

int16_t NPP_HandleEvent(NPP instance, void *event)
{
    if (instance == NULL)
        return 0;

    nsPluginInstanceBase *plugin = static_cast<nsPluginInstanceBase *>(instance->pdata);
    if (plugin == NULL)
        return 0;

    return plugin->HandleEvent(event);
}

#ifdef OJI
jref NPP_GetJavaClass()
{
    return NULL;
}
#endif

/**************************************************/
/*                                                */
/*                     Mac                        */
/*                                                */
/**************************************************/

// Mac needs these wrappers, see npplat.h for more info

#ifdef XP_MAC
NPError Private_New(NPMIMEType pluginType, NPP instance, uint16_t mode,
                    int16_t argc, char *argn[], char *argv[],
                    NPSavedData *saved)
{
    return NPP_New(pluginType, instance, mode, argc, argn, argv, saved);
}

NPError Private_Destroy(NPP instance, NPSavedData **save)
{
    return NPP_Destroy(instance, save);
}

NPError Private_SetWindow(NPP instance, NPWindow *window)
{
    return NPP_SetWindow(instance, window);
}

NPError Private_NewStream(NPP instance, NPMIMEType type, NPStream *stream,
                          NPBool seekable, uint16_t *stype)
{
    return NPP_NewStream(instance, type, stream, seekable, stype);
}

int32_t Private_WriteReady(NPP instance, NPStream *stream)
{
    return NPP_WriteReady(instance, stream);
}

int32_t Private_Write(NPP instance, NPStream *stream, int32_t offset,
                      int32_t len, void *buffer)
{
    return NPP_Write(instance, stream, offset, len, buffer);
}

void Private_StreamAsFile(NPP instance, NPStream *stream, const char *fname)
{
    NPP_StreamAsFile(instance, stream, fname);
}


NPError Private_DestroyStream(NPP instance, NPStream *stream, NPError reason)
{
    return NPP_DestroyStream(instance, stream, reason);
}

int16_t Private_HandleEvent(NPP instance, void *event)
{
  return NPP_HandleEvent(instance, event);
}

void Private_Print(NPP instance, NPPrint *platformPrint)
{
    NPP_Print(instance, platformPrint);
}

void Private_URLNotify(NPP instance, const char *url, NPReason reason,
                       void *notifyData)
{
    NPP_URLNotify(instance, url, reason, notifyData);
}

jref Private_GetJavaClass()
{
    return NULL;
}

NPError Private_GetValue(NPP instance, NPPVariable variable, void *result)
{
    return NPP_GetValue(instance, variable, result);
}

NPError Private_SetValue(NPP instance, NPNVariable variable, void *value)
{
    return NPP_SetValue(instance, variable, value);
}
#endif //XP_MAC
