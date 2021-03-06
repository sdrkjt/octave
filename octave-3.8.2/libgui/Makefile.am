# Makefile for Octave's libgui directory
#
# Copyright (C) 2012-2013 John W. Eaton
#
# This file is part of Octave.
#
# Octave is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; either version 3 of the License, or (at
# your option) any later version.
#
# Octave is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License
# along with Octave; see the file COPYING.  If not, see
# <http://www.gnu.org/licenses/>.

include $(top_srcdir)/build-aux/common.mk

MOC_CPPFLAGS =

## Fix for bug #42839 where -mieee CFLAG option is added to CPPFLAGS by gnulib.
## Eventually gnulib will be appropriately fixed and this hack removed.
MOC_OCTAVE_CPPFLAGS = $(filter-out -mieee, $(AM_CPPFLAGS) $(CPPFLAGS))

octlib_LTLIBRARIES = liboctgui.la

TRANSLATIONS = \
  languages/be_BY.ts \
  languages/de_DE.ts \
  languages/en_US.ts \
  languages/es_ES.ts \
  languages/fr_FR.ts \
  languages/nl_NL.ts \
  languages/pt_BR.ts \
  languages/pt_PT.ts \
  languages/ru_RU.ts \
  languages/uk_UA.ts \
  languages/zh_CN.ts

LOCALES = $(patsubst languages/%.ts, languages/%.qm, $(TRANSLATIONS))

EXTRA_DIST = default-qt-settings.in $(TRANSLATIONS)
CLEANFILES =

BUILT_SOURCES =

noinst_HEADERS =

noinst_LTLIBRARIES =

include src/module.mk
include qterminal-module.mk

## liboctgui merely collects a bunch of compiled convenience libraries.
## It has no source code itself.
liboctgui_la_SOURCES = 

include link-deps.mk

# Dummy C++ source to force C++ linking.
nodist_EXTRA_liboctgui_la_SOURCES = dummy.cc

liboctgui_la_LIBADD = \
  qterminal/libqterminal.la \
  src/libgui-src.la \
  $(top_builddir)/libinterp/liboctinterp.la \
  $(top_builddir)/liboctave/liboctave.la \
  $(LIBOCTGUI_LINK_DEPS)

# Increment these as needed and according to the rules in the libtool manual:
liboctgui_current = 0
liboctgui_revision = 0
liboctgui_age = 0

liboctgui_version_info = $(liboctgui_current):$(liboctgui_revision):$(liboctgui_age)

liboctgui_la_LDFLAGS = \
  -version-info $(liboctgui_version_info) \
  $(NO_UNDEFINED_LDFLAG) \
  -bindir $(bindir) \
  $(LIBOCTGUI_LINK_OPTS)

octetc_DATA = default-qt-settings

octlocale_DATA = $(LOCALES)

default-qt-settings: default-qt-settings.in
	$(do_subst_qt_settings)

define moc-command
( echo '#ifdef HAVE_CONFIG_H'; \
  echo '#include <config.h>'; \
  echo '#endif'; \
  $(MOC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(MOC_OCTAVE_CPPFLAGS) $(MOC_CPPFLAGS) $(liboctgui_la_CPPFLAGS) $< ) > $@-t
mv $@-t $@
endef

define rcc-command
( echo '#ifdef HAVE_CONFIG_H'; \
  echo '#include <config.h>'; \
  echo '#endif'; \
  $(RCC) $< ) > $@-t
mv $@-t $@
endef

moc-%.cc: %.h
	$(moc-command)

ui-%.h: %.ui
	$(UIC) -o $@ $<

qrc-%.cc: %.qrc
	$(rcc-command)

%.qm: %.ts
	$(MKDIR_P) languages
	$(LRELEASE) -qm $@ $<

DISTCLEANFILES = \
  default-qt-settings \
  $(LOCALES)

