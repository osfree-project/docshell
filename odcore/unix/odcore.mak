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

ODINCL=-I$(INTDIR) \
		-I../include \
		-I../../odincl/include \
		-I../../odui/$(PLATFORM)/$(BUILDTYPE) \
		-I../../odbindng/$(PLATFORM)/$(BUILDTYPE) \
		-I../../oddatax/$(PLATFORM)/$(BUILDTYPE) \
		-I../../odstorag/$(PLATFORM)/$(BUILDTYPE) \
		-I../../somidl/$(PLATFORM) \
		-I../../somkpub/include						\
		-I../../somtk/include						\
		$(SOMTKINCL) \
		$(STDINCL)

OBJS=	$(INTDIR)/odcore.o \
		$(INTDIR)/ODObject.o \
		$(INTDIR)/RefCtObj.o \
		$(INTDIR)/Extensn.o \
		$(INTDIR)/ODSessnB.o \
		$(INTDIR)/ODSessn.o \
		$(INTDIR)/PstObj.o \
		$(INTDIR)/EmbFrItr.o \
		$(INTDIR)/Part.o \
		$(INTDIR)/Page.o \
		$(INTDIR)/DocPage.o \
		$(INTDIR)/BindPage.o \
		$(INTDIR)/PresPage.o \
		$(INTDIR)/Notebook.o \
		$(INTDIR)/Info.o \
		$(INTDIR)/odhelp.o \
		$(INTDIR)/NamSpac.o \
		$(INTDIR)/ObjctItr.o \
		$(INTDIR)/ValueItr.o \
		$(INTDIR)/ValueNS.o \
		$(INTDIR)/ObjectNS.o \
		$(INTDIR)/Settings.o \
		$(INTDIR)/NmSpcMg.o \
		$(INTDIR)/odLL.o \
		$(INTDIR)/ToknTabl.o

TARGET=$(OUTDIR_SHLIB)/$(DLLPREFIX)odcore$(DLLSUFFIX)

all: all-$(HAVE_LIBXT)

all-false:

all-true: $(TARGET)

clean:
	$(CLEAN) $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(LINKDLL) $(LINKDLL_HEAD) \
		-o $@ \
		$(OBJS) \
		`$(SHLB_EXP) odcore odcore` \
		`$(SHLB_REF) odbindng odbindng` \
		`$(SHLB_REF) odui odui` \
		`$(SHLB_REF) oddatax oddatax` \
		`$(SHLB_REF) odstorag odstorag` \
		`$(SHLB_REF) odpubutl odpubutl` \
		`$(SHLB_MAP) odcore odcore` \
		$(SOMTKLIBS) \
		$(STDLIB) \
		$(LINKDLL_TAIL)

$(INTDIR)/odcore.o: ../src/odcore.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/odcore.c -o $@

$(INTDIR)/odLL.o: ../src/odLL.c 
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/odLL.c -o $@

$(INTDIR)/ODObject.o: ../src/ODObject.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/ODObject.c -o $@

$(INTDIR)/RefCtObj.o: ../src/RefCtObj.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/RefCtObj.c -o $@

$(INTDIR)/Extensn.o: ../src/Extensn.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/Extensn.c -o $@

$(INTDIR)/ODSessnB.o: ../src/ODSessnB.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/ODSessnB.c -o $@

$(INTDIR)/ODSessn.o: ../src/ODSessn.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/ODSessn.c -o $@

$(INTDIR)/PstObj.o: ../src/PstObj.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/PstObj.c -o $@

$(INTDIR)/Part.o: ../src/Part.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/Part.c -o $@

$(INTDIR)/EmbFrItr.o: ../src/EmbFrItr.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/EmbFrItr.c -o $@

$(INTDIR)/Info.o: ../src/Info.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/Info.c -o $@

$(INTDIR)/odhelp.o: ../src/odhelp.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/odhelp.c -o $@

$(INTDIR)/Settings.o: ../src/Settings.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/Settings.c -o $@

$(INTDIR)/ObjectNS.o: ../src/ObjectNS.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/ObjectNS.c -o $@

$(INTDIR)/ObjctItr.o: ../src/ObjctItr.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/ObjctItr.c -o $@

$(INTDIR)/ValueItr.o: ../src/ValueItr.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/ValueItr.c -o $@

$(INTDIR)/ValueNS.o: ../src/ValueNS.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/ValueNS.c -o $@

$(INTDIR)/NmSpcMg.o: ../src/NmSpcMg.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/NmSpcMg.c -o $@

$(INTDIR)/NamSpac.o: ../src/NamSpac.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/NamSpac.c -o $@

$(INTDIR)/ToknTabl.o: ../src/ToknTabl.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/ToknTabl.c -o $@

$(INTDIR)/Page.o: ../src/Page.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/Page.c -o $@

$(INTDIR)/Notebook.o: ../src/Notebook.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/Notebook.c -o $@

$(INTDIR)/DocPage.o: ../src/DocPage.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/DocPage.c -o $@

$(INTDIR)/BindPage.o: ../src/BindPage.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/BindPage.c -o $@

$(INTDIR)/PresPage.o: ../src/PresPage.c
	$(CC_DLL) $(STDOPT) $(ODINCL)  -c ../src/PresPage.c -o $@


dist install:

