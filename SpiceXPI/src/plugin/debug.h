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

#ifndef DEBUG_H
#define DEBUG_H

#include <sstream>

#include <log4cpp/Category.hh>
#include <log4cpp/convenience.h>

#define ON_PANIC() ::abort()

#ifdef RED_DEBUG

#  ifdef WIN32
#    define ASSERTBREAK DebugBreak()
#  else
#    define ASSERTBREAK ::abort()
#  endif

#  define ASSERT(x) if (!(x)) {                               \
    printf("%s: ASSERT %s failed\n", __FUNCTION__, #x);     \
    ASSERTBREAK;                                            \
}

#else
#  define ASSERT(cond)
#endif

#ifdef __GNUC__
static inline std::string pretty_func_to_func_name(const std::string& f_name)
{
    std::string name(f_name);
    std::string::size_type end_pos = f_name.find('(');
    if (end_pos == std::string::npos) {
        return f_name;
    }
    std::string::size_type start = f_name.rfind(' ', end_pos);
    if (start == std::string::npos) {
        return f_name;
    }
    end_pos -= ++start;
    return name.substr(start, end_pos);
}
#  define FUNC_NAME pretty_func_to_func_name(__PRETTY_FUNCTION__).c_str()
#else
#  define FUNC_NAME __FUNCTION__
#endif

LOG4CPP_LOGGER("spice")

#define LOG(func, message) {                 \
    std::ostringstream os;                   \
    os << FUNC_NAME;                         \
    os << ": ";                              \
    os << message;                           \
    func(logger, os.str().c_str());          \
}

// Used to log just about everything that might be useful (or not).
#define LOG_TRACE(message) LOG(LOG4CPP_DEBUG, message)

// Used to log debug information which doesn't inflate the log with useless
// and repeated messages.
#define LOG_DEBUG(message) LOG(LOG4CPP_DEBUG, message)

// Used to log the program flow.
#define LOG_INFO(message) LOG(LOG4CPP_INFO, message)

// Used to log errors which are handled by the program.
#define LOG_WARN(message) LOG(LOG4CPP_WARN, message)

// Used to log errors that can't be handled by the program.
#define LOG_ERROR(message) LOG(LOG4CPP_ERROR, message)

// Used to log errors that can't happen (even if they do!).
#define LOG_FATAL(message) {     \
    LOG(LOG4CPP_EMERG, message); \
    ON_PANIC();                  \
}

#if 0
#  define DBGLEVEL 1000

#  define DBG(level, msg) {      \
    if (level <= DBGLEVEL) {     \
        std::ostringstream os;   \
        os << __FUNCTION__;      \
        os << ": ";              \
        os << msg;               \
        os << "\n";              \
        std::cout << os.str();   \
        std::cout.flush();       \
    }                            \
}
#endif

#define PANIC(msg)      LOG_FATAL(msg)
#define WARN(msg)       LOG_WARN(msg)
#define ERR(msg)        LOG_ERROR(msg)
#define NOTIFY(msg)     LOG_INFO(msg)
#define INFO(msg)       LOG_INFO(msg)
#define DBG(level, msg) LOG_DEBUG(msg)

#endif // DEBUG_H
