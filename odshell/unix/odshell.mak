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

ODINCL= -I$(INTDIR) \
		-I../include \
		-I../../odcore/$(PLATFORM)/$(BUILDTYPE) \
		-I../../odui/$(PLATFORM)/$(BUILDTYPE) \
		-I../../oddatax/$(PLATFORM)/$(BUILDTYPE) \
		-I../../odincl/include \
		-I../../somidl/$(PLATFORM) \
		-I../../somkpub/include						\
		-I../../somtk/include						\
		$(SOMTKINCL)								\
		$(STDINCL) 

OBJS=$(INTDIR)/rhbshell.o \
	$(INTDIR)/ODShell.o \
	$(INTDIR)/AppShell.o \
	$(INTDIR)/DocMgr.o \
	$(INTDIR)/AIXShell.o \
	$(INTDIR)/ShPInLst.o \
	$(INTDIR)/DraftPage.o

TARGET=$(OUTDIR_SHLIB)/$(DLLPREFIX)odshell$(DLLSUFFIX)

all: all-$(HAVE_LIBXT)

all-false:

all-true: $(TARGET)

clean:
	$(CLEAN) $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(LINKDLL) $(LINKDLL_HEAD) \
		-o $@ \
		$(OBJS) \
		`$(SHLB_EXP) odshell odshell` \
		`$(SHLB_ENT) SOMInitModule` \
		`$(SHLB_REF) odbindng odbindng` \
		`$(SHLB_REF) odui odui` \
		`$(SHLB_REF) odimagng odimagng` \
		`$(SHLB_REF) odlayout odlayout` \
		`$(SHLB_REF) odutils odutils` \
		`$(SHLB_REF) odpubutl odpubutl` \
		`$(SHLB_REF) oddatax oddatax` \
		`$(SHLB_REF) odstorag odstorag` \
		`$(SHLB_REF) odcore odcore` \
		`$(SHLB_REF) odsomuc odsomuc` \
		`$(SHLB_MAP) odshell odshell` \
		$(SOMTKLIBS) \
		$(XTLIBS) $(X11LIBS) $(STDLIB) \
		$(LINKDLL_TAIL)

$(INTDIR)/rhbshell.o: ../src/rhbshell.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/rhbshell.c -o $@

$(INTDIR)/ODShell.o: ../src/ODShell.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/ODShell.c -o $@

$(INTDIR)/AppShell.o: ../src/AppShell.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/AppShell.c -o $@

$(INTDIR)/AIXShell.o: ../src/AIXShell.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/AIXShell.c -o $@

$(INTDIR)/DocMgr.o: ../src/DocMgr.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/DocMgr.c -o $@

$(INTDIR)/ShPInLst.o: ../src/ShPInLst.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/ShPInLst.c -o $@

$(INTDIR)/DraftPage.o: ../src/DraftPage.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/DraftPage.c -o $@




dist install:

