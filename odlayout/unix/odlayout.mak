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
		-I../../odstorag/$(PLATFORM)/$(BUILDTYPE) \
		-I../../odui/$(PLATFORM)/$(BUILDTYPE) \
		-I../../odimagng/$(PLATFORM)/$(BUILDTYPE) \
		-I../../odcore/include \
		-I../../odui \
		-I../../odincl/include \
		-I../../rhbxtutl/include \
		-I../../somidl/$(PLATFORM) \
		-I../../somkpub/include						\
		-I../../somtk/include						\
		$(SOMTKINCL)								\
		$(STDINCL)

OBJS=	$(INTDIR)/odlayout.o \
		$(INTDIR)/Facet.o \
		$(INTDIR)/FacetItr.o \
		$(INTDIR)/Frame.o \
		$(INTDIR)/FrFaItr.o \
		$(INTDIR)/Border.o

TARGET=$(OUTDIR_SHLIB)/$(DLLPREFIX)odlayout$(DLLSUFFIX)

all: all-$(HAVE_LIBXT)

all-false:

all-true: $(TARGET)

clean:
	$(CLEAN) $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(LINKDLL) $(LINKDLL_HEAD) \
		-o $@ \
		$(OBJS) \
		`$(SHLB_EXP) odlayout odlayout` \
		`$(SHLB_ENT) SOMInitModule` \
		`$(SHLB_REF) odui odui` \
		`$(SHLB_REF) odbindng odbindng` \
		`$(SHLB_REF) odimagng odimagng` \
		`$(SHLB_REF) odutils odutils` \
		`$(SHLB_REF) odstorag odstorag` \
		`$(SHLB_REF) odpubutl odpubutl` \
		`$(SHLB_REF) odcore odcore` \
		`$(SHLB_REF) rhbxtutl rhbxtutl` \
		`$(SHLB_MAP) odlayout odlayout` \
		$(SOMTKLIBS) \
		$(XTLIBS) $(X11LIBS) $(STDLIB) \
		$(LINKDLL_TAIL)

$(INTDIR)/odlayout.o: ../src/odlayout.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/odlayout.c -o $@

$(INTDIR)/Facet.o: ../src/Facet.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/Facet.c -o $@

$(INTDIR)/FacetItr.o: ../src/FacetItr.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/FacetItr.c -o $@

$(INTDIR)/Frame.o: ../src/Frame.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/Frame.c -o $@

$(INTDIR)/FrFaItr.o: ../src/FrFaItr.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/FrFaItr.c -o $@

$(INTDIR)/Border.o: ../src/Border.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/Border.c -o $@


dist install:

