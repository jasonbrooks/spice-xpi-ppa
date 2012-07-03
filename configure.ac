AC_PREREQ([2.57])
AC_INIT(spice-xpi, [2.7], [], spice-xpi)

AC_CONFIG_MACRO_DIR([m4])
AM_CONFIG_HEADER([config.h])
AC_CONFIG_AUX_DIR(.)

AM_INIT_AUTOMAKE([dist-bzip2])
AM_MAINTAINER_MODE

AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_CANONICAL_HOST
AC_PROG_LIBTOOL
AM_PROG_CC_C_O

# Check for the CPU we are using
#
AC_MSG_CHECKING(for x86 or x86-64 platform)
case $host_cpu in
  i386|i486|i586|i686|i786|k6|k7)
       variant=32
        ;;
  x86_64)
       variant=64
       ;;
  *)
       AC_MSG_RESULT(no)
       echo Only x86 and x86-64 are supported
       exit 1
esac
AC_MSG_RESULT($variant bit)
AM_CONDITIONAL([X86_64], [test "$variant" = 64])

dnl =========================================================================
dnl Check deps

PKG_CHECK_MODULES(LOG4CPP, log4cpp)
AC_SUBST(LOG4CPP_CFLAGS)
AC_SUBST(LOG4CPP_LIBS)

# The explicit nspr dep is needed because libxul-embedding
# in RHEL5 is missing the Requires
PKG_CHECK_MODULES(XUL, libxul-embedding >= 1.9 nspr >= 4.7.1)
AC_SUBST(XUL_CFLAGS)
AC_SUBST(XUL_LIBS)

# test for const return value of NPP_GetMIMEDescription, based on gecko-mediaplayer
AC_MSG_CHECKING([for const return value of NPP_GetMIMEDescription])
oCFLAGS=$CFLAGS
CFLAGS="$XUL_CFLAGS"
AC_LANG_PUSH([C])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([
#ifndef XP_UNIX
#define XP_UNIX 1
#endif
#include <npapi.h>
#if NP_VERSION_MAJOR == 0 && NP_VERSION_MINOR < 27
#error
#else
const char*NPP_GetMIMEDescription(void) {
  return "test";
}
#endif
],[
const char *f=NPP_GetMIMEDescription();
])],[
    AC_MSG_RESULT([yes])
    AC_DEFINE([NPAPI_USE_CONSTCHARS],[1],[Define if NPAPI declares NPP_GetMIMEDescription as a const char*])
],[
    AC_MSG_RESULT([no])
])
AC_LANG_POP([C])
CFLAGS=$oCFLAGS

# Find xpidl
XPIDL=`pkg-config --variable=libdir libxul`/xpidl
AM_CONDITIONAL([HAVE_XPIDL], [test -x "$XPIDL"])
AC_SUBST(XPIDL)

if ! test -x "$XPIDL"; then
  AM_PATH_PYTHON
  AC_CHECK_FILE([`pkg-config --variable=sdkdir libxul`/sdk/bin/header.py])
  AC_CHECK_FILE([`pkg-config --variable=sdkdir libxul`/sdk/bin/typelib.py])
fi

XUL_INCLUDEDIR=`pkg-config --variable=includedir libxul`
XUL_IDLDIR=`pkg-config --variable=idldir libxul`
AC_SUBST(XUL_INCLUDEDIR)
AC_SUBST(XUL_IDLDIR)

dnl libxul 1.9.1 changed header file names
PKG_CHECK_MODULES(XUL191,
        libxul >= 1.9.1,
        have_xul191=yes,
        have_xul191=no)

if test "x$have_xul191" = "xyes" ; then
  AC_DEFINE([HAVE_XUL191], [1], [Define if we have libxul >= 1.9.1])
fi

m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

AC_OUTPUT([
Makefile
data/Makefile
SpiceXPI/Makefile
SpiceXPI/src/Makefile
SpiceXPI/src/plugin/Makefile
])

dnl ==========================================================================
AC_MSG_NOTICE([

        Spice-XPI $VERSION
        ==============

        prefix:                   ${prefix}
        compiler:                 ${CC}
        xpidl:			  ${XPIDL}
        XUL includes:		  ${XUL_INCLUDEDIR}
        XUL IDL files:	          ${XUL_IDLDIR}

        Red target:               ${red_target}

        Now type 'make' to build $PACKAGE
])
