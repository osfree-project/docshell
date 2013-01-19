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

ODINCL=	-I. \
		-I../include \
		-I../../odincl/include \
		-I../../somidl/$(PLATFORM) \
		-I../../somkpub/include	\
		-I../../somtk/include \
		$(SOMTKINCL) \
		$(STDINCL) -I../../odui/include $(SOMTKINCL)

TARGETDIR=$(OUTDIR_IMPLIB)

TARGETS=$(TARGETDIR)/$(DLLPREFIX)odstorag$(DLLSUFFIX) \
	$(TARGETDIR)/$(DLLPREFIX)oddatax$(DLLSUFFIX) \
	$(TARGETDIR)/$(DLLPREFIX)odimagng$(DLLSUFFIX) \
	$(TARGETDIR)/$(DLLPREFIX)odpubutl$(DLLSUFFIX) \
	$(TARGETDIR)/$(DLLPREFIX)odui$(DLLSUFFIX) \
	$(TARGETDIR)/$(DLLPREFIX)odbindng$(DLLSUFFIX) \
	$(TARGETDIR)/$(DLLPREFIX)odutils$(DLLSUFFIX) \
	$(TARGETDIR)/$(DLLPREFIX)odlayout$(DLLSUFFIX) \
	$(TARGETDIR)/$(DLLPREFIX)odbento$(DLLSUFFIX) \
	$(TARGETDIR)/$(DLLPREFIX)odcore$(DLLSUFFIX)

OBJS=	$(INTDIR)/odbento.o \
		$(INTDIR)/odstorag.o \
		$(INTDIR)/oddatax.o \
		$(INTDIR)/odimagng.o \
		$(INTDIR)/odpubutl.o \
		$(INTDIR)/odui.o \
		$(INTDIR)/odbindng.o \
		$(INTDIR)/odutils.o \
		$(INTDIR)/odlayout.o \
		$(INTDIR)/odcore.o

all: all-$(HAVE_LIBXT)

all-true: $(TARGETS)

all-false:

clean:
	$(CLEAN) $(OBJS) $(TARGETS)

$(TARGETDIR)/$(DLLPREFIX)oddatax$(DLLSUFFIX): $(INTDIR)/oddatax.o ../../oddatax/unix/oddatax.exp
	$(LINKDLL) $(LINKDLL_HEAD) \
		-o $@ \
		$(INTDIR)/oddatax.o \
		`$(SHLB_EXP) oddatax oddatax` \
		`$(SHLB_ENT) SOMInitModule` \
		`$(SHLB_MAP) oddatax oddatax` \
		$(STDLIB) \
		$(LINKDLL_TAIL)

$(TARGETDIR)/$(DLLPREFIX)odbindng$(DLLSUFFIX): $(INTDIR)/odbindng.o ../../odbindng/unix/odbindng.exp
	$(LINKDLL) $(LINKDLL_HEAD) \
		-o $@ \
		$(INTDIR)/odbindng.o \
		`$(SHLB_EXP) odbindng odbindng` \
		`$(SHLB_ENT) SOMInitModule` \
		`$(SHLB_MAP) odbindng odbindng` \
		$(STDLIB) \
		$(LINKDLL_TAIL)

$(TARGETDIR)/$(DLLPREFIX)odui$(DLLSUFFIX): $(INTDIR)/odui.o ../../odui/unix/odui.exp
	$(LINKDLL) $(LINKDLL_HEAD) \
		-o $@ \
		$(INTDIR)/odui.o \
		`$(SHLB_EXP) odui odui` \
		`$(SHLB_ENT) SOMInitModule` \
		`$(SHLB_MAP) odui odui` \
		$(STDLIB) \
		$(LINKDLL_TAIL)

$(TARGETDIR)/$(DLLPREFIX)odimagng$(DLLSUFFIX): $(INTDIR)/odimagng.o ../../odimagng/unix/odimagng.exp
	$(LINKDLL) $(LINKDLL_HEAD) \
		-o $@ \
		$(INTDIR)/odimagng.o \
		`$(SHLB_EXP) odimagng odimagng` \
		`$(SHLB_ENT) SOMInitModule` \
		`$(SHLB_MAP) odimagng odimagng` \
		$(STDLIB) \
		$(LINKDLL_TAIL)

$(TARGETDIR)/$(DLLPREFIX)odlayout$(DLLSUFFIX): $(INTDIR)/odlayout.o ../../odlayout/unix/odlayout.exp
	$(LINKDLL) $(LINKDLL_HEAD) \
		-o $@ \
		$(INTDIR)/odlayout.o \
		`$(SHLB_EXP) odlayout odlayout` \
		`$(SHLB_ENT) SOMInitModule` \
		`$(SHLB_MAP) odlayout odlayout` \
		$(STDLIB) \
		$(LINKDLL_TAIL)

$(TARGETDIR)/$(DLLPREFIX)odpubutl$(DLLSUFFIX): $(INTDIR)/odpubutl.o ../../odpubutl/unix/odpubutl.exp
	$(LINKDLL) $(LINKDLL_HEAD) \
		-o $@ \
		$(INTDIR)/odpubutl.o \
		`$(SHLB_EXP) odpubutl odpubutl` \
		`$(SHLB_MAP) odpubutl odpubutl` \
		$(STDLIB) \
		$(LINKDLL_TAIL)

$(TARGETDIR)/$(DLLPREFIX)odstorag$(DLLSUFFIX): $(INTDIR)/odstorag.o ../../odstorag/unix/odstorag.exp
	$(LINKDLL) $(LINKDLL_HEAD) \
		-o $@ \
		$(INTDIR)/odstorag.o \
		`$(SHLB_EXP) odstorag odstorag` \
		`$(SHLB_ENT) SOMInitModule` \
		`$(SHLB_MAP) odstorag odstorag` \
		$(STDLIB) \
		$(LINKDLL_TAIL)

$(TARGETDIR)/$(DLLPREFIX)odcore$(DLLSUFFIX): $(INTDIR)/odcore.o ../../odcore/unix/odcore.exp
	$(LINKDLL) $(LINKDLL_HEAD) \
		-o $@ \
		$(INTDIR)/odcore.o \
		`$(SHLB_EXP) odcore odcore` \
		`$(SHLB_MAP) odcore odcore` \
		$(STDLIB) \
		$(LINKDLL_TAIL)

$(TARGETDIR)/$(DLLPREFIX)odutils$(DLLSUFFIX): $(INTDIR)/odutils.o ../../odutils/unix/odutils.exp
	$(LINKDLL) $(LINKDLL_HEAD) \
		-o $@ \
		$(INTDIR)/odutils.o \
		`$(SHLB_EXP) odutils odutils` \
		`$(SHLB_MAP) odutils odutils` \
		$(STDLIB) \
		$(LINKDLL_TAIL)

$(TARGETDIR)/$(DLLPREFIX)odbento$(DLLSUFFIX): $(INTDIR)/odbento.o ../../odbento/unix/odbento.exp
	$(LINKDLL) $(LINKDLL_HEAD) \
		-o $@ \
		$(INTDIR)/odbento.o \
		`$(SHLB_EXP) odbento odbento` \
		`$(SHLB_ENT) SOMInitModule` \
		`$(SHLB_MAP) odbento odbento` \
		$(STDLIB) \
		$(LINKDLL_TAIL)

$(INTDIR)/odstorag.o: ../../odstorag/src/stub.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../../odstorag/src/stub.c -o $@

$(INTDIR)/odlayout.o: ../../odlayout/src/stub.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../../odlayout/src/stub.c -o $@

$(INTDIR)/odimagng.o: ../../odimagng/src/stub.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../../odimagng/src/stub.c -o $@

$(INTDIR)/odbindng.o: ../../odbindng/src/stub.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../../odbindng/src/stub.c -o $@

$(INTDIR)/odpubutl.o: ../../odpubutl/src/stub.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../../odpubutl/src/stub.c -o $@

$(INTDIR)/oddatax.o: ../../oddatax/src/stub.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../../oddatax/src/stub.c -o $@

$(INTDIR)/odui.o: ../../odui/src/stub.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../../odui/src/stub.c -o $@

$(INTDIR)/odcore.o: ../../odcore/src/stub.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../../odcore/src/stub.c -o $@

$(INTDIR)/odutils.o: ../../odutils/src/stub.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../../odutils/src/stub.c -o $@

$(INTDIR)/odbento.o: ../../odbento/src/stub.c
	$(CC_DLL) $(STDOPT) -I../../odbento/$(PLATFORM)/$(BUILDTYPE) $(ODINCL)  -c ../../odbento/src/stub.c -o $@

dist install:

