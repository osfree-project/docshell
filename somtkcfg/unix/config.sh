#!/bin/sh -ex
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
# $Id$
#

MAKEDEFS="$1"

test -f "$MAKEDEFS"

if test "$SOMBASE" = ""
then
	if test -d /opt/somtk
	then
		SOMBASE=/opt/somtk
	else
		SOMBASE="$OUTDIR"
	fi
fi

if test "$SOMBASE" != ""
then
	if test "$SC" = ""
	then
		if test -x "$SOMBASE/bin/sc"
		then
			SC="$SOMBASE/bin/sc"
			echo "SC=$SC" >>"$MAKEDEFS"
		fi
	fi

	if test "$PDL" = ""
	then
		if test -x "$SOMBASE/bin/pdl"
		then
			PDL="$SOMBASE/bin/pdl"
			echo "PDL=$PDL" >>"$MAKEDEFS"
		fi
	fi

	if test "$SOMTKLIBS" = ""
	then
		if test -d "$SOMBASE/lib"
		then
			SOMTKLIBS="-L$SOMBASE/lib -lsomos -lsomem -lsomu -lsom"
			echo "SOMTKLIBS=$SOMTKLIBS" >>"$MAKEDEFS"
		fi
	fi

	if test "$SOMTK_IDL" = ""
	then
		if test -d "$SOMBASE/include"
		then
			SOMTK_IDL="$SOMBASE/include"
			echo "SOMTK_IDL=$SOMTK_IDL" >>"$MAKEDEFS"
		fi
	fi

	if test "$SOMTKINCL" = ""
	then
		if test -d "$SOMBASE/include"
		then
			SOMTKINCL="-I$SOMBASE/include"
			echo "SOMTKINCL=$SOMTKINCL" >>"$MAKEDEFS"
		fi
	fi
fi

test "$PDL" != ""
test "$SC" != ""
test "$SOMTKLIBS" != ""
test "$SOMTKINCL" != ""
test "$SOMTK_IDL" != ""

