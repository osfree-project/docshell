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

#ifndef _STORDEF_
#define _STORDEF_

#include <ODTypes.h>

#define kODAppleScrapTypePrefix "+//ISO 9070/ANSI::113722::US::CI LABS::MacOS:ScrapType:"
#define kODAppleFileTypePrefix "+//ISO 9070/ANSI::113722::US::CI LABS::MacOS:FileType:"

#ifdef _PLATFORM_WIN32_
#define kODdragDraw 	0x00000001
#define kODdragErase	0x00000002
#define kODdragMove		0x00000008
#define kODdragCopy		0x00000010
#define kODdragLink		0x00000020
#endif

#endif
