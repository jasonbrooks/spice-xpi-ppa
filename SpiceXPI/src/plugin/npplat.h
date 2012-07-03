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

#ifndef NPPLAT_H
#define NPPLAT_H

#include "config.h"

#include "npapi.h"
#ifdef HAVE_XUL191
#  include "npfunctions.h"
#else
#  include "npupp.h"
#endif

// For 1.9 compatibility
// ------------------------------------------------
#ifndef CallNPN_GetURLNotifyProc
#  define NewNPN_GetURLNotifyProc(FUNC) \
          ((NPN_GetURLNotifyUPP) (FUNC))
#  define CallNPN_GetURLNotifyProc(FUNC, ARG1, ARG2, ARG3, ARG4) \
          (*(FUNC))((ARG1), (ARG2), (ARG3), (ARG4))
#endif

#ifndef CallNPN_GetURLProc
#  define NewNPN_GetURLProc(FUNC) \
          ((NPN_GetURLUPP) (FUNC))
#  define CallNPN_GetURLProc(FUNC, ARG1, ARG2, ARG3) \
          (*(FUNC))((ARG1), (ARG2), (ARG3))
#endif

#ifndef CallNPN_PostURLNotifyProc
#  define NewNPN_PostURLNotifyProc(FUNC) \
          ((NPN_PostURLNotifyUPP) (FUNC))
#  define CallNPN_PostURLNotifyProc(FUNC, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7) \
          (*(FUNC))((ARG1), (ARG2), (ARG3), (ARG4), (ARG5), (ARG6), (ARG7))
#endif

#ifndef CallNPN_PostURLProc
#  define NewNPN_PostURLProc(FUNC) ((NPN_PostURLUPP) (FUNC))
#  define CallNPN_PostURLProc(FUNC, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6) \
          (*(FUNC))((ARG1), (ARG2), (ARG3), (ARG4), (ARG5), (ARG6))
#endif

#ifndef CallNPN_RequestReadProc
#  define NewNPN_RequestReadProc(FUNC) \
          ((NPN_RequestReadUPP) (FUNC))
#  define CallNPN_RequestReadProc(FUNC, stream, range) \
          (*(FUNC))((stream), (range))
#endif

#ifndef CallNPN_NewStreamProc
#  define NewNPN_NewStreamProc(FUNC) \
          ((NPN_NewStreamUPP) (FUNC))
#  define CallNPN_NewStreamProc(FUNC, npp, type, window, stream) \
          (*(FUNC))((npp), (type), (window), (stream))
#endif

#ifndef CallNPN_WriteProc
#  define NewNPN_WriteProc(FUNC) \
          ((NPN_WriteUPP) (FUNC))
#  define CallNPN_WriteProc(FUNC, npp, stream, len, buffer) \
          (*(FUNC))((npp), (stream), (len), (buffer))
#endif

#ifndef CallNPN_DestroyStreamProc
#  define NewNPN_DestroyStreamProc(FUNC) \
          ((NPN_DestroyStreamUPP) (FUNC))
#  define CallNPN_DestroyStreamProc(FUNC, npp, stream, reason) \
          (*(FUNC))((npp), (stream), (reason))
#endif

#ifndef CallNPN_StatusProc
#  define NewNPN_StatusProc(FUNC) \
          ((NPN_StatusUPP) (FUNC))
#  define CallNPN_StatusProc(FUNC, npp, msg) \
          (*(FUNC))((npp), (msg))
#endif

#ifndef CallNPN_UserAgentProc
#  define NewNPN_UserAgentProc(FUNC) \
          ((NPN_UserAgentUPP) (FUNC))
#  define CallNPN_UserAgentProc(FUNC, ARG1) \
          (*(FUNC))((ARG1))
#endif

#ifndef CallNPN_MemAllocProc
#  define NewNPN_MemAllocProc(FUNC) \
          ((NPN_MemAllocUPP) (FUNC))
#  define CallNPN_MemAllocProc(FUNC, ARG1) \
          (*(FUNC))((ARG1))
#endif

#ifndef CallNPN_MemFreeProc
#  define NewNPN_MemFreeProc(FUNC) \
          ((NPN_MemFreeUPP) (FUNC))
#  define CallNPN_MemFreeProc(FUNC, ARG1) \
          (*(FUNC))((ARG1))
#endif

#ifndef CallNPN_MemFlushProc
#  define NewNPN_MemFlushProc(FUNC) \
          ((NPN_MemFlushUPP) (FUNC))
#  define CallNPN_MemFlushProc(FUNC, ARG1) \
          (*(FUNC))((ARG1))
#endif

#ifndef CallNPN_ReloadPluginsProc
#  define NewNPN_ReloadPluginsProc(FUNC) \
          ((NPN_ReloadPluginsUPP) (FUNC))
#  define CallNPN_ReloadPluginsProc(FUNC, ARG1) \
          (*(FUNC))((ARG1))
#endif

#ifndef CallNPN_GetValueProc
#  define NewNPN_GetValueProc(FUNC) \
          ((NPN_GetValueUPP) (FUNC))
#  define CallNPN_GetValueProc(FUNC, ARG1, ARG2, ARG3) \
          (*(FUNC))((ARG1), (ARG2), (ARG3))
#endif

#ifndef CallNPN_SetValueProc
#  define NewNPN_SetValueProc(FUNC) \
          ((NPN_SetValueUPP) (FUNC))
#  define CallNPN_SetValueProc(FUNC, ARG1, ARG2, ARG3) \
          (*(FUNC))((ARG1), (ARG2), (ARG3))
#endif

#ifndef CallNPN_InvalidateRectProc
#  define NewNPN_InvalidateRectProc(FUNC) \
          ((NPN_InvalidateRectUPP) (FUNC))
#  define CallNPN_InvalidateRectProc(FUNC, ARG1, ARG2) \
          (*(FUNC))((ARG1), (ARG2))
#endif

#ifndef CallNPN_InvalidateRegionProc
#  define NewNPN_InvalidateRegionProc(FUNC) \
          ((NPN_InvalidateRegionUPP) (FUNC))
#  define CallNPN_InvalidateRegionProc(FUNC, ARG1, ARG2) \
          (*(FUNC))((ARG1), (ARG2))
#endif

#ifndef CallNPN_ForceRedrawProc
#  define NewNPN_ForceRedrawProc(FUNC) \
          ((NPN_ForceRedrawUPP) (FUNC))
#  define CallNPN_ForceRedrawProc(FUNC, ARG1) \
          (*(FUNC))((ARG1))
#endif

// ------------------------------------------------

/**************************************************/
/*                                                */
/*                   Windows                      */
/*                                                */
/**************************************************/
#ifdef XP_WIN
#  include "windows.h"
#endif //XP_WIN

/**************************************************/
/*                                                */
/*                    Unix                        */
/*                                                */
/**************************************************/
#ifdef XP_UNIX
#  include <stdio.h>
#endif //XP_UNIX

/**************************************************/
/*                                                */
/*                     Mac                        */
/*                                                */
/**************************************************/
#ifdef XP_MAC
#  include <Processes.h>
#  include <Gestalt.h>
#  include <CodeFragments.h>
#  include <Timer.h>
#  include <Resources.h>
#  include <ToolUtils.h>

#  include "jri.h"

// The Mixed Mode procInfos defined in npupp.h assume Think C-
// style calling conventions.  These conventions are used by
// Metrowerks with the exception of pointer return types, which
// in Metrowerks 68K are returned in A0, instead of the standard
// D0. Thus, since NPN_MemAlloc and NPN_UserAgent return pointers,
// Mixed Mode will return the values to a 68K plugin in D0, but 
// a 68K plugin compiled by Metrowerks will expect the result in
// A0.  The following pragma forces Metrowerks to use D0 instead.
//
#  ifdef __MWERKS__
#    ifndef powerc
#      pragma pointers_in_D0
#    endif
#  endif

#  ifdef __MWERKS__
#    ifndef powerc
#      pragma pointers_in_A0
#    endif
#  endif

// The following fix for static initializers (which fixes a preious
// incompatibility with some parts of PowerPlant, was submitted by 
// Jan Ulbrich.
#  ifdef __MWERKS__
#    ifdef __cplusplus
extern "C" {
#    endif
#    ifndef powerc
extern void	__InitCode__(void);
#    else
extern void __sinit(void);
#      define __InitCode__ __sinit
#    endif
extern void	__destroy_global_chain(void);
#    ifdef __cplusplus
}
#    endif // __cplusplus
#  endif // __MWERKS__

// Wrapper functions for all calls from Netscape to the plugin.
// These functions let the plugin developer just create the APIs
// as documented and defined in npapi.h, without needing to 
// install those functions in the function table or worry about
// setting up globals for 68K plugins.
NPError Private_Initialize(void);
void    Private_Shutdown(void);
NPError Private_New(NPMIMEType pluginType, NPP instance, uint16 mode, int16 argc, char *argn[], char *argv[], NPSavedData *saved);
NPError Private_Destroy(NPP instance, NPSavedData **save);
NPError Private_SetWindow(NPP instance, NPWindow *window);
NPError Private_NewStream(NPP instance, NPMIMEType type, NPStream *stream, NPBool seekable, uint16 *stype);
NPError Private_DestroyStream(NPP instance, NPStream *stream, NPError reason);
int32   Private_WriteReady(NPP instance, NPStream *stream);
int32   Private_Write(NPP instance, NPStream *stream, int32 offset, int32 len, void *buffer);
void    Private_StreamAsFile(NPP instance, NPStream *stream, const char *fname);
void    Private_Print(NPP instance, NPPrint *platformPrint);
int16   Private_HandleEvent(NPP instance, void *event);
void    Private_URLNotify(NPP instance, const char *url, NPReason reason, void *notifyData);
jref    Private_GetJavaClass(void);
NPError Private_GetValue(NPP instance, NPPVariable variable, void *result);
NPError Private_SetValue(NPP instance, NPNVariable variable, void *value);

#endif //XP_MAC

#ifndef HIBYTE
#  define HIBYTE(i) (i >> 8)
#endif

#ifndef LOBYTE
#  define LOBYTE(i) (i & 0xff)
#endif

#endif // NPPLAT_H
