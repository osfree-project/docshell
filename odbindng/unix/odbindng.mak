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
		-I../../odcore/$(PLATFORM)/$(BUILDTYPE) \
		-I../../odcore/include \
		-I../../odstorag/$(PLATFORM)/$(BUILDTYPE) \
		-I../../odbento/include \
		-I../../odincl/include \
		-I../../rhbxtutl/include \
		-I../../somidl/$(PLATFORM) \
		-I../../somkpub/include						\
		-I../../somtk/include						\
		$(SOMTKINCL)								\
		$(STDINCL) 

OBJS=$(INTDIR)/rhbbind.o \
	$(INTDIR)/NoPart.o \
	$(INTDIR)/ODBindng.o \
	$(INTDIR)/PartWrap.o

TARGET=$(OUTDIR_SHLIB)/$(DLLPREFIX)odbindng$(DLLSUFFIX)

all: all-$(HAVE_LIBXT)

all-false:

all-true: $(TARGET)

clean:
	$(CLEAN) $(TARGET) $(OBJS) $(INTDIR)/odbinddl.h

$(TARGET): $(OBJS)
	$(LINKDLL) $(LINKDLL_HEAD) \
		-o $@ \
		$(OBJS) \
		`$(SHLB_EXP) odbindng odbindng` \
		`$(SHLB_ENT) SOMInitModule` \
		`$(SHLB_REF) odregsty odregsty` \
		`$(SHLB_REF) odui odui` \
		`$(SHLB_REF) odlayout odlayout` \
		`$(SHLB_REF) odimagng odimagng` \
		`$(SHLB_REF) odutils odutils` \
		`$(SHLB_REF) odpubutl odpubutl` \
		`$(SHLB_REF) odstorag odstorag` \
		`$(SHLB_REF) odcore odcore` \
		`$(SHLB_REF) rhbxtutl rhbxtutl` \
		`$(SHLB_MAP) odbindng odbindng` \
		$(SOMTKLIBS)					\
		$(XTLIBS) $(X11LIBS) $(STDLIB)   \
		$(LINKDLL_TAIL)

$(INTDIR)/rhbbind.o: ../src/rhbbind.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/rhbbind.c -o $@

$(INTDIR)/NoPart.o: ../src/NoPart.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/NoPart.c -o $@

$(INTDIR)/PartWrap.o: ../src/PartWrap.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/PartWrap.c -o $@

$(INTDIR)/ODBindng.o: ../src/ODBindng.c $(INTDIR)/odbinddl.h
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/ODBindng.c -o $@

$(INTDIR)/odbinddl.h: ../unix/odbindng.mak 
	$(SHLB_DLO) -s odbento >$@
	cat $@

dist install:

