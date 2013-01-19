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

TARGET=$(OUTDIR_TOOLS)/region
OBJS=$(INTDIR)/region.o
REGION_H=$(INTDIR)/region.h

all: all-$(HAVE_LIBX11)

all-true: $(TARGET) $(REGION_H)

all-false:

clean:
	$(CLEAN) $(OBJS) $(TARGET) $(REGION_H)

$(INTDIR)/region.o: ../src/region.c
	$(CC_EXE) $(STDOPT) $(STDINCL) -c ../src/region.c -o $@ 

$(TARGET): $(OBJS)
	$(LINKAPP) \
		$(LINKAPP_HEAD) \
		$(OBJS) -o $@ \
		$(X11APP) \
		$(X11LIBS) \
		$(SOCKLIBS) \
		$(STDLIB) \
		$(LINKAPP_TAIL) 

$(REGION_H): $(TARGET)
	$(EXEC_CONFIG) $(TARGET) $@

dist install:

