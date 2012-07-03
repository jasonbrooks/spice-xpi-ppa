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

#ifndef SPICE_CONTROLLER_H
#define SPICE_CONTROLLER_H

/*
    Basic assumption:
    ------------------
    Cross platform compatible.
    Easy to transform into remote process communication
    Secured

    chosen:
        Unix - Unix Domain Sockets (easy to change into regular sockets for remote communication)
        Windows - Named pipe (which allows remote access and is duplex
            (rather than anonymous pipe which is local only and one way)
*/

#include <string>
extern "C" {
#  include <stdint.h>
#  include <limits.h>
}

#include "controller_prot.h"

class SpiceController
{
public:
    SpiceController();
    SpiceController(const std::string &name);
    ~SpiceController();

    void SetFilename(const std::string &name);
    int Connect();
    int Connect(int nRetries);
    void Disconnect();
    uint32_t Write(const void *lpBuffer, uint32_t nBytesToWrite);

    static int TranslateRC(int nRC);

private:
    int m_client_socket;
    std::string m_name;
};

#endif // SPICE_CONTROLLER_H
