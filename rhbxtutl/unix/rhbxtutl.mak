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

TARGET=$(OUTDIR_SHLIB)/$(SHLIBPREFIX)rhbxtutl$(SHLIBSUFFIX)
OBJS=	$(INTDIR)/rhbxtutl.o 	\
		$(INTDIR)/rhbmenus.o 	\
		$(INTDIR)/rhbdrawx.o 	\
		$(INTDIR)/rhbmenui.o 	\
		$(INTDIR)/rhbmenuw.o	\
		$(INTDIR)/rhbxtwdg.o	\
		$(INTDIR)/rhbxtmgr.o	\
		$(INTDIR)/rhbxtsep.o	\
		$(INTDIR)/rhbxttgb.o	\
		$(INTDIR)/rhbxtlbl.o	\
		$(INTDIR)/rhbxtrcw.o	\
		$(INTDIR)/rhbxtmnw.o	\
		$(INTDIR)/rhbxtscw.o	\
		$(INTDIR)/rhbxtcbw.o

INCL=	-I$(INTDIR) \
		-I../include $(STDINCL) \
		-I../../region/$(PLATFORM)/$(BUILDTYPE)

all: all-$(HAVE_LIBXT)

all-true: $(TARGET)

all-false:

clean:
	$(CLEAN) $(OBJS) $(TARGET)

$(TARGET): $(OBJS) 
	$(LINKDLL) $(LINKDLL_HEAD) \
		-o $@ \
		$(OBJS) \
		`$(SHLB_EXP) rhbxtutl rhbxtutl` \
		`$(SHLB_MAP) rhbxtutl rhbxtutl` \
		$(XTLIBS) \
		$(X11LIBS) \
		$(SOCKLIBS) \
		$(STDLIB) \
		$(LINKDLL_TAIL)

$(INTDIR)/rhbxtutl.o: ../src/rhbxtutl.c
	$(CC_DLL) $(STDOPT) $(INCL)  -c ../src/rhbxtutl.c -o $@

$(INTDIR)/rhbxtmgr.o: ../src/rhbxtmgr.c
	$(CC_DLL) $(STDOPT) $(INCL)  -c ../src/rhbxtmgr.c -o $@

$(INTDIR)/rhbxtwdg.o: ../src/rhbxtwdg.c
	$(CC_DLL) $(STDOPT) $(INCL)  -c ../src/rhbxtwdg.c -o $@

$(INTDIR)/rhbxtlbl.o: ../src/rhbxtlbl.c
	$(CC_DLL) $(STDOPT) $(INCL)  -c ../src/rhbxtlbl.c -o $@

$(INTDIR)/rhbxtcbw.o: ../src/rhbxtcbw.c
	$(CC_DLL) $(STDOPT) $(INCL)  -c ../src/rhbxtcbw.c -o $@

$(INTDIR)/rhbxtmnw.o: ../src/rhbxtmnw.c
	$(CC_DLL) $(STDOPT) $(INCL)  -c ../src/rhbxtmnw.c -o $@

$(INTDIR)/rhbxtscw.o: ../src/rhbxtscw.c
	$(CC_DLL) $(STDOPT) $(INCL)  -c ../src/rhbxtscw.c -o $@

$(INTDIR)/rhbxttgb.o: ../src/rhbxttgb.c
	$(CC_DLL) $(STDOPT) $(INCL)  -c ../src/rhbxttgb.c -o $@

$(INTDIR)/rhbxtrcw.o: ../src/rhbxtrcw.c
	$(CC_DLL) $(STDOPT) $(INCL)  -c ../src/rhbxtrcw.c -o $@

$(INTDIR)/rhbxtsep.o: ../src/rhbxtsep.c
	$(CC_DLL) $(STDOPT) $(INCL)  -c ../src/rhbxtsep.c -o $@

$(INTDIR)/rhbdrawx.o: ../src/rhbdrawx.c
	$(CC_DLL) $(STDOPT) $(INCL)  -c ../src/rhbdrawx.c -o $@

$(INTDIR)/rhbmenus.o: ../../rhbmenus/src/rhbmenus.c
	$(CC_DLL) $(STDOPT) $(INCL) -I../../rhbmenus/include  -c ../../rhbmenus/src/rhbmenus.c -o $@

$(INTDIR)/rhbmenui.o: ../../rhbmenus/src/rhbmenui.c
	$(CC_DLL) $(STDOPT) $(INCL) -I../../rhbmenus/include  -c ../../rhbmenus/src/rhbmenui.c -o $@

$(INTDIR)/rhbmenuw.o: ../../rhbmenus/src/rhbmenuw.c
	$(CC_DLL) $(STDOPT) $(INCL) -I../../rhbmenus/include  -c ../../rhbmenus/src/rhbmenuw.c -o $@


dist install:

