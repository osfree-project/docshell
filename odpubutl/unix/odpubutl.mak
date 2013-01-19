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
		-I.. \
		-I../../odincl/include \
		-I../../somidl/$(PLATFORM) \
		-I../../somkpub/include						\
		-I../../somtk/include						\
		$(SOMTKINCL) \
		$(STDINCL)
OBJS=$(INTDIR)/IText.o $(INTDIR)/ODExcept.o
TARGET=$(OUTDIR_SHLIB)/$(DLLPREFIX)odpubutl$(DLLSUFFIX)

all: all-$(HAVE_LIBXT)

all-false:

all-true: $(TARGET)

clean:
	$(CLEAN) $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(LINKDLL) $(LINKDLL_HEAD) \
		-o $@ \
		$(OBJS) \
		`$(SHLB_EXP) odpubutl odpubutl` \
		`$(SHLB_MAP) odpubutl odpubutl` \
		$(SOMTKLIBS) \
		$(X11LIBS) \
		$(SOCKLIBS) \
		$(STDLIB) \
		$(LINKDLL_TAIL) 

$(INTDIR)/IText.o: ../src/IText.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/IText.c -o $@

$(INTDIR)/ODExcept.o: ../src/ODExcept.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/ODExcept.c -o $@


dist install:

