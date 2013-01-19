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

ODINCL=	-I$(INTDIR) \
		-I../include \
		-I../../odincl/include \
		-I../../somidl/$(PLATFORM) \
		-I../../somkpub/include						\
		-I../../somtk/include						\
		$(SOMTKINCL)								\
		$(STDINCL)

TARGET=$(OUTDIR_SHLIB)/$(DLLPREFIX)odutils$(DLLSUFFIX)

OBJS=	$(INTDIR)/rhbutils.o \
		$(INTDIR)/ODMath.o \
		$(INTDIR)/ODDebug.o \
		$(INTDIR)/ODMemory.o \
		$(INTDIR)/StdTypIO.o \
		$(INTDIR)/StorUtil.o \
		$(INTDIR)/WinUtils.o \
		$(INTDIR)/InfoUtil.o \
		$(INTDIR)/DocUtils.o \
		$(INTDIR)/ISOStr.o \
		$(INTDIR)/NmSpcUtl.o

all: all-$(HAVE_LIBXT)

all-false:

all-true: $(TARGET)

clean:
	$(CLEAN) $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(LINKDLL) $(LINKDLL_HEAD) \
		-o $@ \
		$(OBJS) \
		`$(SHLB_EXP) odutils odutils` \
		`$(SHLB_REF) odpubutl odpubutl` \
		`$(SHLB_REF) odui odui` \
		`$(SHLB_REF) odimagng odimagng` \
		`$(SHLB_REF) oddatax oddatax` \
		`$(SHLB_REF) odstorag odstorag` \
		`$(SHLB_REF) odcore odcore` \
		`$(SHLB_MAP) odutils odutils` \
		$(SOMTKLIBS) \
		$(X11LIBS) \
		$(SOCKLIBS) \
		$(STDLIB) \
		$(LINKDLL_TAIL) 

$(INTDIR)/rhbutils.o: ../src/rhbutils.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/rhbutils.c -o $@

$(INTDIR)/ODMath.o: ../src/ODMath.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/ODMath.c -o $@

$(INTDIR)/ODDebug.o: ../src/ODDebug.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/ODDebug.c -o $@

$(INTDIR)/ODMemory.o: ../src/ODMemory.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/ODMemory.c -o $@

$(INTDIR)/StorUtil.o: ../src/StorUtil.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/StorUtil.c -o $@

$(INTDIR)/StdTypIO.o: ../src/StdTypIO.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/StdTypIO.c -o $@

$(INTDIR)/InfoUtil.o: ../src/InfoUtil.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/InfoUtil.c -o $@

$(INTDIR)/WinUtils.o: ../src/WinUtils.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/WinUtils.c -o $@

$(INTDIR)/DocUtils.o: ../src/DocUtils.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/DocUtils.c -o $@

$(INTDIR)/ISOStr.o: ../src/ISOStr.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/ISOStr.c -o $@

$(INTDIR)/NmSpcUtl.o: ../src/NmSpcUtl.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/NmSpcUtl.c -o $@

dist install:

