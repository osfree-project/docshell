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
OBJS=	$(INTDIR)/rhbxtutl.o 

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

dist install:

