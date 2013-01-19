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

PART=odcm
PART_MAK=../$(PLATFORM_PROTO)/$(PART).mak

TARGET=$(OUTDIR_SHLIB)/$(DLLPREFIX)$(PART)$(DLLSUFFIX)

ODCMINCL=-I../../cmbento/include $(STDINCL)

OBJS=	$(INTDIR)/BufferIO.o  \
		$(INTDIR)/CMCntOps.o  \
		$(INTDIR)/CMDbgOps.o  \
		$(INTDIR)/CMErrOps.o  \
		$(INTDIR)/CMHndOps.o	\
		$(INTDIR)/CMObjOps.o	\
		$(INTDIR)/CMRefOps.o  \
		$(INTDIR)/CMSesOps.o  \
		$(INTDIR)/CMTPOps.o   \
		$(INTDIR)/CMValOps.o  \
		$(INTDIR)/CMVHOps.o	\
		$(INTDIR)/DynValus.o	\
		$(INTDIR)/FreeSpce.o  \
		$(INTDIR)/GlbNames.o  \
		$(INTDIR)/Handlers.o  \
		$(INTDIR)/ListMgr.o   \
		$(INTDIR)/SymTbMgr.o  \
		$(INTDIR)/TOCEnts.o	\
		$(INTDIR)/TOCIO.o     \
		$(INTDIR)/TOCObjs.o   \
		$(INTDIR)/Update.o	\
		$(INTDIR)/Utility.o   \
		$(INTDIR)/ValueRtn.o

all: $(TARGET)
	if test -f "$(PART_MAK)"; then \
		$(MAKE) \
			-f "$(PART_MAK)" \
			MAKEDEFS="$(MAKEDEFS)" \
			PLATFORM="$(PLATFORM)" \
			BUILDTYPE="$(BUILDTYPE)" \
			PLATFORM_HOST="$(PLATFORM_HOST)" \
			BUILDTYPE_HOST="$(BUILDTYPE_HOST)" \
			PLATFORM_PROTO="$(PLATFORM_PROTO)" \
			OBJS="$(OBJS)" \
			$@; \
		if test "$$?" != "0"; then exit 1; fi \
	fi

clean:
	$(CLEAN) $(OBJS) $(TARGET)
	if test -f "$(PART_MAK)"; then \
		$(MAKE) \
			-f "$(PART_MAK)" \
			MAKEDEFS="$(MAKEDEFS)" \
			PLATFORM="$(PLATFORM)" \
			BUILDTYPE="$(BUILDTYPE)" \
			PLATFORM_HOST="$(PLATFORM_HOST)" \
			BUILDTYPE_HOST="$(BUILDTYPE_HOST)" \
			PLATFORM_PROTO="$(PLATFORM_PROTO)" \
			$@; \
		if test "$$?" != "0"; then exit 1; fi \
	fi

$(TARGET): $(OBJS) 
	$(LINKDLL) $(LINKDLL_HEAD) \
		-o $@ \
		$(OBJS) \
		`$(SHLB_EXP) odcm odcm` \
		`$(SHLB_MAP) odcm odcm` \
		$(STDLIB) \
		$(LINKDLL_TAIL)

$(INTDIR)/BufferIO.o: ../../cmbento/src/BufferIO.c
	$(CC_DLL) $(STDOPT) $(ODCMINCL)  -c ../../cmbento/src/BufferIO.c -o $@

$(INTDIR)/CMCntOps.o: ../../cmbento/src/CMCntOps.c
	$(CC_DLL) $(STDOPT) $(ODCMINCL)  -c ../../cmbento/src/CMCntOps.c -o $@

$(INTDIR)/CMDbgOps.o: ../../cmbento/src/CMDbgOps.c
	$(CC_DLL) $(STDOPT) $(ODCMINCL)  -c ../../cmbento/src/CMDbgOps.c -o $@

$(INTDIR)/CMErrOps.o: ../../cmbento/src/CMErrOps.c
	$(CC_DLL) $(STDOPT) $(ODCMINCL)  -c ../../cmbento/src/CMErrOps.c -o $@

$(INTDIR)/CMHndOps.o: ../../cmbento/src/CMHndOps.c
	$(CC_DLL) $(STDOPT) $(ODCMINCL)  -c ../../cmbento/src/CMHndOps.c -o $@

$(INTDIR)/CMObjOps.o: ../../cmbento/src/CMObjOps.c
	$(CC_DLL) $(STDOPT) $(ODCMINCL)  -c ../../cmbento/src/CMObjOps.c -o $@

$(INTDIR)/CMRefOps.o: ../../cmbento/src/CMRefOps.c
	$(CC_DLL) $(STDOPT) $(ODCMINCL)  -c ../../cmbento/src/CMRefOps.c -o $@

$(INTDIR)/CMSesOps.o: ../../cmbento/src/CMSesOps.c
	$(CC_DLL) $(STDOPT) $(ODCMINCL)  -c ../../cmbento/src/CMSesOps.c -o $@

$(INTDIR)/CMTPOps.o: ../../cmbento/src/CMTPOps.c
	$(CC_DLL) $(STDOPT) $(ODCMINCL)  -c ../../cmbento/src/CMTPOps.c -o $@

$(INTDIR)/CMValOps.o: ../../cmbento/src/CMValOps.c
	$(CC_DLL) $(STDOPT) $(ODCMINCL)  -c ../../cmbento/src/CMValOps.c -o $@

$(INTDIR)/CMVHOps.o: ../../cmbento/src/CMVHOps.c
	$(CC_DLL) $(STDOPT) $(ODCMINCL)  -c ../../cmbento/src/CMVHOps.c -o $@

$(INTDIR)/DynValus.o: ../../cmbento/src/DynValus.c
	$(CC_DLL) $(STDOPT) $(ODCMINCL)  -c ../../cmbento/src/DynValus.c -o $@

$(INTDIR)/FreeSpce.o: ../../cmbento/src/FreeSpce.c
	$(CC_DLL) $(STDOPT) $(ODCMINCL)  -c ../../cmbento/src/FreeSpce.c -o $@

$(INTDIR)/GlbNames.o: ../../cmbento/src/GlbNames.c
	$(CC_DLL) $(STDOPT) $(ODCMINCL)  -c ../../cmbento/src/GlbNames.c -o $@

$(INTDIR)/Handlers.o: ../../cmbento/src/Handlers.c
	$(CC_DLL) $(STDOPT) $(ODCMINCL)  -c ../../cmbento/src/Handlers.c -o $@

$(INTDIR)/ListMgr.o: ../../cmbento/src/ListMgr.c
	$(CC_DLL) $(STDOPT) $(ODCMINCL)  -c ../../cmbento/src/ListMgr.c -o $@

$(INTDIR)/SymTbMgr.o: ../../cmbento/src/SymTbMgr.c
	$(CC_DLL) $(STDOPT) $(ODCMINCL)  -c ../../cmbento/src/SymTbMgr.c -o $@

$(INTDIR)/TOCEnts.o: ../../cmbento/src/TOCEnts.c
	$(CC_DLL) $(STDOPT) $(ODCMINCL)  -c ../../cmbento/src/TOCEnts.c -o $@

$(INTDIR)/TOCIO.o: ../../cmbento/src/TOCIO.c
	$(CC_DLL) $(STDOPT) $(ODCMINCL)  -c ../../cmbento/src/TOCIO.c -o $@

$(INTDIR)/TOCObjs.o: ../../cmbento/src/TOCObjs.c
	$(CC_DLL) $(STDOPT) $(ODCMINCL)  -c ../../cmbento/src/TOCObjs.c -o $@

$(INTDIR)/Update.o: ../../cmbento/src/Update.c
	$(CC_DLL) $(STDOPT) $(ODCMINCL)  -c ../../cmbento/src/Update.c -o $@

$(INTDIR)/Utility.o: ../../cmbento/src/Utility.c
	$(CC_DLL) $(STDOPT) $(ODCMINCL)  -c ../../cmbento/src/Utility.c -o $@

$(INTDIR)/ValueRtn.o: ../../cmbento/src/ValueRtn.c
	$(CC_DLL) $(STDOPT) $(ODCMINCL)  -c ../../cmbento/src/ValueRtn.c -o $@

dist install:

