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

/* these maps evenly distribute the dither pattern */
static unsigned char ditherMap3[64]={
0,36,32,4,18,54,50,22,
16,52,48,20,2,38,34,6,
9,45,41,13,27,63,59,31,
25,61,57,29,11,47,43,15,
8,44,40,12,26,62,58,30,
24,60,56,28,10,46,42,14,
1,37,33,5,19,55,51,23,
17,53,49,21,3,39,35,7};
static unsigned char ditherMap4[256]={
0,136,128,8,68,204,196,76,
64,200,192,72,4,140,132,12,
34,170,162,42,102,238,230,110,
98,234,226,106,38,174,166,46,
32,168,160,40,100,236,228,108,
96,232,224,104,36,172,164,44,
2,138,130,10,70,206,198,78,
66,202,194,74,6,142,134,14,
17,153,145,25,85,221,213,93,
81,217,209,89,21,157,149,29,
51,187,179,59,119,255,247,127,
115,251,243,123,55,191,183,63,
49,185,177,57,117,253,245,125,
113,249,241,121,53,189,181,61,
19,155,147,27,87,223,215,95,
83,219,211,91,23,159,151,31,
16,152,144,24,84,220,212,92,
80,216,208,88,20,156,148,28,
50,186,178,58,118,254,246,126,
114,250,242,122,54,190,182,62,
48,184,176,56,116,252,244,124,
112,248,240,120,52,188,180,60,
18,154,146,26,86,222,214,94,
82,218,210,90,22,158,150,30,
1,137,129,9,69,205,197,77,
65,201,193,73,5,141,133,13,
35,171,163,43,103,239,231,111,
99,235,227,107,39,175,167,47,
33,169,161,41,101,237,229,109,
97,233,225,105,37,173,165,45,
3,139,131,11,71,207,199,79,
67,203,195,75,7,143,135,15};
