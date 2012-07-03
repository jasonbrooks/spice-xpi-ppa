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

#ifndef CONTROLLER_PROT_H
#define CONTROLLER_PROT_H

#define CONTROLLER_MAGIC      (*(uint32_t*)"CTRL")
#define CONTROLLER_VERSION    1

#ifdef __GNUC__
#  define ATTR_PACKED __attribute__ ((__packed__))
#else
#  define ATTR_PACKED __declspec(align(1))
#endif

typedef struct ATTR_PACKED ControllerInitHeader {
    uint32_t magic;
    uint32_t version;
    uint32_t size;
} ControllerInitHeader;

typedef struct ATTR_PACKED ControllerInit {
    ControllerInitHeader base;
    uint64_t credentials;
    uint32_t flags;
} ControllerInit;

enum {
    CONTROLLER_FLAG_EXCLUSIVE = 1 << 0,
};

typedef struct ATTR_PACKED ControllerMsg {
    uint32_t id;
    uint32_t size;
} ControllerMsg;

enum {
    //extrenal app -> spice client
    CONTROLLER_HOST = 1,
    CONTROLLER_PORT,
    CONTROLLER_SPORT,
    CONTROLLER_PASSWORD,

    CONTROLLER_SECURE_CHANNELS,
    CONTROLLER_DISABLE_CHANNELS,

    CONTROLLER_TLS_CIPHERS,
    CONTROLLER_CA_FILE,
    CONTROLLER_HOST_SUBJECT,

    CONTROLLER_FULL_SCREEN,
    CONTROLLER_SET_TITLE,

    CONTROLLER_CREATE_MENU,
    CONTROLLER_DELETE_MENU,

    CONTROLLER_HOTKEYS,
    CONTROLLER_SEND_CAD,

    CONTROLLER_CONNECT,
    CONTROLLER_SHOW,
    CONTROLLER_HIDE,

    //spice client -> extrenal app
    CONTROLLER_MENU_ITEM_CLICK = 1001,
};

#define CONTROLLER_TRUE (1 << 0)

enum {
    CONTROLLER_SET_FULL_SCREEN  = CONTROLLER_TRUE,
    CONTROLLER_AUTO_DISPLAY_RES = 1 << 1,
};

typedef struct ATTR_PACKED ControllerValue {
    ControllerMsg base;
    uint32_t value;
} ControllerValue;

typedef struct ATTR_PACKED ControllerData {
    ControllerMsg base;
    uint8_t data[0];
} ControllerData;

#define CONTROLLER_MENU_ITEM_DELIMITER L"\n"
#define CONTROLLER_MENU_PARAM_DELIMITER L"\r"

enum {
    CONTROLLER_MENU_FLAGS_SEPARATOR    = 1 << 0,
    CONTROLLER_MENU_FLAGS_DISABLED     = 1 << 1,
    CONTROLLER_MENU_FLAGS_POPUP        = 1 << 2,
    CONTROLLER_MENU_FLAGS_CHECKED      = 1 << 3,
    CONTROLLER_MENU_FLAGS_GRAYED       = 1 << 4,
};

#define SPICE_MENU_INTERNAL_ID_BASE   0x1300
#define SPICE_MENU_INTERNAL_ID_SHIFT  8

#undef ATTR_PACKED

#endif // CONTROLLER_PROT_H
