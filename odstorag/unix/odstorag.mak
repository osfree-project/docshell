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
		-I../../odcore/$(PLATFORM) \
		-I../../odcore/include \
		-I../../odincl/include \
		-I../../somidl/$(PLATFORM) \
		-I../../somkpub/include						\
		-I../../somtk/include						\
		$(SOMTKINCL) 								\
		$(STDINCL) 

TARGET=$(OUTDIR_SHLIB)/$(DLLPREFIX)odstorag$(DLLSUFFIX)
OBJS=	$(INTDIR)/Document.o \
		$(INTDIR)/Draft.o \
		$(INTDIR)/ODCtr.o \
		$(INTDIR)/ODStor.o \
		$(INTDIR)/odstorag.o \
		$(INTDIR)/PfTLItr.o \
		$(INTDIR)/PfTypLs.o \
		$(INTDIR)/StorageU.o \
		$(INTDIR)/SUCursor.o \
		$(INTDIR)/SURefItr.o \
		$(INTDIR)/SUView.o \
		$(INTDIR)/TypeList.o \
		$(INTDIR)/TypLsItr.o

all: all-$(HAVE_LIBXT)

all-false:

all-true: $(TARGET)

clean:
	$(CLEAN) $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(LINKDLL) $(LINKDLL_HEAD) \
		-o $@ \
		$(OBJS) \
		`$(SHLB_EXP) odstorag odstorag` \
		`$(SHLB_ENT) SOMInitModule` \
		`$(SHLB_REF) odbindng odbindng` \
		`$(SHLB_REF) odcore odcore` \
		`$(SHLB_REF) odui odui` \
		`$(SHLB_REF) oddatax oddatax` \
		`$(SHLB_REF) odpubutl odpubutl` \
		`$(SHLB_MAP) odstorag odstorag` \
		$(SOMTKLIBS) \
		$(STDLIB) \
		$(LINKDLL_TAIL)

$(INTDIR)/odstorag.o: ../src/odstorag.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/odstorag.c -o $@

$(INTDIR)/Document.o: ../src/Document.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/Document.c -o $@

$(INTDIR)/Draft.o: ../src/Draft.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/Draft.c -o $@

$(INTDIR)/ODCtr.o: ../src/ODCtr.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/ODCtr.c -o $@

$(INTDIR)/ODStor.o: ../src/ODStor.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/ODStor.c -o $@

$(INTDIR)/PfTLItr.o: ../src/PfTLItr.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/PfTLItr.c -o $@

$(INTDIR)/PfTypLs.o: ../src/PfTypLs.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/PfTypLs.c -o $@

$(INTDIR)/StorageU.o: ../src/StorageU.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/StorageU.c -o $@

$(INTDIR)/SUCursor.o: ../src/SUCursor.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/SUCursor.c -o $@

$(INTDIR)/SURefItr.o: ../src/SURefItr.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/SURefItr.c -o $@

$(INTDIR)/SUView.o: ../src/SUView.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/SUView.c -o $@

$(INTDIR)/TypeList.o: ../src/TypeList.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/TypeList.c -o $@

$(INTDIR)/TypLsItr.o: ../src/TypLsItr.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/TypLsItr.c -o $@


dist install:

