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
ODSTORAG_DIR=../../odstorag
ODDATAX_DIR=../../oddatax
ODUI_DIR=../../odui
ODLAYOUT_DIR=../../odlayout
ODIMAGNG_DIR=../../odimagng
ODBINDNG_DIR=../../odbindng
ODREGSTY_DIR=../../odregsty
ODSHELL_DIR=../../odshell
ODSOMUC_DIR=../../odsomuc

ODCORE_IDL=	$(SOMIDL_IDL)/BindPage.idl		\
			$(SOMIDL_IDL)/CmdDefs.idl		\
			$(SOMIDL_IDL)/DocPage.idl		\
			$(SOMIDL_IDL)/EmbFrItr.idl		\
			$(SOMIDL_IDL)/ErrorDef.idl		\
			$(SOMIDL_IDL)/Extensn.idl		\
			$(SOMIDL_IDL)/GeoTypes.idl		\
			$(SOMIDL_IDL)/Info.idl			\
			$(SOMIDL_IDL)/NamSpac.idl		\
			$(SOMIDL_IDL)/NmSpcMg.idl		\
			$(SOMIDL_IDL)/Notebook.idl		\
			$(SOMIDL_IDL)/ObjctItr.idl		\
			$(SOMIDL_IDL)/ObjectNS.idl		\
			$(SOMIDL_IDL)/odhelp.idl		\
			$(SOMIDL_IDL)/ODObject.idl		\
			$(SOMIDL_IDL)/ODRgstry.idl		\
			$(SOMIDL_IDL)/ODSessn.idl		\
			$(SOMIDL_IDL)/ODSessnB.idl		\
			$(SOMIDL_IDL)/ODTypesB.idl		\
			$(SOMIDL_IDL)/ODTypesM.idl		\
			$(SOMIDL_IDL)/Page.idl			\
			$(SOMIDL_IDL)/Part.idl			\
			$(SOMIDL_IDL)/Polygon.idl		\
			$(SOMIDL_IDL)/PresPage.idl		\
			$(SOMIDL_IDL)/PstObj.idl		\
			$(SOMIDL_IDL)/RefCtObj.idl		\
			$(SOMIDL_IDL)/Settings.idl		\
			$(SOMIDL_IDL)/StdDefs.idl		\
			$(SOMIDL_IDL)/StdExts.idl		\
			$(SOMIDL_IDL)/StdProps.idl		\
			$(SOMIDL_IDL)/StdTypes.idl		\
			$(SOMIDL_IDL)/ValueItr.idl		\
			$(SOMIDL_IDL)/ValueNS.idl		

ODSTORAG_IDL=								\
			$(SOMIDL_IDL)/Document.idl		\
			$(SOMIDL_IDL)/Draft.idl			\
			$(SOMIDL_IDL)/ODCtr.idl			\
			$(SOMIDL_IDL)/ODStor.idl		\
			$(SOMIDL_IDL)/PfTLItr.idl		\
			$(SOMIDL_IDL)/PfTypLs.idl		\
			$(SOMIDL_IDL)/StorageU.idl		\
			$(SOMIDL_IDL)/SUCursor.idl		\
			$(SOMIDL_IDL)/SURefItr.idl		\
			$(SOMIDL_IDL)/SUView.idl		\
			$(SOMIDL_IDL)/TypeList.idl		\
			$(SOMIDL_IDL)/TypLsItr.idl

ODDATAX_IDL=$(SOMIDL_IDL)/Clipbd.idl		\
			$(SOMIDL_IDL)/DgItmIt.idl		\
			$(SOMIDL_IDL)/DragDrp.idl		\
			$(SOMIDL_IDL)/Link.idl			\
			$(SOMIDL_IDL)/LinkB.idl			\
			$(SOMIDL_IDL)/LinkItr.idl		\
			$(SOMIDL_IDL)/LinkMgr.idl		\
			$(SOMIDL_IDL)/LinkSpcB.idl		\
			$(SOMIDL_IDL)/LinkSpec.idl		\
			$(SOMIDL_IDL)/LinkSrc.idl		\
			$(SOMIDL_IDL)/LinkSrcB.idl		\
			$(SOMIDL_IDL)/LkSrcItr.idl		\
			$(SOMIDL_IDL)/Translt.idl

ODUI_IDL=	$(SOMIDL_IDL)/Arbitrat.idl		\
			$(SOMIDL_IDL)/DispMod.idl		\
			$(SOMIDL_IDL)/Disptch.idl		\
			$(SOMIDL_IDL)/ExcFocus.idl		\
			$(SOMIDL_IDL)/Foci.idl			\
			$(SOMIDL_IDL)/FocusItr.idl		\
			$(SOMIDL_IDL)/FocusMod.idl		\
			$(SOMIDL_IDL)/FocusOwn.idl		\
			$(SOMIDL_IDL)/FocusSet.idl		\
			$(SOMIDL_IDL)/MenuAdX.idl		\
			$(SOMIDL_IDL)/MenuBar.idl		\
			$(SOMIDL_IDL)/PltMenuX.idl		\
			$(SOMIDL_IDL)/Popup.idl			\
			$(SOMIDL_IDL)/StdDisp.idl		\
			$(SOMIDL_IDL)/StdDispX.idl		\
			$(SOMIDL_IDL)/Undo.idl			\
			$(SOMIDL_IDL)/Window.idl		\
			$(SOMIDL_IDL)/WinIter.idl		\
			$(SOMIDL_IDL)/WinStat.idl

ODIMAGNG_IDL=								\
			$(SOMIDL_IDL)/AIXPCanv.idl		\
			$(SOMIDL_IDL)/AIXSCanv.idl		\
			$(SOMIDL_IDL)/AIXWCanv.idl		\
			$(SOMIDL_IDL)/Canvas.idl		\
			$(SOMIDL_IDL)/CanvasB.idl		\
			$(SOMIDL_IDL)/PlatCanv.idl		\
			$(SOMIDL_IDL)/Shape.idl			\
			$(SOMIDL_IDL)/ShapeB.idl		\
			$(SOMIDL_IDL)/Trnsform.idl		\
			$(SOMIDL_IDL)/TrnsfrmB.idl

ODLAYOUT_IDL=								\
			$(SOMIDL_IDL)/Facet.idl			\
			$(SOMIDL_IDL)/FacetItr.idl		\
			$(SOMIDL_IDL)/Frame.idl			\
			$(SOMIDL_IDL)/FrFaItr.idl

ODBINDNG_IDL=								\
			$(SOMIDL_IDL)/ODBindng.idl		\
			$(SOMIDL_IDL)/PartWrap.idl		\
			$(SOMIDL_IDL)/NoPart.idl

ODSHELL_IDL=								\
			$(SOMIDL_IDL)/AIXShell.idl		\
			$(SOMIDL_IDL)/AppShell.idl		\
			$(SOMIDL_IDL)/DocMgr.idl		\
			$(SOMIDL_IDL)/DraftPage.idl		\
			$(SOMIDL_IDL)/ODShell.idl

ODREGSTY_IDL=								\
			$(SOMIDL_IDL)/ODPrtReg.idl		\
			$(SOMIDL_IDL)/ODPrtRei.idl		\
			$(SOMIDL_IDL)/PartCat.idl		\
			$(SOMIDL_IDL)/PartInfi.idl		\
			$(SOMIDL_IDL)/PartInfo.idl		\
			$(SOMIDL_IDL)/PartKind.idl		\
			$(SOMIDL_IDL)/RManager.idl

EMIT_CONSTANTS=								\
			$(SOMIDL_HEADERS)/StdDefs.xh	\
			$(SOMIDL_HEADERS)/StdExts.xh	\
			$(SOMIDL_HEADERS)/StdProps.xh	\
			$(SOMIDL_HEADERS)/StdTypes.xh	\
			$(SOMIDL_HEADERS)/CmdDefs.xh	\
			$(SOMIDL_HEADERS)/StdDefs.h		\
			$(SOMIDL_HEADERS)/StdExts.h		\
			$(SOMIDL_HEADERS)/StdProps.h	\
			$(SOMIDL_HEADERS)/StdTypes.h	\
			$(SOMIDL_HEADERS)/ErrorDef.h	\
			$(SOMIDL_HEADERS)/ErrorDef.xh	\
			$(SOMIDL_HEADERS)/Foci.h		\
			$(SOMIDL_HEADERS)/Foci.xh		\
			$(SOMIDL_HEADERS)/CmdDefs.h

EMIT_TYPES=									\
			$(SOMIDL_HEADERS)/ODTypesB.h	\
			$(SOMIDL_HEADERS)/ODTypesM.h	\
			$(SOMIDL_HEADERS)/ODTypesB.xh	\
			$(SOMIDL_HEADERS)/ODTypesM.xh	\
			$(SOMIDL_HEADERS)/GeoTypes.h	\
			$(SOMIDL_HEADERS)/GeoTypes.xh

EMIT_POLYGON=								\
			$(SOMIDL_HEADERS)/Polygon.h		\
			$(SOMIDL_HEADERS)/Polygon.xh

ODCORE_H=	$(SOMIDL_HEADERS)/BindPage.h	\
			$(SOMIDL_HEADERS)/DocPage.h		\
			$(SOMIDL_HEADERS)/EmbFrItr.h	\
			$(SOMIDL_HEADERS)/Extensn.h		\
			$(SOMIDL_HEADERS)/Info.h		\
			$(SOMIDL_HEADERS)/NamSpac.h		\
			$(SOMIDL_HEADERS)/NmSpcMg.h		\
			$(SOMIDL_HEADERS)/Notebook.h	\
			$(SOMIDL_HEADERS)/ObjctItr.h	\
			$(SOMIDL_HEADERS)/ObjectNS.h	\
			$(SOMIDL_HEADERS)/odhelp.h		\
			$(SOMIDL_HEADERS)/ODObject.h	\
			$(SOMIDL_HEADERS)/ODRgstry.h	\
			$(SOMIDL_HEADERS)/ODSessn.h		\
			$(SOMIDL_HEADERS)/ODSessnB.h	\
			$(SOMIDL_HEADERS)/Page.h		\
			$(SOMIDL_HEADERS)/Part.h		\
			$(SOMIDL_HEADERS)/PresPage.h	\
			$(SOMIDL_HEADERS)/PstObj.h		\
			$(SOMIDL_HEADERS)/RefCtObj.h	\
			$(SOMIDL_HEADERS)/Settings.h	\
			$(SOMIDL_HEADERS)/ValueItr.h	\
			$(SOMIDL_HEADERS)/ValueNS.h		

ODCORE_XH=	$(SOMIDL_HEADERS)/BindPage.xh	\
			$(SOMIDL_HEADERS)/DocPage.xh	\
			$(SOMIDL_HEADERS)/EmbFrItr.xh	\
			$(SOMIDL_HEADERS)/Extensn.xh	\
			$(SOMIDL_HEADERS)/Info.xh		\
			$(SOMIDL_HEADERS)/NamSpac.xh	\
			$(SOMIDL_HEADERS)/NmSpcMg.xh	\
			$(SOMIDL_HEADERS)/Notebook.xh	\
			$(SOMIDL_HEADERS)/ObjctItr.xh	\
			$(SOMIDL_HEADERS)/ObjectNS.xh	\
			$(SOMIDL_HEADERS)/odhelp.xh		\
			$(SOMIDL_HEADERS)/ODObject.xh	\
			$(SOMIDL_HEADERS)/ODRgstry.xh	\
			$(SOMIDL_HEADERS)/ODSessn.xh	\
			$(SOMIDL_HEADERS)/ODSessnB.xh	\
			$(SOMIDL_HEADERS)/Page.xh		\
			$(SOMIDL_HEADERS)/Part.xh		\
			$(SOMIDL_HEADERS)/PresPage.xh	\
			$(SOMIDL_HEADERS)/PstObj.xh		\
			$(SOMIDL_HEADERS)/RefCtObj.xh	\
			$(SOMIDL_HEADERS)/Settings.xh	\
			$(SOMIDL_HEADERS)/ValueItr.xh	\
			$(SOMIDL_HEADERS)/ValueNS.xh		

ODSTORAG_H=	$(SOMIDL_HEADERS)/Document.h	\
			$(SOMIDL_HEADERS)/Draft.h		\
			$(SOMIDL_HEADERS)/ODCtr.h		\
			$(SOMIDL_HEADERS)/ODStor.h		\
			$(SOMIDL_HEADERS)/PfTLItr.h		\
			$(SOMIDL_HEADERS)/PfTypLs.h		\
			$(SOMIDL_HEADERS)/StorageU.h	\
			$(SOMIDL_HEADERS)/SUCursor.h	\
			$(SOMIDL_HEADERS)/SURefItr.h	\
			$(SOMIDL_HEADERS)/SUView.h		\
			$(SOMIDL_HEADERS)/TypeList.h	\
			$(SOMIDL_HEADERS)/TypLsItr.h

ODSTORAG_XH=$(SOMIDL_HEADERS)/Document.xh	\
			$(SOMIDL_HEADERS)/Draft.xh		\
			$(SOMIDL_HEADERS)/ODCtr.xh		\
			$(SOMIDL_HEADERS)/ODStor.xh		\
			$(SOMIDL_HEADERS)/PfTLItr.xh	\
			$(SOMIDL_HEADERS)/PfTypLs.xh	\
			$(SOMIDL_HEADERS)/StorageU.xh	\
			$(SOMIDL_HEADERS)/SUCursor.xh	\
			$(SOMIDL_HEADERS)/SURefItr.xh	\
			$(SOMIDL_HEADERS)/SUView.xh		\
			$(SOMIDL_HEADERS)/TypeList.xh	\
			$(SOMIDL_HEADERS)/TypLsItr.xh

ODDATAX_H=	$(SOMIDL_HEADERS)/Clipbd.h		\
			$(SOMIDL_HEADERS)/DgItmIt.h		\
			$(SOMIDL_HEADERS)/DragDrp.h		\
			$(SOMIDL_HEADERS)/Link.h		\
			$(SOMIDL_HEADERS)/LinkB.h		\
			$(SOMIDL_HEADERS)/LinkItr.h		\
			$(SOMIDL_HEADERS)/LinkMgr.h		\
			$(SOMIDL_HEADERS)/LinkSpcB.h	\
			$(SOMIDL_HEADERS)/LinkSpec.h	\
			$(SOMIDL_HEADERS)/LinkSrc.h		\
			$(SOMIDL_HEADERS)/LinkSrcB.h	\
			$(SOMIDL_HEADERS)/LkSrcItr.h	\
			$(SOMIDL_HEADERS)/Translt.h

ODDATAX_XH=	$(SOMIDL_HEADERS)/Clipbd.xh		\
			$(SOMIDL_HEADERS)/DgItmIt.xh	\
			$(SOMIDL_HEADERS)/DragDrp.xh	\
			$(SOMIDL_HEADERS)/Link.xh		\
			$(SOMIDL_HEADERS)/LinkB.xh		\
			$(SOMIDL_HEADERS)/LinkItr.xh	\
			$(SOMIDL_HEADERS)/LinkMgr.xh	\
			$(SOMIDL_HEADERS)/LinkSpcB.xh	\
			$(SOMIDL_HEADERS)/LinkSpec.xh	\
			$(SOMIDL_HEADERS)/LinkSrc.xh	\
			$(SOMIDL_HEADERS)/LinkSrcB.xh	\
			$(SOMIDL_HEADERS)/LkSrcItr.xh	\
			$(SOMIDL_HEADERS)/Translt.xh

ODUI_H=		$(SOMIDL_HEADERS)/Arbitrat.h	\
			$(SOMIDL_HEADERS)/DispMod.h		\
			$(SOMIDL_HEADERS)/Disptch.h		\
			$(SOMIDL_HEADERS)/ExcFocus.h	\
			$(SOMIDL_HEADERS)/FocusItr.h	\
			$(SOMIDL_HEADERS)/FocusMod.h	\
			$(SOMIDL_HEADERS)/FocusOwn.h	\
			$(SOMIDL_HEADERS)/FocusSet.h	\
			$(SOMIDL_HEADERS)/MenuAdX.h		\
			$(SOMIDL_HEADERS)/MenuBar.h		\
			$(SOMIDL_HEADERS)/PltMenuX.h	\
			$(SOMIDL_HEADERS)/Popup.h		\
			$(SOMIDL_HEADERS)/StdDisp.h		\
			$(SOMIDL_HEADERS)/StdDispX.h	\
			$(SOMIDL_HEADERS)/Undo.h		\
			$(SOMIDL_HEADERS)/Window.h		\
			$(SOMIDL_HEADERS)/WinIter.h		\
			$(SOMIDL_HEADERS)/WinStat.h

ODIMAGNG_H=									\
			$(SOMIDL_HEADERS)/AIXPCanv.h	\
			$(SOMIDL_HEADERS)/AIXSCanv.h	\
			$(SOMIDL_HEADERS)/AIXWCanv.h	\
			$(SOMIDL_HEADERS)/Canvas.h		\
			$(SOMIDL_HEADERS)/CanvasB.h		\
			$(SOMIDL_HEADERS)/PlatCanv.h	\
			$(SOMIDL_HEADERS)/Shape.h		\
			$(SOMIDL_HEADERS)/ShapeB.h		\
			$(SOMIDL_HEADERS)/Trnsform.h	\
			$(SOMIDL_HEADERS)/TrnsfrmB.h

ODLAYOUT_H=									\
			$(SOMIDL_HEADERS)/Facet.h		\
			$(SOMIDL_HEADERS)/FacetItr.h	\
			$(SOMIDL_HEADERS)/Frame.h		\
			$(SOMIDL_HEADERS)/FrFaItr.h

ODUI_XH=	$(SOMIDL_HEADERS)/Arbitrat.xh	\
			$(SOMIDL_HEADERS)/DispMod.xh	\
			$(SOMIDL_HEADERS)/Disptch.xh	\
			$(SOMIDL_HEADERS)/ExcFocus.xh	\
			$(SOMIDL_HEADERS)/FocusItr.xh	\
			$(SOMIDL_HEADERS)/FocusMod.xh	\
			$(SOMIDL_HEADERS)/FocusOwn.xh	\
			$(SOMIDL_HEADERS)/FocusSet.xh	\
			$(SOMIDL_HEADERS)/MenuAdX.xh	\
			$(SOMIDL_HEADERS)/MenuBar.xh	\
			$(SOMIDL_HEADERS)/PltMenuX.xh	\
			$(SOMIDL_HEADERS)/Popup.xh		\
			$(SOMIDL_HEADERS)/StdDisp.xh	\
			$(SOMIDL_HEADERS)/StdDispX.xh	\
			$(SOMIDL_HEADERS)/Undo.xh		\
			$(SOMIDL_HEADERS)/Window.xh		\
			$(SOMIDL_HEADERS)/WinIter.xh	\
			$(SOMIDL_HEADERS)/WinStat.xh

ODIMAGNG_XH=								\
			$(SOMIDL_HEADERS)/AIXPCanv.xh	\
			$(SOMIDL_HEADERS)/AIXSCanv.xh	\
			$(SOMIDL_HEADERS)/AIXWCanv.xh	\
			$(SOMIDL_HEADERS)/Canvas.xh		\
			$(SOMIDL_HEADERS)/CanvasB.xh	\
			$(SOMIDL_HEADERS)/PlatCanv.xh	\
			$(SOMIDL_HEADERS)/Shape.xh		\
			$(SOMIDL_HEADERS)/ShapeB.xh		\
			$(SOMIDL_HEADERS)/Trnsform.xh	\
			$(SOMIDL_HEADERS)/TrnsfrmB.xh

ODLAYOUT_XH=								\
			$(SOMIDL_HEADERS)/Facet.xh		\
			$(SOMIDL_HEADERS)/FacetItr.xh	\
			$(SOMIDL_HEADERS)/Frame.xh		\
			$(SOMIDL_HEADERS)/FrFaItr.xh

ODBINDNG_XH=								\
			$(SOMIDL_HEADERS)/ODBindng.xh	\
			$(SOMIDL_HEADERS)/PartWrap.xh	\
			$(SOMIDL_HEADERS)/NoPart.xh

ODBINDNG_H=	$(SOMIDL_HEADERS)/ODBindng.h	\
			$(SOMIDL_HEADERS)/PartWrap.h	\
			$(SOMIDL_HEADERS)/NoPart.h

ODSHELL_H=									\
			$(SOMIDL_HEADERS)/AIXShell.h	\
			$(SOMIDL_HEADERS)/AppShell.h	\
			$(SOMIDL_HEADERS)/DocMgr.h		\
			$(SOMIDL_HEADERS)/DraftPage.h	\
			$(SOMIDL_HEADERS)/ODShell.h

ODSHELL_XH=									\
			$(SOMIDL_HEADERS)/AIXShell.xh	\
			$(SOMIDL_HEADERS)/AppShell.xh	\
			$(SOMIDL_HEADERS)/DocMgr.xh		\
			$(SOMIDL_HEADERS)/DraftPage.xh	\
			$(SOMIDL_HEADERS)/ODShell.xh

ODREGSTY_H=									\
			$(SOMIDL_HEADERS)/ODPrtReg.h	\
			$(SOMIDL_HEADERS)/ODPrtRei.h	\
			$(SOMIDL_HEADERS)/PartCat.h		\
			$(SOMIDL_HEADERS)/PartInfi.h	\
			$(SOMIDL_HEADERS)/PartInfo.h	\
			$(SOMIDL_HEADERS)/PartKind.h	\
			$(SOMIDL_HEADERS)/RManager.h

ODREGSTY_XH=								\
			$(SOMIDL_HEADERS)/ODPrtReg.xh	\
			$(SOMIDL_HEADERS)/ODPrtRei.xh	\
			$(SOMIDL_HEADERS)/PartCat.xh	\
			$(SOMIDL_HEADERS)/PartInfi.xh	\
			$(SOMIDL_HEADERS)/PartInfo.xh	\
			$(SOMIDL_HEADERS)/PartKind.xh	\
			$(SOMIDL_HEADERS)/RManager.xh

all: all-$(HAVE_LIBX11)

all-false:

all-true:	$(ODCORE_IDL)		\
			$(ODBINDNG_IDL)		\
			$(ODSTORAG_IDL)		\
			$(ODLAYOUT_IDL)		\
			$(ODIMAGNG_IDL)		\
			$(ODDATAX_IDL)		\
			$(ODSHELL_IDL)		\
			$(ODREGSTY_IDL)		\
			$(ODUI_IDL)			\
			$(ODCORE_H)			\
			$(ODCORE_XH)		\
			$(EMIT_CONSTANTS)	\
			$(EMIT_TYPES)		\
			$(EMIT_POLYGON)		\
			$(ODSTORAG_H)		\
			$(ODSTORAG_XH)		\
			$(ODBINDNG_XH)		\
			$(ODBINDNG_H)		\
			$(ODDATAX_H)		\
			$(ODDATAX_XH)		\
			$(ODUI_XH)			\
			$(ODUI_H)			\
			$(ODLAYOUT_XH)		\
			$(ODLAYOUT_H)		\
			$(ODIMAGNG_XH)		\
			$(ODIMAGNG_H)		\
			$(ODSHELL_H)		\
			$(ODSHELL_XH)		\
			$(ODREGSTY_H)		\
			$(ODREGSTY_XH)

clean:	
	$(CLEAN)	$(ODCORE_IDL)
	$(CLEAN)	$(ODSTORAG_IDL)
	$(CLEAN)	$(EMIT_CONSTANTS)
	$(CLEAN)	$(EMIT_TYPES)
	$(CLEAN)	$(EMIT_POLYGON)
	$(CLEAN)	$(ODCORE_H)
	$(CLEAN)	$(ODCORE_XH)
	$(CLEAN)	$(ODSTORAG_H)
	$(CLEAN)	$(ODSTORAG_XH)
	$(CLEAN)	$(ODDATAX_IDL)
	$(CLEAN)	$(ODDATAX_H)
	$(CLEAN)	$(ODDATAX_XH)
	$(CLEAN)	$(ODBINDNG_IDL)
	$(CLEAN)	$(ODBINDNG_H)
	$(CLEAN)	$(ODBINDNG_XH)
	$(CLEAN)	$(ODSHELL_IDL)
	$(CLEAN)	$(ODSHELL_H)
	$(CLEAN)	$(ODSHELL_XH)
	$(CLEAN)	$(ODREGSTY_IDL)
	$(CLEAN)	$(ODREGSTY_H)
	$(CLEAN)	$(ODREGSTY_XH)
	$(CLEAN)	$(ODIMAGNG_IDL)
	$(CLEAN)	$(ODIMAGNG_H)
	$(CLEAN)	$(ODIMAGNG_XH)
	$(CLEAN)	$(ODLAYOUT_IDL)
	$(CLEAN)	$(ODLAYOUT_H)
	$(CLEAN)	$(ODLAYOUT_XH)
	$(CLEAN)	$(ODUI_IDL)
	$(CLEAN)	$(ODUI_H)
	$(CLEAN)	$(ODUI_XH)

$(ODCORE_IDL):
	$(IDLTOOL) $(PDL) $(ODCORE_DIR) -o $@

$(ODCORE_H)			\
	$(ODCORE_XH)	\
	$(ODDATAX_H)	\
	$(ODDATAX_XH)	\
	$(ODUI_H)		\
	$(ODUI_XH)		\
	$(ODIMAGNG_H)	\
	$(ODIMAGNG_XH)	\
	$(ODLAYOUT_H)	\
	$(ODLAYOUT_XH)	\
	$(ODBINDNG_H)	\
	$(ODBINDNG_XH)	\
	$(ODSTORAG_H)	\
	$(ODREGSTY_H)	\
	$(ODREGSTY_XH)	\
	$(ODSHELL_H)	\
	$(ODSHELL_XH)	\
	$(ODSTORAG_XH):
		$(IDLTOOL) $(SC) $(SOMIDL_IDL) -o $@ $(PLATFORM_SCFLAGS) -I$(SOMIDL_IDL) -I$(SOMTK_IDL)

$(EMIT_CONSTANTS):
	$(IDLTOOL) $(SC) $(SOMIDL_IDL) -o $@ $(PLATFORM_SCFLAGS) -DEMIT_GLOBAL_CONSTANTS -I$(SOMIDL_IDL) -I$(SOMTK_IDL)

$(EMIT_TYPES):
	$(IDLTOOL) $(SC) $(SOMIDL_IDL) -o $@ $(PLATFORM_SCFLAGS) -DEMIT_GLOBAL_TYPES -I$(SOMIDL_IDL) -I$(SOMTK_IDL)

$(EMIT_POLYGON):
	$(IDLTOOL) $(SC) $(SOMIDL_IDL) -o $@ $(PLATFORM_SCFLAGS) -DEMIT_POLYGON -I$(SOMIDL_IDL) -I$(SOMTK_IDL)

$(ODSTORAG_IDL):
	$(IDLTOOL) $(PDL) $(ODSTORAG_DIR) -o $@

$(ODDATAX_IDL):
	$(IDLTOOL) $(PDL) $(ODDATAX_DIR) -o $@

$(ODUI_IDL):
	$(IDLTOOL) $(PDL) $(ODUI_DIR) -o $@

$(ODIMAGNG_IDL):
	$(IDLTOOL) $(PDL) $(ODIMAGNG_DIR) -o $@

$(ODLAYOUT_IDL):
	$(IDLTOOL) $(PDL) $(ODLAYOUT_DIR) -o $@

$(ODBINDNG_IDL):
	$(IDLTOOL) $(PDL) $(ODBINDNG_DIR) -o $@

$(ODSHELL_IDL):
	$(IDLTOOL) $(PDL) $(ODSHELL_DIR) -o $@

$(ODREGSTY_IDL):
	$(IDLTOOL) $(PDL) $(ODREGSTY_DIR) -o $@


dist install:





