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
		-I../../cmbento/include \
		-I../../registry \
		-I../../odcore \
		-I../../odui \
		-I../../odincl/include \
		-I../../somidl/$(PLATFORM) \
		-I../../somkpub/include						\
		-I../../somtk/include						\
		$(SOMTKINCL)								\
		$(STDINCL)

OBJS=$(INTDIR)/odregsty.o $(INTDIR)/ODPrtReg.o $(INTDIR)/ODPrtRei.o \
		$(INTDIR)/ODRegApi.o $(INTDIR)/PartCat.o $(INTDIR)/PartInfi.o \
		$(INTDIR)/PartInfo.o $(INTDIR)/PartKind.o $(INTDIR)/RManager.o
#		$(INTDIR)/xsession.o $(INTDIR)/xhandlrs.o $(INTDIR)/xembeded.o

TARGET=$(OUTDIR_SHLIB)/$(DLLPREFIX)odregsty$(DLLSUFFIX)

all: all-$(HAVE_LIBXT)

all-false:

all-true: $(TARGET)

clean:
	$(CLEAN) $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(LINKDLL) $(LINKDLL_HEAD) \
		-o $@ \
		$(OBJS) \
		`$(SHLB_EXP) odregsty odregsty` \
		`$(SHLB_ENT) SOMInitModule` \
		`$(SHLB_REF) odcm odcm` \
		`$(SHLB_REF) odui odui` \
		`$(SHLB_REF) odutils odutils` \
		`$(SHLB_REF) odstorag odstorag` \
		`$(SHLB_REF) odcore odcore` \
		`$(SHLB_REF) odbindng odbindng` \
		`$(SHLB_MAP) odregsty odregsty` \
		$(SOMTKLIBS) \
		$(X11LIBS) $(STDLIB) \
		$(LINKDLL_TAIL)

$(INTDIR)/odregsty.o: ../src/odregsty.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/odregsty.c -o $@

$(INTDIR)/ODPrtReg.o: ../src/ODPrtReg.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/ODPrtReg.c -o $@

$(INTDIR)/ODPrtRei.o: ../src/ODPrtRei.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/ODPrtRei.c -o $@

$(INTDIR)/ODRegApi.o: ../src/ODRegApi.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/ODRegApi.c -o $@

$(INTDIR)/PartCat.o: ../src/PartCat.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/PartCat.c -o $@

$(INTDIR)/PartInfi.o: ../src/PartInfi.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/PartInfi.c -o $@

$(INTDIR)/PartInfo.o: ../src/PartInfo.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/PartInfo.c -o $@

$(INTDIR)/PartKind.o: ../src/PartKind.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/PartKind.c -o $@

$(INTDIR)/RManager.o: ../src/RManager.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/RManager.c -o $@

$(INTDIR)/xsession.o: ../../registry/src/xsession.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../../registry/src/xsession.c -o $@

$(INTDIR)/xhandlrs.o: ../../registry/src/xhandlrs.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../../registry/src/xhandlrs.c -o $@

$(INTDIR)/xembeded.o: ../../registry/xembeded.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../../registry/src/xembeded.c -o $@


dist install:

