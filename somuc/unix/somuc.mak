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

PART=somuc

OBJS=	$(INTDIR)/rhbsomuc.o \
		$(INTDIR)/mlink.o \
		$(INTDIR)/tpll.o \
		$(INTDIR)/tpllitr.o 

INCL=	-I$(INTDIR) \
		-I../include  \
		-I../../somidl/$(PLATFORM) \
		-I../../somkpub/include	\
		-I../../somtk/include	\
		$(SOMTKINCL) \
		$(STDINCL)

TARGET=$(OUTDIR_SHLIB)/$(DLLPREFIX)$(PART)$(DLLSUFFIX)

all: $(TARGET)

clean:
	$(CLEAN) $(TARGET) $(OBJS)

$(TARGET): $(OBJS) 
	$(LINKDLL) $(LINKDLL_HEAD) \
		-o $@ \
		$(OBJS) \
		`$(SHLB_EXP) $(PART) $(PART)` \
		`$(SHLB_ENT) SOMInitModule` \
		`$(SHLB_MAP) $(PART) $(PART)` \
		$(SOMTKLIBS) \
		$(STDLIB) \
		$(LINKDLL_TAIL)

$(INTDIR)/rhbsomuc.o: ../src/rhbsomuc.c
	$(CC_DLL) $(STDOPT) $(INCL)  -DBUILD_SOMUC -c ../src/rhbsomuc.c -o $@

$(INTDIR)/mlink.o: ../src/mlink.c
	$(CC_DLL) $(STDOPT) $(INCL)  -DBUILD_SOMUC -c ../src/mlink.c -o $@

$(INTDIR)/tpll.o: ../src/tpll.c
	$(CC_DLL) $(STDOPT) $(INCL)  -DBUILD_SOMUC -c ../src/tpll.c -o $@

$(INTDIR)/tpllitr.o: ../src/tpllitr.c
	$(CC_DLL) $(STDOPT) $(INCL)  -DBUILD_SOMUC -c ../src/tpllitr.c -o $@


dist install:

