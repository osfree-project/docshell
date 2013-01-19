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

ODINCL=-I$(INTDIR) \
		-I../include \
		-I../../odincl/include \
		-I../../rhbmenus \
		-I../../somidl/$(PLATFORM) \
		-I../../somkpub/include						\
		-I../../somtk/include						\
		$(SOMTKINCL) \
		$(STDINCL) 

TARGET=$(OUTDIR_SHLIB)/$(DLLPREFIX)oddatax$(DLLSUFFIX)

OBJS=	$(INTDIR)/Clipbd.o \
		$(INTDIR)/DgItmIt.o \
		$(INTDIR)/DragDrp.o \
		$(INTDIR)/Link.o \
		$(INTDIR)/LinkB.o \
		$(INTDIR)/LinkItr.o \
		$(INTDIR)/LinkMgr.o \
		$(INTDIR)/LinkSpcB.o \
		$(INTDIR)/LinkSpec.o \
		$(INTDIR)/LinkSrc.o \
		$(INTDIR)/LinkSrcB.o \
		$(INTDIR)/LkSrcItr.o \
		$(INTDIR)/oddatax.o \
		$(INTDIR)/Translt.o

all: all-$(HAVE_LIBXT)

all-false:

all-true: $(TARGET)

clean:
	$(CLEAN) $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(LINKDLL) $(LINKDLL_HEAD) \
		-o $@ \
		$(OBJS) \
		`$(SHLB_EXP) oddatax oddatax` \
		`$(SHLB_ENT) SOMInitModule` \
		`$(SHLB_REF) odcore odcore` \
		`$(SHLB_REF) odui odui` \
		`$(SHLB_REF) odstorag odstorag` \
		`$(SHLB_REF) odbindng odbindng` \
		`$(SHLB_REF) odpubutl odpubutl` \
		`$(SHLB_MAP) oddatax oddatax` \
		$(SOMTKLIBS) \
		$(X11LIBS) \
		$(SOCKLIBS) \
		$(STDLIB)  \
		$(LINKDLL_TAIL)

$(INTDIR)/oddatax.o: ../src/oddatax.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/oddatax.c -o $@

$(INTDIR)/Clipbd.o: ../src/Clipbd.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/Clipbd.c -o $@

$(INTDIR)/DgItmIt.o: ../src/DgItmIt.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/DgItmIt.c -o $@

$(INTDIR)/DragDrp.o: ../src/DragDrp.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/DragDrp.c -o $@

$(INTDIR)/Link.o: ../src/Link.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/Link.c -o $@

$(INTDIR)/LinkB.o: ../src/LinkB.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/LinkB.c -o $@

$(INTDIR)/LinkItr.o: ../src/LinkItr.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/LinkItr.c -o $@

$(INTDIR)/LinkMgr.o: ../src/LinkMgr.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/LinkMgr.c -o $@

$(INTDIR)/LinkSpcB.o: ../src/LinkSpcB.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/LinkSpcB.c -o $@

$(INTDIR)/LinkSpec.o: ../src/LinkSpec.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/LinkSpec.c -o $@

$(INTDIR)/LinkSrc.o: ../src/LinkSrc.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/LinkSrc.c -o $@

$(INTDIR)/LinkSrcB.o: ../src/LinkSrcB.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/LinkSrcB.c -o $@

$(INTDIR)/LkSrcItr.o: ../src/LkSrcItr.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/LkSrcItr.c -o $@

$(INTDIR)/Translt.o: ../src/Translt.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/Translt.c -o $@


dist install:

