/*
 (C) Copyright 2016, TP-Link Inc, konstantin.mauch@tp-link.com

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License as
 published by the Free Software Foundation; either version 2 of
 the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT any WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 MA 02111-1307 USA
*/

#ifndef _DEFINES_H_
#define _DEFINES_H_

/* length specific types recommended by MISRA rule 13 */

/*
typedef  signed char    int8_t;
typedef  unsigned char  uint8_t;
typedef  signed short   int16_t;
typedef  unsigned short uint16_t;
typedef  signed long    int32_t;
typedef  unsigned long  uint32_t;

typedef  float    float16;
typedef  double   float32;
*/


#define  int8_t signed char
#define  uint8_t unsigned char
#define int16_t signed short
#define uint16_t unsigned short
#define int32_t signed long 
#define uint32_t  unsigned long

#define float16 float
#define  float32 double

#endif /* _DEFINES_H_ */
