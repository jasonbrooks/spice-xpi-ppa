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

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>

extern "C" {
#  include <stdint.h>
#  include <unistd.h>
#  include <fcntl.h>
#  include <sys/socket.h>
#  include <sys/un.h>
}

#include "debug.h"
#include "rederrorcodes.h"
#include "controller.h"

const char *channel_names[] = { "dummy", "main", "display", "inputs", "cursor", "playback", "record" };

void QErrorHandler(int err, const char *custom_string)
{
    LOG_DEBUG("Something went wrong: " << custom_string << ", " << err);
}

SpiceController::SpiceController():
    m_client_socket(-1)
{
}

SpiceController::SpiceController(const std::string &name):
    m_client_socket(-1),
    m_name(name)
{
}

SpiceController::~SpiceController()
{
    LOG_TRACE("");
    Disconnect();
}

void SpiceController::SetFilename(const std::string &name)
{
    m_name = name;
}

int SpiceController::Connect()
{
    // check, if we have a filename for socket to create
    if (m_name.empty())
        return -1;

    if (m_client_socket == -1)
    {
        if ((m_client_socket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
        {
            QErrorHandler(errno, "SpiceController::Connect socket create error");
            return -1;
        }
    }

    struct sockaddr_un remote;
    remote.sun_family = AF_UNIX;
    strcpy(remote.sun_path, m_name.c_str());

    int rc = connect(m_client_socket, (struct sockaddr *) &remote, strlen(remote.sun_path) + sizeof(remote.sun_family));
    if (rc == -1)
    {
        QErrorHandler(errno, "connect error");
        LOG_DEBUG("Connect Error");
    }
    else
    {
        LOG_DEBUG("Connected!");
    }

    return rc;
}

int SpiceController::Connect(const int nRetries)
{
    int rc = -1;
    int sleep_time = 0;

    // try to connect for specified count
    for (int i = 0; rc != 0 && i < nRetries; ++i)
    {
        rc = Connect();
        sleep(sleep_time);
        ++sleep_time;
    }

    return rc;
}

void SpiceController::Disconnect()
{
    // close the socket
    close(m_client_socket);
    m_client_socket = -1;

    // delete the temporary file, which is used for the socket
    unlink(m_name.c_str());
    m_name.clear();
}

uint32_t SpiceController::Write(const void *lpBuffer, uint32_t nBytesToWrite)
{
    ssize_t len = send(m_client_socket, lpBuffer, nBytesToWrite, 0);

    if (len != (ssize_t)nBytesToWrite)
    {
        LOG_WARN("send error, bytes to write = " << nBytesToWrite <<
                 ", bytes actually written = " << len << ", errno = " << errno);
        QErrorHandler(1, "send error");
    }

    return len;
}

int SpiceController::TranslateRC(int nRC)
{
    switch (nRC)
    {
    case SPICEC_ERROR_CODE_SUCCESS:
        return 0;

    case SPICEC_ERROR_CODE_GETHOSTBYNAME_FAILED:
        return RDP_ERROR_CODE_HOST_NOT_FOUND;

    case SPICEC_ERROR_CODE_CONNECT_FAILED:
        return RDP_ERROR_CODE_WINSOCK_CONNECT_FAILED;

    case SPICEC_ERROR_CODE_ERROR:
    case SPICEC_ERROR_CODE_SOCKET_FAILED:
        return RDP_ERROR_CODE_INTERNAL_ERROR;

    case SPICEC_ERROR_CODE_RECV_FAILED:
        return RDP_ERROR_RECV_WINSOCK_FAILED;

    case SPICEC_ERROR_CODE_SEND_FAILED:
        return RDP_ERROR_SEND_WINSOCK_FAILED;

    case SPICEC_ERROR_CODE_NOT_ENOUGH_MEMORY:
        return RDP_ERROR_CODE_OUT_OF_MEMORY;

    case SPICEC_ERROR_CODE_AGENT_TIMEOUT:
        return RDP_ERROR_CODE_TIMEOUT;

    case SPICEC_ERROR_CODE_AGENT_ERROR:
        return RDP_ERROR_CODE_INTERNAL_ERROR;

    default:
        return RDP_ERROR_CODE_INTERNAL_ERROR;
    }
}

