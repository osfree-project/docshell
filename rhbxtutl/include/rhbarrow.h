/**************************************************************************
 *
 *  Copyright 2008, Roger Brown
 *
 *  This file is part of Roger Brown's Toolkit.
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or (at your
 *  option) any later version.
 * 
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */

/*
 * $Id$
 */

#define arrow_width 16
#define arrow_height 32
#define arrow_x_hot 1
#define arrow_y_hot 1
static unsigned char arrow_bits[] = {
   0x03, 0x00, 
   0x07, 0x00, 
   0x0f, 0x00, 
   0x1f, 0x00, 
   0x3f, 0x00, 
   0x7f, 0x00,
   0xff, 0x00, 
   0xff, 0x01, 
   0xff, 0x03, 
   0xff, 0x03, 
   0x7f, 0x00, 
   0xf7, 0x00,
   0xf3, 0x00, 
   0xe1, 0x01, 
   0xe0, 0x01, 
   0xc0, 0x00, 
   0x00, 0x00, 
   0x02, 0x00,
   0x06, 0x00, 
   0x0e, 0x00, 
   0x1e, 0x00, 
   0x3e, 0x00, 
   0x7e, 0x00, 
   0xfe, 0x00,
   0xfe, 0x01, 
   0x3e, 0x00, 
   0x36, 0x00, 
   0x62, 0x00, 
   0x60, 0x00, 
   0xc0, 0x00,
   0xc0, 0x00, 
   0x00, 0x00};
