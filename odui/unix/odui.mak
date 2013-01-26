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
		-I../../odcore/$(PLATFORM)/$(BUILDTYPE) \
		-I../../odcore/include \
		-I../../odlayout/$(PLATFORM)/$(BUILDTYPE) \
		-I../../odimagng/$(PLATFORM)/$(BUILDTYPE) \
		-I../../odincl/include \
		-I../../rhbmenus/include \
		-I../../rhbxtutl/include \
		-I../../somidl/$(PLATFORM) \
		-I../../somkpub/include						\
		-I../../somtk/include						\
		$(SOMTKINCL) 								\
		-DBUILD_ODUI \
		$(STDINCL) 

TARGET=$(OUTDIR_SHLIB)/$(DLLPREFIX)odui$(DLLSUFFIX)

OBJS=$(INTDIR)/odui.o \
	$(INTDIR)/MenuAdX.o \
	$(INTDIR)/StdDispX.o \
	$(INTDIR)/WinStat.o \
	$(INTDIR)/Window.o \
	$(INTDIR)/WinIter.o \
	$(INTDIR)/Undo.o \
	$(INTDIR)/MenuBar.o \
	$(INTDIR)/Popup.o \
	$(INTDIR)/Disptch.o \
	$(INTDIR)/DispMod.o \
	$(INTDIR)/StdDisp.o \
	$(INTDIR)/Arbitrat.o \
	$(INTDIR)/FocusMod.o \
	$(INTDIR)/FocusOwn.o \
	$(INTDIR)/FocusSet.o \
	$(INTDIR)/FocusItr.o \
	$(INTDIR)/ExcFocus.o

all: all-$(HAVE_LIBXT)

all-false:

all-true: $(TARGET)

clean:
	$(CLEAN) $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(LINKDLL) $(LINKDLL_HEAD) \
		-o $@ \
		$(OBJS) \
		`$(SHLB_EXP) odui odui` \
		`$(SHLB_ENT) SOMInitModule` \
		`$(SHLB_REF) odlayout odlayout` \
		`$(SHLB_REF) odimagng odimagng` \
		`$(SHLB_REF) odutils odutils` \
		`$(SHLB_REF) odstorag odstorag` \
		`$(SHLB_REF) odpubutl odpubutl` \
		`$(SHLB_REF) odcore odcore` \
		`$(SHLB_REF) odbindng odbindng` \
		`$(SHLB_REF) rhbxtutl rhbxtutl` \
		`$(SHLB_MAP) odui odui` \
		$(SOMTKLIBS) \
		$(XTLIBS) \
		$(X11LIBS) \
		$(STDLIB) \
		$(LINKDLL_TAIL) 

$(INTDIR)/odui.o: ../src/odui.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/odui.c -o $@

$(INTDIR)/MenuAdX.o: ../src/MenuAdX.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/MenuAdX.c -o $@

$(INTDIR)/Window.o: ../src/Window.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/Window.c -o $@

$(INTDIR)/WinStat.o: ../src/WinStat.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/WinStat.c -o $@

$(INTDIR)/WinIter.o: ../src/WinIter.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/WinIter.c -o $@

$(INTDIR)/Undo.o: ../src/Undo.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/Undo.c -o $@

$(INTDIR)/MenuBar.o: ../src/MenuBar.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/MenuBar.c -o $@

$(INTDIR)/Disptch.o: ../src/Disptch.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/Disptch.c -o $@

$(INTDIR)/DispMod.o: ../src/DispMod.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/DispMod.c -o $@

$(INTDIR)/StdDisp.o: ../src/StdDisp.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/StdDisp.c -o $@

$(INTDIR)/StdDispX.o: ../src/StdDispX.c
	$(CC_DLL) $(STDOPT) $(ODINCL) -c ../src/StdDispX.c -o $@

$(INTDIR)/Arbitrat.o: ../src/Arbitrat.c
	$(CC_DLL) $(STDOPT) $(ODINCL) -c ../src/Arbitrat.c -o $@

$(INTDIR)/FocusMod.o: ../src/FocusMod.c
	$(CC_DLL) $(STDOPT) $(ODINCL) -c ../src/FocusMod.c -o $@

$(INTDIR)/FocusSet.o: ../src/FocusSet.c
	$(CC_DLL) $(STDOPT) $(ODINCL) -c ../src/FocusSet.c -o $@

$(INTDIR)/FocusItr.o: ../src/FocusItr.c
	$(CC_DLL) $(STDOPT) $(ODINCL) -c ../src/FocusItr.c -o $@

$(INTDIR)/ExcFocus.o: ../src/ExcFocus.c
	$(CC_DLL) $(STDOPT) $(ODINCL) -c ../src/ExcFocus.c -o $@

$(INTDIR)/FocusOwn.o: ../src/FocusOwn.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/FocusOwn.c -o $@

$(INTDIR)/Popup.o: ../src/Popup.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/Popup.c -o $@

$(INTDIR)/odFrameX.o: ../src/odFrameX.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/odFrameX.c -o $@

$(INTDIR)/odShellX.o: ../src/odShellX.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/odShellX.c -o $@

$(INTDIR)/PltMenuX.o: ../src/PltMenuX.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/PltMenuX.c -o $@


dist install:

