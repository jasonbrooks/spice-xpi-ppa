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

#ifndef RED_ERROR_CODES_H
#define RED_ERROR_CODES_H

#define SPICEC_ERROR_CODE_SUCCESS                 (0)
#define SPICEC_ERROR_CODE_ERROR                   (-1)
#define SPICEC_ERROR_CODE_GETHOSTBYNAME_FAILED    (-2)
#define SPICEC_ERROR_CODE_CONNECT_FAILED          (-3)
#define SPICEC_ERROR_CODE_SOCKET_FAILED           (-4)
#define SPICEC_ERROR_CODE_SEND_FAILED             (-5)
#define SPICEC_ERROR_CODE_RECV_FAILED             (-6)
#define SPICEC_ERROR_CODE_SSL_ERROR               (-7)
#define SPICEC_ERROR_CODE_NOT_ENOUGH_MEMORY       (-8)
#define SPICEC_ERROR_CODE_AGENT_TIMEOUT           (-9)
#define SPICEC_ERROR_CODE_AGENT_ERROR             (-10)
#define SPICEC_ERROR_CODE_VERSION_MISMATCH        (-11)
#define SPICEC_ERROR_CODE_PERMISSION_DENIED       (-12)

// http://msdn.microsoft.com/en-us/library/aa911976.aspx
#define RDP_ERROR_CODE_LOCAL_DISCONNECTION        (1)
#define RDP_ERROR_CODE_DNS_LOOKUP_FAILED          (260)
#define RDP_ERROR_CODE_OUT_OF_MEMORY              (262)
#define RDP_ERROR_CODE_WINSOCK_CONNECT_FAILED     (516)
#define RDP_ERROR_CODE_HOST_NOT_FOUND             (520)
#define RDP_ERROR_CODE_INTERNAL_ERROR             (1032)
#define RDP_ERROR_RECV_WINSOCK_FAILED             (1028)
#define RDP_ERROR_SEND_WINSOCK_FAILED             (772)
#define RDP_ERROR_CODE_TIMEOUT                    (1796)

#endif // RED_ERROR_CODES_H
