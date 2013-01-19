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

ODINCL=-I$(INTDIR) 									\
		-I../include 								\
		-I../../odcore/$(PLATFORM)/$(BUILDTYPE) 	\
		-I../../odcore/include 						\
		-I../../odstorag/$(PLATFORM)/$(BUILDTYPE) 	\
		-I../../odlayout/$(PLATFORM)/$(BUILDTYPE) 	\
		-I../../odbindng/$(PLATFORM)/$(BUILDTYPE) 	\
		-I../../cmbento/include 					\
		-I../../rhbbento/include 					\
		-I../../odincl/include 						\
		-I../../somidl/$(PLATFORM) 					\
		-I../../msdev/include						\
		-I../../uuid/include						\
		-I../../dce/include							\
		-I../../somkpub/include						\
		-I../../somtk/include						\
		$(SOMTKINCL)								\
		$(STDINCL) 

TARGET=$(OUTDIR_SHLIB)/$(DLLPREFIX)odbento$(DLLSUFFIX)

OBJS=$(INTDIR)/odbento.o \
	$(INTDIR)/CMCtr.o \
	$(INTDIR)/CMDoc.o \
	$(INTDIR)/CMDraft.o \
	$(INTDIR)/CMLkItr.o \
	$(INTDIR)/CMLkSItr.o \
	$(INTDIR)/CMSU.o \
	$(INTDIR)/CMSURefI.o \
	$(INTDIR)/EmbedCtr.o \
	$(INTDIR)/FileCtr.o \
	$(INTDIR)/MemCtr.o \
	$(INTDIR)/UpdatCtr.o \
	$(INTDIR)/DocPriv.o \
	$(INTDIR)/OpenHash.o \
	$(INTDIR)/IDList.o \
	$(INTDIR)/rhbbento.o \
	$(INTDIR)/rhbfstrm.o \
	$(INTDIR)/rhbhstrm.o \
	$(INTDIR)/TargtHdr.o \
	$(INTDIR)/SURef.o \
	$(INTDIR)/IndHdr.o

all: all-$(HAVE_LIBXT)

all-false:

all-true: $(TARGET)

clean:
	$(CLEAN) $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(LINKDLL) $(LINKDLL_HEAD) \
		-o $@ \
		$(OBJS) \
		`$(SHLB_EXP) odbento odbento` \
		`$(SHLB_ENT) SOMInitModule` \
		`$(SHLB_REF) odbindng odbindng` \
		`$(SHLB_REF) odlayout odlayout` \
		`$(SHLB_REF) odutils odutils` \
		`$(SHLB_REF) oddatax oddatax` \
		`$(SHLB_REF) odcm odcm` \
		`$(SHLB_REF) odstorag odstorag` \
		`$(SHLB_REF) odpubutl odpubutl` \
		`$(SHLB_REF) odcore odcore` \
		`$(SHLB_MAP) odbento odbento` \
		$(SOMTKLIBS) \
		$(X11LIBS) \
		$(SOCKLIBS) \
		$(STDLIB) \
		$(LINKDLL_TAIL)

$(INTDIR)/odbento.o: ../src/odbento.c 
	$(CC_DLL) $(STDOPT) $(ODINCL) -c ../src/odbento.c -o $@

$(INTDIR)/CMCtr.o: ../src/CMCtr.c 
	$(CC_DLL) $(STDOPT) $(ODINCL) -c ../src/CMCtr.c -o $@

$(INTDIR)/CMDoc.o: ../src/CMDoc.c 
	$(CC_DLL) $(STDOPT) $(ODINCL) -c ../src/CMDoc.c -o $@

$(INTDIR)/CMDraft.o: ../src/CMDraft.c 
	$(CC_DLL) $(STDOPT) $(ODINCL) -c ../src/CMDraft.c -o $@

$(INTDIR)/CMLkItr.o: ../src/CMLkItr.c 
	$(CC_DLL) $(STDOPT) $(ODINCL) -c ../src/CMLkItr.c -o $@

$(INTDIR)/CMLkSItr.o: ../src/CMLkSItr.c 
	$(CC_DLL) $(STDOPT) $(ODINCL) -c ../src/CMLkSItr.c -o $@

$(INTDIR)/CMSU.o: ../src/CMSU.c 
	$(CC_DLL) $(STDOPT) $(ODINCL) -c ../src/CMSU.c -o $@

$(INTDIR)/CMSURefI.o: ../src/CMSURefI.c 
	$(CC_DLL) $(STDOPT) $(ODINCL) -c ../src/CMSURefI.c -o $@

$(INTDIR)/EmbedCtr.o: ../src/EmbedCtr.c 
	$(CC_DLL) $(STDOPT) $(ODINCL) -c ../src/EmbedCtr.c -o $@

$(INTDIR)/FileCtr.o: ../src/FileCtr.c 
	$(CC_DLL) $(STDOPT) $(ODINCL) -c ../src/FileCtr.c -o $@

$(INTDIR)/MemCtr.o: ../src/MemCtr.c 
	$(CC_DLL) $(STDOPT) $(ODINCL) -c ../src/MemCtr.c -o $@

$(INTDIR)/UpdatCtr.o: ../src/UpdatCtr.c 
	$(CC_DLL) $(STDOPT) $(ODINCL) -c ../src/UpdatCtr.c -o $@

$(INTDIR)/SURef.o: ../src/SURef.c 
	$(CC_DLL) $(STDOPT) $(ODINCL) -c ../src/SURef.c -o $@

$(INTDIR)/OpenHash.o: ../src/OpenHash.c 
	$(CC_DLL) $(STDOPT) $(ODINCL) -c ../src/OpenHash.c -o $@

$(INTDIR)/DocPriv.o: ../src/DocPriv.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/DocPriv.c -o $@

$(INTDIR)/IDList.o: ../src/IDList.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/IDList.c -o $@

$(INTDIR)/IndHdr.o: ../src/IndHdr.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/IndHdr.c -o $@

$(INTDIR)/rhbbento.o: ../../rhbbento/src/rhbbento.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../../rhbbento/src/rhbbento.c -o $@

$(INTDIR)/rhbfstrm.o: ../../rhbbento/src/rhbfstrm.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../../rhbbento/src/rhbfstrm.c -o $@

$(INTDIR)/rhbhstrm.o: ../../rhbbento/src/rhbhstrm.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../../rhbbento/src/rhbhstrm.c -o $@

$(INTDIR)/TargtHdr.o: ../../cmbento/src/TargtHdr.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../../cmbento/src/TargtHdr.c -o $@

dist install:

