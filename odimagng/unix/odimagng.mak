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
		-I../../odincl/include	\
		-I../../somidl/$(PLATFORM) \
		-I../../somkpub/include						\
		-I../../somtk/include						\
		$(SOMTKINCL)								\
		$(STDINCL) 
TARGET=$(OUTDIR_SHLIB)/$(DLLPREFIX)odimagng$(DLLSUFFIX)

OBJS=	$(INTDIR)/odimagng.o \
		$(INTDIR)/PlatCanv.o \
		$(INTDIR)/Canvas.o \
		$(INTDIR)/CanvasB.o \
		$(INTDIR)/AIXPCanv.o \
		$(INTDIR)/AIXSCanv.o \
		$(INTDIR)/AIXWCanv.o \
		$(INTDIR)/ShapeB.o \
		$(INTDIR)/Shape.o \
		$(INTDIR)/TrnsfrmB.o \
		$(INTDIR)/Trnsform.o \
		$(INTDIR)/Matrix.o \
		$(INTDIR)/rhbshape.o

all: all-$(HAVE_LIBXT)

all-false:

all-true: $(TARGET)

clean:
	$(CLEAN) $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(LINKDLL) $(LINKDLL_HEAD) \
		-o $@ \
		$(OBJS) \
		`$(SHLB_EXP) odimagng odimagng` \
		`$(SHLB_ENT) SOMInitModule` \
		`$(SHLB_REF) odui odui` \
		`$(SHLB_REF) odbindng odbindng` \
		`$(SHLB_REF) odutils odutils` \
		`$(SHLB_REF) odpubutl odpubutl` \
		`$(SHLB_REF) odstorag odstorag` \
		`$(SHLB_REF) odcore odcore` \
		`$(SHLB_MAP) odimagng odimagng` \
		$(SOMTKLIBS) \
		$(X11LIBS) \
		$(SOCKLIBS) \
		$(STDLIB) \
		$(LINKDLL_TAIL)

$(INTDIR)/odimagng.o: ../src/odimagng.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/odimagng.c -o $@

$(INTDIR)/PlatCanv.o: ../src/PlatCanv.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/PlatCanv.c -o $@

$(INTDIR)/Canvas.o: ../src/Canvas.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/Canvas.c -o $@

$(INTDIR)/CanvasB.o: ../src/CanvasB.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/CanvasB.c -o $@

$(INTDIR)/AIXPCanv.o: ../src/AIXPCanv.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/AIXPCanv.c -o $@

$(INTDIR)/AIXSCanv.o: ../src/AIXSCanv.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/AIXSCanv.c -o $@

$(INTDIR)/AIXWCanv.o: ../src/AIXWCanv.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/AIXWCanv.c -o $@

$(INTDIR)/ShapeB.o: ../src/ShapeB.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/ShapeB.c -o $@

$(INTDIR)/Shape.o: ../src/Shape.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/Shape.c -o $@

$(INTDIR)/TrnsfrmB.o: ../src/TrnsfrmB.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/TrnsfrmB.c -o $@

$(INTDIR)/Trnsform.o: ../src/Trnsform.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/Trnsform.c -o $@

$(INTDIR)/Matrix.o: ../src/Matrix.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/Matrix.c -o $@

$(INTDIR)/rhbshape.o: ../src/rhbshape.c ../../region/$(PLATFORM)/$(BUILDTYPE)/region.h
	$(CC_DLL) $(STDOPT) -I../../region/$(PLATFORM)/$(BUILDTYPE) $(ODINCL)  -c ../src/rhbshape.c -o $@

dist install:

