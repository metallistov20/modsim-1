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

#include "defines.h"

/* To declare external variables and functions */
#define global extern

#include "stdlib.h"

#undef global

/* To declare global variables and functions */
#define global

#include "util.h"

void delay(int16_t value)
{
	while (value-- != 0u)
	{

	}
}

void increment(uint16_t *value)
{
	(*value)++;
}

void mem_copy(uint8_t *dest, uint8_t *source, int16_t len)
{
	while (len-- != 0u)
	{
		*(dest++) = *(source++);
	}
}

/* linked lists */

void lnk_lst (void)
{
/* can not use malloc/free and sprintf because the generated program exceeds the 16kb code limit */
}
