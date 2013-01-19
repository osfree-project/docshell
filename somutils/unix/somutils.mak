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

SOMIDL_IDL=../../somidl
IDLTOOL=../../toolbox/idltool.sh

SOMIDL_HEADERS=$(SOMIDL_IDL)/$(PLATFORM)

SOMUC_DIR=../../somuc

SOMUC_INTDIR=$(SOMUC_DIR)/$(PLATFORM)/$(BUILDTYPE)

SOMUC_IDL=	$(SOMIDL_IDL)/mlink.idl			\
			$(SOMIDL_IDL)/tpll.idl			\
			$(SOMIDL_IDL)/tpllitr.idl

SOMUC_H=	$(SOMIDL_HEADERS)/mlink.h		\
			$(SOMIDL_HEADERS)/tpll.h		\
			$(SOMIDL_HEADERS)/tpllitr.h

SOMUC_XH=	$(SOMIDL_HEADERS)/mlink.xh		\
			$(SOMIDL_HEADERS)/tpll.xh		\
			$(SOMIDL_HEADERS)/tpllitr.xh

SOMUC_IH=	$(SOMUC_INTDIR)/mlink.ih	\
			$(SOMUC_INTDIR)/mlink.h		\
			$(SOMUC_INTDIR)/tpll.ih		\
			$(SOMUC_INTDIR)/tpll.h		\
			$(SOMUC_INTDIR)/tpllitr.ih	\
			$(SOMUC_INTDIR)/tpllitr.h	

DIRS=	$(SOMIDL_IDL) $(SOMIDL_HEADERS) $(SOMUC_INTDIR)	

all:	config 				\
		$(DIRS)				\
		$(SOMUC_IDL)		\
		$(SOMUC_H)			\
		$(SOMUC_XH)			\
		$(SOMUC_IH)

config:
	if test "$(SC)" = ""; then echo check SC; exit 1; fi
	if test "$(PDL)" = ""; then echo check PDL; exit 1; fi
	if test "$(SOMTK_IDL)" = ""; then echo check SOMTK_IDL; exit 1; fi

clean:
	$(CLEAN) $(SOMUC_IH)
	$(CLEAN) $(SOMUC_IDL)
	$(CLEAN) $(SOMUC_H)
	$(CLEAN) $(SOMUC_XH)

$(SOMUC_IH): 
	$(IDLTOOL) $(SC) $(SOMUC_DIR) -o $@ -p -I$(SOMUC_DIR) -I$(SOMIDL_IDL) -I$(SOMTK_IDL)

$(SOMUC_H) $(SOMUC_XH):
	$(IDLTOOL) $(SC) $(SOMIDL_IDL) -o $@ $(PLATFORM_SCFLAGS) -I$(SOMIDL_IDL) -I$(SOMTK_IDL)

$(SOMUC_IDL):
	$(IDLTOOL) $(PDL) $(SOMUC_DIR) -o $@

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

