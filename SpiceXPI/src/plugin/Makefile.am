NULL            =
EXTENSION_ID    = SpiceXPI@redhat.com
FIREFOX_APPID   = {ec8030f7-c20a-464f-9b0e-13a3a9e97384}
extensiondir    = $(libdir)/mozilla
SDK_INCLUDE_DIR = `pkg-config --variable=idldir mozilla-plugin`

INCLUDES =                           \
	$(XUL_CFLAGS)                    \
	-I$(top_srcdir)/common           \
	-I..                             \
	-I$(top_srcdir)/red	             \
	-I$(top_srcdir)/red/$(red_target)\
	$(QCAIRO_CFLAGS)                 \
	$(LOG4CPP_CFLAGS)                \
	-DCAIRO_CANVAS_ACCESS_TEST       \
	-DCAIRO_CANVAS_CACHE             \
	-DCAIRO_CANVAS_NO_CHUNKS         \
	-DXP_UNIX                        \
	-DMOZILLA_INTERNAL_API           \
	$(NULL)

plugindir=$(extensiondir)/plugins

plugin_LTLIBRARIES = libnsISpicec.la

libnsISpicec_la_LDFLAGS = -avoid-version -module
libnsISpicec_la_CPPFLAGS =           \
	$(AM_CPPFLAGS)                   \
	-I $(XUL_INCLUDEDIR)             \
	-I $(XUL_INCLUDEDIR)/dom         \
	-I $(XUL_INCLUDEDIR)/necko       \
	-I $(XUL_INCLUDEDIR)/plugin      \
	-I $(XUL_INCLUDEDIR)/string      \
	$(NULL)

libnsISpicec_la_LIBADD =             \
	$(QCAIRO_LIBS)                   \
	$(LOG4CPP_LIBS)                  \
	$(XUL_LIBS)                      \
	$(NULL)

libnsISpicec_la_SOURCES =            \
	np_entry.cpp                     \
	npn_gate.cpp                     \
	npp_gate.cpp                     \
	nsScriptablePeer.h               \
	nsScriptablePeer.cpp             \
	nsScriptablePeerBase.h           \
	nsScriptablePeerBase.cpp         \
	pluginbase.h                     \
	pluginbase.cpp                   \
	plugin.h                         \
	plugin.cpp                       \
	nsISpicec.h                      \
	controller.cpp                   \
	debug.h                          \
	controller.h                     \
	controller_prot.h                \
	../../../common/rederrorcodes.h  \
	../../../common/common.h         \
	nsISpicec.idl                    \
	npplat.h                         \
	$(NULL)

if HAVE_XPIDL
nsISpicec.h: nsISpicec.idl
	$(XPIDL) -m header -I $(SDK_INCLUDE_DIR) $<

nsISpicec.xpt: nsISpicec.idl
	$(XPIDL) -m typelib -I $(SDK_INCLUDE_DIR) $<
else
nsISpicec.h: nsISpicec.idl
	$(PYTHON) `pkg-config --variable=sdkdir mozilla-plugin`/sdk/bin/header.py --cachedir . -I $(SDK_INCLUDE_DIR) $< -o $@

nsISpicec.xpt: nsISpicec.idl
	$(PYTHON) `pkg-config --variable=sdkdir mozilla-plugin`/sdk/bin/typelib.py --cachedir . -I $(SDK_INCLUDE_DIR) $< -o $@
endif

BUILT_SOURCES =					\
	nsISpicec.h				\
	nsISpicec.xpt				\
	$(NULL)

distclean-local:
	rm -f $(BUILT_SOURCES)

CLEANFILES = \
	xpidllex.py   \
	xpidllex.pyc  \
	xpidlyacc.py  \
	xpidlyacc.pyc \
	xpidl_debug
