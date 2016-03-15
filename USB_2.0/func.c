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

#include "util.h"

#undef global

/* To declare global variables and functions */
#define global          

#include "func.h"

static uint32_t Count1,Count2;

static uint32_t Count11, Count12, Count21, Count22;

static uint16_t Count111, Count112, Count121, Count122;

static uint8_t Count211, Count212, Count221, Count222;

/* module func.c */

static void func111 (void)
{
	Count111++;
}

static void func112 (void)
{
	Count112++;
}

static void func121 (void)
{
	Count121++;
}

static void func122 (void)
{
	Count122++;
}

static void func211 (void)
{
	Count211++;
}

static void func212 (void)
{
	Count212++;
}

static void func221 (void)
{
	Count221++;
}

static void func222 (void)
{
	Count222++;
}

static void func11(uint16_t count)
{
	while (count-- != 0u)
	{
		func111 ();
	}

	func112 ();

	Count11++;
}

static void func12 (uint16_t count)
{
	while (count-- != 0u)
	{
		func121 ();
	}

	func122 ();

	Count12++;
}

static void func21 (uint16_t count)
{
	while (count-- != 0u)
	{
		func211 ();
	}

	func212 ();

	Count21++;
}

static void func22 (uint16_t count)
{
	while (count-- != 0u)
	{
		func221();
	}

	func222();

	Count22++;
}

static void func1 (uint16_t count)
{
	func11 (count % 4u);

	func12 (count % 5u);

	Count1++;
}

static void func2 (uint16_t count)
{
	func21 (count % 2u);

	func22 (count % 3u);

	Count2++;
}

void init_func (void)
{
	Count1    = 0u;
	Count2    = 0u;
	Count11   = 0l;
	Count12   = 0l;
	Count21   = 0l;
	Count22   = 0l;
	Count111  = 0u;
	Count112  = 0u;
	Count121  = 0u;
	Count122  = 0u;
	Count211  = (uint8_t)0u;
	Count212  = (uint8_t)0u;
	Count221  = (uint8_t)0u;
	Count222  = (uint8_t)0u;
}

uint32_t func(int16_t count)
{
	init_func ();

	func1 (count);

	func2 (count);

	delay (count);

	return (Count11);
}
