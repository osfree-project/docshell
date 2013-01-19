#
#  Copyright 2008, Roger Brown
#
#  This file is part of Roger Brown's Toolkit.
#
#  This program is free software: you can redistribute it and/or modify it
#  under the terms of the GNU Lesser General Public License as published by the
#  Free Software Foundation, either version 3 of the License, or (at your
#  option) any later version.
# 
#  This program is distributed in the hope that it will be useful, but WITHOUT
#  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
#  more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>
#
#  $Id$

include $(MAKEDEFS)

INTDIR=$(BUILDTYPE)

BUILD=../..
FRAMEWORKS=$(OUTDIR_FRAMEWORKS)
SHARED=-dynamiclib -Wl,-single_module
BUNDLE=-bundle
FRAMEWORK_NAME=ODCM.framework
ODCM_SHLIB=Versions/A/ODCM
TARGET_DARWIN_SONAME=/Library/Frameworks/$(FRAMEWORK_NAME)/$(ODCM_SHLIB)
TARGET_DARWIN_SHLIB=$(FRAMEWORKS)/$(FRAMEWORK_NAME)/$(ODCM_SHLIB)
TARGET_DARWIN_DEF=$(INTDIR)/odcm.exp.def
TARGET_DARWIN_LINK=$(FRAMEWORKS)/$(FRAMEWORK_NAME)/ODCM

all: ODCM

clean:
	rm -rf $(FRAMEWORKS)/$(FRAMEWORK_NAME)

ODCM: $(TARGET_DARWIN_SHLIB) $(TARGET_DARWIN_LINK)

$(TARGET_DARWIN_SHLIB): $(OBJS) $(TARGET_DARWIN_DEF)
	mkdir -p `dirname $@`
	$(CC) \
		`for d in \`../../toolbox/lipoarch.sh $(INTDIR)/BufferIO.o\`; do echo -arch $$d; done` \
		`if test "$(PLATFORM_ISYSROOT)" != ""; then echo -isysroot "$(PLATFORM_ISYSROOT)" ; fi` \
		-dynamiclib -o $@ -Wl,-install_name,$(TARGET_DARWIN_SONAME) $(OBJS) -Wl,-exported_symbols_list,$(TARGET_DARWIN_DEF) $(STDLIB)

$(TARGET_DARWIN_LINK):
	cd `dirname $@`; ln -s $(ODCM_SHLIB) `basename $@`
