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

ODINCL=-I$(INTDIR) -I.. 							\
		-I../../odcore/$(PLATFORM)/$(BUILDTYPE) 	\
		-I../../odshell/$(PLATFORM)/$(BUILDTYPE) 	\
		-I../../odincl/include 						\
		-I../../somidl/$(PLATFORM) 					\
		-I../../somkpub/include						\
		-I../../somtk/include						\
		$(SOMTKINCL)								\
		$(STDINCL)

TARGET=$(OUTDIR_BIN)/docshell$(EXESUFFIX)
TARGET_C=$(OUTDIR_BIN)/docshell-c$(EXESUFFIX)
TARGET_CPP=$(OUTDIR_BIN)/docshell-cpp$(EXESUFFIX)
OBJS_CPP=$(INTDIR)/DocShell.cpp.o
OBJS_C=$(INTDIR)/DocShell.c.o

all: all-$(HAVE_LIBXT)

all-false:

all-true: $(TARGET_C) $(TARGET_CPP) $(TARGET)

clean:
	$(CLEAN) $(TARGET_C) $(TARGET_CPP) $(TARGET) $(OBJS_CPP) $(OBJS_C)

$(INTDIR)/DocShell.cpp.o: ../src/DocShell.cpp ../src/DocShell.c
	$(CXX_EXE) -I../src $(STDOPTXX) $(ODINCL) -c ../src/DocShell.cpp -o $@

$(INTDIR)/DocShell.c.o: ../src/DocShell.c
	$(CC_EXE) $(STDOPT) $(ODINCL) -c ../src/DocShell.c -o $@

$(TARGET_CPP): $(OBJS_CPP)
	$(LINKAPPXX) $(LINKAPP_HEAD) $(OBJS_CPP) -o $@ \
		`$(SHLB_REF) odshell odshell` \
		`$(SHLB_REF) odbento odbento` \
		`$(SHLB_REF) odui odui` \
		`$(SHLB_REF) oddatax oddatax` \
        `$(SHLB_REF) odcore odcore` \
 		`$(SHLB_REF) odutils odutils` \
		`$(SHLB_MAP) docshell docshell-cpp` \
		$(SOMTKLIBS) \
		$(XTLIBS) $(X11LIBS) $(SOCKLIBS) $(STDLIBXX) $(LINKAPP_TAIL)  

$(TARGET_C): $(OBJS_C)
	$(LINKAPP) $(LINKAPP_HEAD) $(OBJS_C) -o $@ \
		`$(SHLB_REF) odshell odshell` \
		`$(SHLB_REF) odbento odbento` \
		`$(SHLB_REF) odui odui` \
		`$(SHLB_REF) oddatax oddatax` \
        `$(SHLB_REF) odcore odcore` \
 		`$(SHLB_REF) odutils odutils` \
		`$(SHLB_MAP) docshell docshell-c` \
		$(SOMTKLIBS) \
		$(XTLIBS) $(X11LIBS) $(SOCKLIBS) $(STDLIB) $(LINKAPP_TAIL)  

$(TARGET): 
	cd `dirname $@`; ln -s `basename $(TARGET_C)` `basename $@`

dist install:

