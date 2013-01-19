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

IDLTOOL=../../toolbox/idltool.sh
SOMIDL_IDL=../../somidl
SOMIDL_HEADERS=$(SOMIDL_IDL)/$(PLATFORM)

ODCORE_DIR=../../odcore
ODBENTO_DIR=../../odbento
ODSTORAG_DIR=../../odstorag
ODUI_DIR=../../odui
ODDATAX_DIR=../../oddatax
ODBINDNG_DIR=../../odbindng
ODIMAGNG_DIR=../../odimagng
ODLAYOUT_DIR=../../odlayout
ODSHELL_DIR=../../odshell
ODREGSTY_DIR=../../odregsty

ODCORE_INTDIR=$(ODCORE_DIR)/$(PLATFORM)/$(BUILDTYPE)
ODBENTO_INTDIR=$(ODBENTO_DIR)/$(PLATFORM)/$(BUILDTYPE)
ODSTORAG_INTDIR=$(ODSTORAG_DIR)/$(PLATFORM)/$(BUILDTYPE)
ODUI_INTDIR=$(ODUI_DIR)/$(PLATFORM)/$(BUILDTYPE)
ODDATAX_INTDIR=$(ODDATAX_DIR)/$(PLATFORM)/$(BUILDTYPE)
ODBINDNG_INTDIR=$(ODBINDNG_DIR)/$(PLATFORM)/$(BUILDTYPE)
ODIMAGNG_INTDIR=$(ODIMAGNG_DIR)/$(PLATFORM)/$(BUILDTYPE)
ODLAYOUT_INTDIR=$(ODLAYOUT_DIR)/$(PLATFORM)/$(BUILDTYPE)
ODSHELL_INTDIR=$(ODSHELL_DIR)/$(PLATFORM)/$(BUILDTYPE)
ODREGSTY_INTDIR=$(ODREGSTY_DIR)/$(PLATFORM)/$(BUILDTYPE)

EMIT_CONSTANTS=								\
			$(ODCORE_INTDIR)/ErrorDef.ih	\
			$(ODCORE_INTDIR)/ErrorDef.h		\
			$(ODCORE_INTDIR)/StdDefs.ih		\
			$(ODCORE_INTDIR)/StdExts.ih		\
			$(ODCORE_INTDIR)/StdProps.ih	\
			$(ODCORE_INTDIR)/StdTypes.ih	\
			$(ODCORE_INTDIR)/StdDefs.h		\
			$(ODCORE_INTDIR)/StdExts.h		\
			$(ODCORE_INTDIR)/StdProps.h		\
			$(ODCORE_INTDIR)/CmdDefs.h		\
			$(ODCORE_INTDIR)/CmdDefs.ih		\
			$(ODCORE_INTDIR)/StdTypes.h		\
			$(ODUI_INTDIR)/Foci.h			\
			$(ODUI_INTDIR)/Foci.ih

EMIT_TYPES=									\
			$(ODCORE_INTDIR)/ODTypesB.h		\
			$(ODCORE_INTDIR)/ODTypesM.h		\
			$(ODCORE_INTDIR)/ODTypesB.ih	\
			$(ODCORE_INTDIR)/ODTypesM.ih	\
			$(ODCORE_INTDIR)/GeoTypes.h		\
			$(ODCORE_INTDIR)/GeoTypes.ih

EMIT_POLYGON=								\
			$(ODCORE_INTDIR)/Polygon.h		\
			$(ODCORE_INTDIR)/Polygon.ih

ODCORE_H=	$(ODCORE_INTDIR)/BindPage.h		\
			$(ODCORE_INTDIR)/DocPage.h		\
			$(ODCORE_INTDIR)/EmbFrItr.h		\
			$(ODCORE_INTDIR)/Extensn.h		\
			$(ODCORE_INTDIR)/Info.h			\
			$(ODCORE_INTDIR)/NamSpac.h		\
			$(ODCORE_INTDIR)/NmSpcMg.h		\
			$(ODCORE_INTDIR)/Notebook.h		\
			$(ODCORE_INTDIR)/ObjctItr.h		\
			$(ODCORE_INTDIR)/ObjectNS.h		\
			$(ODCORE_INTDIR)/odhelp.h		\
			$(ODCORE_INTDIR)/ODObject.h		\
			$(ODCORE_INTDIR)/ODRgstry.h		\
			$(ODCORE_INTDIR)/ODSessn.h		\
			$(ODCORE_INTDIR)/ODSessnB.h		\
			$(ODCORE_INTDIR)/Page.h			\
			$(ODCORE_INTDIR)/Part.h			\
			$(ODCORE_INTDIR)/PresPage.h		\
			$(ODCORE_INTDIR)/PstObj.h		\
			$(ODCORE_INTDIR)/RefCtObj.h		\
			$(ODCORE_INTDIR)/Settings.h		\
			$(ODCORE_INTDIR)/ValueItr.h		\
			$(ODCORE_INTDIR)/ValueNS.h		

ODCORE_IH=	$(ODCORE_INTDIR)/BindPage.ih	\
			$(ODCORE_INTDIR)/DocPage.ih		\
			$(ODCORE_INTDIR)/EmbFrItr.ih	\
			$(ODCORE_INTDIR)/Extensn.ih		\
			$(ODCORE_INTDIR)/Info.ih		\
			$(ODCORE_INTDIR)/NamSpac.ih		\
			$(ODCORE_INTDIR)/NmSpcMg.ih		\
			$(ODCORE_INTDIR)/Notebook.ih	\
			$(ODCORE_INTDIR)/ObjctItr.ih	\
			$(ODCORE_INTDIR)/ObjectNS.ih	\
			$(ODCORE_INTDIR)/odhelp.ih		\
			$(ODCORE_INTDIR)/ODObject.ih	\
			$(ODCORE_INTDIR)/ODRgstry.ih	\
			$(ODCORE_INTDIR)/ODSessn.ih		\
			$(ODCORE_INTDIR)/ODSessnB.ih	\
			$(ODCORE_INTDIR)/Page.ih		\
			$(ODCORE_INTDIR)/Part.ih		\
			$(ODCORE_INTDIR)/PresPage.ih	\
			$(ODCORE_INTDIR)/PstObj.ih		\
			$(ODCORE_INTDIR)/RefCtObj.ih	\
			$(ODCORE_INTDIR)/Settings.ih	\
			$(ODCORE_INTDIR)/ValueItr.ih	\
			$(ODCORE_INTDIR)/ValueNS.ih		

ODBENTO_H=	$(ODBENTO_INTDIR)/CMCtr.h		\
			$(ODBENTO_INTDIR)/CMDoc.h		\
			$(ODBENTO_INTDIR)/CMDraft.h		\
			$(ODBENTO_INTDIR)/CMLkItr.h		\
			$(ODBENTO_INTDIR)/CMLkSItr.h	\
			$(ODBENTO_INTDIR)/CMSU.h		\
			$(ODBENTO_INTDIR)/CMSURefI.h	\
			$(ODBENTO_INTDIR)/EmbedCtr.h	\
			$(ODBENTO_INTDIR)/FileCtr.h		\
			$(ODBENTO_INTDIR)/MemCtr.h		\
			$(ODBENTO_INTDIR)/UpdatCtr.h

ODBENTO_IH=	$(ODBENTO_INTDIR)/CMCtr.ih		\
			$(ODBENTO_INTDIR)/CMDoc.ih		\
			$(ODBENTO_INTDIR)/CMDraft.ih	\
			$(ODBENTO_INTDIR)/CMLkItr.ih	\
			$(ODBENTO_INTDIR)/CMLkSItr.ih	\
			$(ODBENTO_INTDIR)/CMSU.ih		\
			$(ODBENTO_INTDIR)/CMSURefI.ih	\
			$(ODBENTO_INTDIR)/EmbedCtr.ih	\
			$(ODBENTO_INTDIR)/FileCtr.ih	\
			$(ODBENTO_INTDIR)/MemCtr.ih		\
			$(ODBENTO_INTDIR)/UpdatCtr.ih

ODSTORAG_H=	$(ODSTORAG_INTDIR)/Document.h	\
			$(ODSTORAG_INTDIR)/Draft.h		\
			$(ODSTORAG_INTDIR)/ODCtr.h		\
			$(ODSTORAG_INTDIR)/ODStor.h		\
			$(ODSTORAG_INTDIR)/PfTLItr.h	\
			$(ODSTORAG_INTDIR)/PfTypLs.h	\
			$(ODSTORAG_INTDIR)/StorageU.h	\
			$(ODSTORAG_INTDIR)/SUCursor.h	\
			$(ODSTORAG_INTDIR)/SURefItr.h	\
			$(ODSTORAG_INTDIR)/SUView.h		\
			$(ODSTORAG_INTDIR)/TypeList.h	\
			$(ODSTORAG_INTDIR)/TypLsItr.h

ODSTORAG_IH=$(ODSTORAG_INTDIR)/Document.ih	\
			$(ODSTORAG_INTDIR)/Draft.ih		\
			$(ODSTORAG_INTDIR)/ODCtr.ih		\
			$(ODSTORAG_INTDIR)/ODStor.ih	\
			$(ODSTORAG_INTDIR)/PfTLItr.ih	\
			$(ODSTORAG_INTDIR)/PfTypLs.ih	\
			$(ODSTORAG_INTDIR)/StorageU.ih	\
			$(ODSTORAG_INTDIR)/SUCursor.ih	\
			$(ODSTORAG_INTDIR)/SURefItr.ih	\
			$(ODSTORAG_INTDIR)/SUView.ih	\
			$(ODSTORAG_INTDIR)/TypeList.ih	\
			$(ODSTORAG_INTDIR)/TypLsItr.ih

ODUI_H=		$(ODUI_INTDIR)/Arbitrat.h		\
			$(ODUI_INTDIR)/DispMod.h		\
			$(ODUI_INTDIR)/Disptch.h		\
			$(ODUI_INTDIR)/ExcFocus.h		\
			$(ODUI_INTDIR)/FocusItr.h		\
			$(ODUI_INTDIR)/FocusMod.h		\
			$(ODUI_INTDIR)/FocusOwn.h		\
			$(ODUI_INTDIR)/FocusSet.h		\
			$(ODUI_INTDIR)/MenuAdX.h		\
			$(ODUI_INTDIR)/MenuBar.h		\
			$(ODUI_INTDIR)/PltMenuX.h		\
			$(ODUI_INTDIR)/Popup.h			\
			$(ODUI_INTDIR)/StdDisp.h		\
			$(ODUI_INTDIR)/StdDispW.h		\
			$(ODUI_INTDIR)/StdDispX.h		\
			$(ODUI_INTDIR)/Undo.h			\
			$(ODUI_INTDIR)/Window.h			\
			$(ODUI_INTDIR)/WinIter.h		\
			$(ODUI_INTDIR)/WinStat.h

ODUI_IH=	$(ODUI_INTDIR)/Arbitrat.ih		\
			$(ODUI_INTDIR)/DispMod.ih		\
			$(ODUI_INTDIR)/Disptch.ih		\
			$(ODUI_INTDIR)/ExcFocus.ih		\
			$(ODUI_INTDIR)/FocusItr.ih		\
			$(ODUI_INTDIR)/FocusMod.ih		\
			$(ODUI_INTDIR)/FocusOwn.ih		\
			$(ODUI_INTDIR)/FocusSet.ih		\
			$(ODUI_INTDIR)/MenuAdX.ih		\
			$(ODUI_INTDIR)/MenuBar.ih		\
			$(ODUI_INTDIR)/PltMenuX.ih		\
			$(ODUI_INTDIR)/Popup.ih			\
			$(ODUI_INTDIR)/StdDisp.ih		\
			$(ODUI_INTDIR)/StdDispW.ih		\
			$(ODUI_INTDIR)/StdDispX.ih		\
			$(ODUI_INTDIR)/Undo.ih			\
			$(ODUI_INTDIR)/Window.ih		\
			$(ODUI_INTDIR)/WinIter.ih		\
			$(ODUI_INTDIR)/WinStat.ih

ODDATAX_H=	$(ODDATAX_INTDIR)/Clipbd.h		\
			$(ODDATAX_INTDIR)/DgItmIt.h		\
			$(ODDATAX_INTDIR)/DragDrp.h		\
			$(ODDATAX_INTDIR)/Link.h		\
			$(ODDATAX_INTDIR)/LinkB.h		\
			$(ODDATAX_INTDIR)/LinkItr.h		\
			$(ODDATAX_INTDIR)/LinkMgr.h		\
			$(ODDATAX_INTDIR)/LinkSpcB.h	\
			$(ODDATAX_INTDIR)/LinkSpec.h	\
			$(ODDATAX_INTDIR)/LinkSrc.h		\
			$(ODDATAX_INTDIR)/LinkSrcB.h	\
			$(ODDATAX_INTDIR)/LkSrcItr.h	\
			$(ODDATAX_INTDIR)/Translt.h

ODDATAX_IH=	$(ODDATAX_INTDIR)/Clipbd.ih		\
			$(ODDATAX_INTDIR)/DgItmIt.ih	\
			$(ODDATAX_INTDIR)/DragDrp.ih	\
			$(ODDATAX_INTDIR)/Link.ih		\
			$(ODDATAX_INTDIR)/LinkB.ih		\
			$(ODDATAX_INTDIR)/LinkItr.ih	\
			$(ODDATAX_INTDIR)/LinkMgr.ih	\
			$(ODDATAX_INTDIR)/LinkSpcB.ih	\
			$(ODDATAX_INTDIR)/LinkSpec.ih	\
			$(ODDATAX_INTDIR)/LinkSrc.ih	\
			$(ODDATAX_INTDIR)/LinkSrcB.ih	\
			$(ODDATAX_INTDIR)/LkSrcItr.ih	\
			$(ODDATAX_INTDIR)/Translt.ih

ODBINDNG_H=	$(ODBINDNG_INTDIR)/ODBindng.h	\
			$(ODBINDNG_INTDIR)/PartWrap.h	\
			$(ODBINDNG_INTDIR)/NoPart.h

ODBINDNG_IH=$(ODBINDNG_INTDIR)/ODBindng.ih	\
			$(ODBINDNG_INTDIR)/PartWrap.ih	\
			$(ODBINDNG_INTDIR)/NoPart.ih

ODLAYOUT_H=	$(ODLAYOUT_INTDIR)/Facet.h		\
			$(ODLAYOUT_INTDIR)/FacetItr.h	\
			$(ODLAYOUT_INTDIR)/Frame.h		\
			$(ODLAYOUT_INTDIR)/FrFaItr.h

ODLAYOUT_IH=$(ODLAYOUT_INTDIR)/Facet.ih		\
			$(ODLAYOUT_INTDIR)/FacetItr.ih	\
			$(ODLAYOUT_INTDIR)/Frame.ih		\
			$(ODLAYOUT_INTDIR)/FrFaItr.ih

ODIMAGNG_H=	$(ODIMAGNG_INTDIR)/AIXPCanv.h	\
			$(ODIMAGNG_INTDIR)/AIXSCanv.h	\
			$(ODIMAGNG_INTDIR)/AIXWCanv.h	\
			$(ODIMAGNG_INTDIR)/Canvas.h		\
			$(ODIMAGNG_INTDIR)/CanvasB.h	\
			$(ODIMAGNG_INTDIR)/PlatCanv.h	\
			$(ODIMAGNG_INTDIR)/Shape.h		\
			$(ODIMAGNG_INTDIR)/ShapeB.h		\
			$(ODIMAGNG_INTDIR)/Trnsform.h	\
			$(ODIMAGNG_INTDIR)/TrnsfrmB.h

ODIMAGNG_IH=$(ODIMAGNG_INTDIR)/AIXPCanv.ih	\
			$(ODIMAGNG_INTDIR)/AIXSCanv.ih	\
			$(ODIMAGNG_INTDIR)/AIXWCanv.ih	\
			$(ODIMAGNG_INTDIR)/Canvas.ih	\
			$(ODIMAGNG_INTDIR)/CanvasB.ih	\
			$(ODIMAGNG_INTDIR)/PlatCanv.ih	\
			$(ODIMAGNG_INTDIR)/Shape.ih		\
			$(ODIMAGNG_INTDIR)/ShapeB.ih	\
			$(ODIMAGNG_INTDIR)/Trnsform.ih	\
			$(ODIMAGNG_INTDIR)/TrnsfrmB.ih

ODREGSTY_H=									\
			$(ODREGSTY_INTDIR)/ODPrtReg.h	\
			$(ODREGSTY_INTDIR)/ODPrtRei.h	\
			$(ODREGSTY_INTDIR)/PartCat.h	\
			$(ODREGSTY_INTDIR)/PartInfi.h	\
			$(ODREGSTY_INTDIR)/PartInfo.h	\
			$(ODREGSTY_INTDIR)/PartKind.h	\
			$(ODREGSTY_INTDIR)/RManager.h

ODREGSTY_IH=								\
			$(ODREGSTY_INTDIR)/ODPrtReg.ih	\
			$(ODREGSTY_INTDIR)/ODPrtRei.ih	\
			$(ODREGSTY_INTDIR)/PartCat.ih	\
			$(ODREGSTY_INTDIR)/PartInfi.ih	\
			$(ODREGSTY_INTDIR)/PartInfo.ih	\
			$(ODREGSTY_INTDIR)/PartKind.ih	\
			$(ODREGSTY_INTDIR)/RManager.ih

ODSHELL_H=									\
			$(ODSHELL_INTDIR)/AIXShell.h	\
			$(ODSHELL_INTDIR)/AppShell.h	\
			$(ODSHELL_INTDIR)/DocMgr.h		\
			$(ODSHELL_INTDIR)/DraftPage.h	\
			$(ODSHELL_INTDIR)/ODShell.h

ODSHELL_IH=									\
			$(ODSHELL_INTDIR)/AIXShell.ih	\
			$(ODSHELL_INTDIR)/AppShell.ih	\
			$(ODSHELL_INTDIR)/DocMgr.ih		\
			$(ODSHELL_INTDIR)/DraftPage.ih	\
			$(ODSHELL_INTDIR)/ODShell.ih

DIRS=		$(ODCORE_INTDIR)	\
			$(ODBENTO_INTDIR)	\
			$(ODUI_INTDIR)		\
			$(ODSTORAG_INTDIR)	\
			$(ODDATAX_INTDIR)	\
			$(ODBINDNG_INTDIR)	\
			$(ODIMAGNG_INTDIR)	\
			$(ODLAYOUT_INTDIR)	\
			$(ODREGSTY_INTDIR)	\
			$(ODSHELL_INTDIR)

all: all-$(HAVE_LIBX11)

all-false:

all-true:	$(DIRS)				\
			$(ODCORE_H)			\
			$(ODCORE_IH)		\
			$(EMIT_TYPES)		\
			$(EMIT_CONSTANTS)	\
			$(EMIT_POLYGON)		\
			$(ODBENTO_H)		\
			$(ODBENTO_IH)		\
			$(ODSTORAG_IH)		\
			$(ODSTORAG_H)		\
			$(ODUI_H)			\
			$(ODUI_IH)			\
			$(ODDATAX_H)		\
			$(ODDATAX_IH)		\
			$(ODBINDNG_H)		\
			$(ODBINDNG_IH)		\
			$(ODIMAGNG_IH)		\
			$(ODIMAGNG_H)		\
			$(ODLAYOUT_IH)		\
			$(ODLAYOUT_H)		\
			$(ODSHELL_H)		\
			$(ODSHELL_IH)		\
			$(ODREGSTY_H)		\
			$(ODREGSTY_IH)

clean:	
	$(CLEAN)	$(ODCORE_H)
	$(CLEAN)	$(ODCORE_IH)
	$(CLEAN)	$(EMIT_TYPES)
	$(CLEAN)	$(EMIT_CONSTANTS)
	$(CLEAN)	$(EMIT_POLYGON)
	$(CLEAN)	$(ODBENTO_H)
	$(CLEAN)	$(ODBENTO_IH)
	$(CLEAN)	$(ODSTORAG_IH)
	$(CLEAN)	$(ODSTORAG_H)
	$(CLEAN)	$(ODUI_IH)
	$(CLEAN)	$(ODUI_H)
	$(CLEAN)	$(ODDATAX_H)
	$(CLEAN)	$(ODDATAX_IH)
	$(CLEAN)	$(ODBINDNG_H)
	$(CLEAN)	$(ODBINDNG_IH)
	$(CLEAN)	$(ODIMAGNG_IH)
	$(CLEAN)	$(ODIMAGNG_H)
	$(CLEAN)	$(ODLAYOUT_IH)
	$(CLEAN)	$(ODLAYOUT_H)
	$(CLEAN)	$(ODSHELL_H)
	$(CLEAN)	$(ODSHELL_IH)
	$(CLEAN)	$(ODREGSTY_H)
	$(CLEAN)	$(ODREGSTY_IH)

$(ODCORE_H) $(ODCORE_IH):
	$(IDLTOOL) $(SC) $(ODCORE_DIR) -o $@ -p -I$(ODCORE_DIR) $(PLATFORM_SCFLAGS) -I$(SOMIDL_IDL) -I$(SOMTK_IDL)

$(EMIT_CONSTANTS):
	$(IDLTOOL) $(SC) $(ODCORE_DIR) \
					 $(ODUI_DIR) -o $@ -p -I$(ODCORE_DIR) $(PLATFORM_SCFLAGS) -DEMIT_GLOBAL_CONSTANTS -I$(SOMIDL_IDL) -I$(SOMTK_IDL)

$(EMIT_TYPES):
	$(IDLTOOL) $(SC) $(ODCORE_DIR) -o $@ -p -I$(ODCORE_DIR) $(PLATFORM_SCFLAGS) -DEMIT_GLOBAL_TYPES -I$(SOMIDL_IDL) -I$(SOMTK_IDL)

$(EMIT_POLYGON):
	$(IDLTOOL) $(SC) $(ODCORE_DIR) -o $@ -p -I$(ODCORE_DIR) $(PLATFORM_SCFLAGS) -DEMIT_POLYGON -I$(SOMIDL_IDL) -I$(SOMTK_IDL)

$(ODBENTO_H) $(ODBENTO_IH):
	$(IDLTOOL) $(SC) $(ODBENTO_DIR) -o $@ -p -I$(ODBENTO_DIR) -I$(ODSTORAG_DIR) $(PLATFORM_SCFLAGS) -I$(SOMIDL_IDL) -I$(SOMTK_IDL)

$(ODSTORAG_H) $(ODSTORAG_IH):
	$(IDLTOOL) $(SC) $(ODSTORAG_DIR) -o $@ -p -I$(ODSTORAG_DIR) $(PLATFORM_SCFLAGS) -I$(SOMIDL_IDL) -I$(SOMTK_IDL)

$(ODUI_H) $(ODUI_IH):
	$(IDLTOOL) $(SC) $(ODUI_DIR) -o $@ -p -I$(ODUI_DIR) $(PLATFORM_SCFLAGS) -I$(SOMIDL_IDL) -I$(SOMTK_IDL)

$(ODDATAX_H) $(ODDATAX_IH):
	$(IDLTOOL) $(SC) $(ODDATAX_DIR) -o $@ -p -I$(ODDATAX_DIR) $(PLATFORM_SCFLAGS) -I$(SOMIDL_IDL) -I$(SOMTK_IDL)

$(ODBINDNG_H) $(ODBINDNG_IH):
	$(IDLTOOL) $(SC) $(ODBINDNG_DIR) -o $@ -p -I$(ODBINDNG_DIR) $(PLATFORM_SCFLAGS) -I$(SOMIDL_IDL) -I$(SOMTK_IDL)

$(ODIMAGNG_H) $(ODIMAGNG_IH):
	$(IDLTOOL) $(SC) $(ODIMAGNG_DIR) -o $@ -p -I$(ODIMAGNG_DIR) $(PLATFORM_SCFLAGS) -I$(SOMIDL_IDL) -I$(SOMTK_IDL)

$(ODLAYOUT_H) $(ODLAYOUT_IH):
	$(IDLTOOL) $(SC) $(ODLAYOUT_DIR) -o $@ -p -I$(ODLAYOUT_DIR) $(PLATFORM_SCFLAGS) -I$(SOMIDL_IDL) -I$(SOMTK_IDL)

$(ODREGSTY_H) $(ODREGSTY_IH):
	$(IDLTOOL) $(SC) $(ODREGSTY_DIR) -o $@ -p -I$(ODREGSTY_DIR) $(PLATFORM_SCFLAGS) -I$(SOMIDL_IDL) -I$(SOMTK_IDL)

$(ODSHELL_H) $(ODSHELL_IH):
	$(IDLTOOL) $(SC) $(ODSHELL_DIR) -o $@ -p -I$(ODSHELL_DIR) $(PLATFORM_SCFLAGS) -I$(SOMIDL_IDL) -I$(SOMTK_IDL)

$(DIRS):
	while test ! -d $@; 			\
	do NAME=$@; 					\
		while test ! -d $$NAME; 	\
		do PARENT=`dirname $$NAME`; \
			if test -d $$PARENT;	\
			then					\
				mkdir $$NAME;		\
				if test "$$?" != 0;	\
				then exit 1; 		\
				fi;					\
			else					\
				NAME=$$PARENT; 		\
			fi; 					\
		done; 						\
	done

dist install:

