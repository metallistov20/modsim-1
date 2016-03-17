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

/* to declare external variables and functions */
#define global extern   

#include "example.h"


#undef global
/* to declare global variables and functions */
#define global          

#include "date_cnt.h"

#define DATE_LEN  11u

struct DATE_ST
{
	uint16_t day;
	uint16_t month;
	uint16_t year;
	uint8_t mem_date[DATE_LEN];
} date = {1u, 1u, 2002u} ;

static void date_cpy (void)
{
	date.mem_date[0u] = '0' + date.day / 10u;
	date.mem_date[1u] = '0' + date.day % 10u;
	date.mem_date[2u] = ':';
	date.mem_date[3u] = '0' + date.month / 10u;
	date.mem_date[4u] = '0' + date.month % 10u;
	date.mem_date[5u] = ':';
	date.mem_date[6u] = '0' + date.year / 1000u;
	date.mem_date[7u] = '0' + (date.year / 100u) % 10u;
	date.mem_date[8u] = '0' + (date.year / 10u) % 10u;
	date.mem_date[9u] = '0' + (date.year % 10u) ;
	date.mem_date[10u]= '\0';
}

void inc_date (uint16_t GlobalCounter)
{
	if (!(GlobalCounter % 100u))
	{
		date.day++;
	}

	if (date.day >= 31u)
	{
		date.day = 1u;
		date.month++;
	}

	if (date.month >= 13u)
	{
		date.month = 1u;
		date.year ++;
	}

	date_cpy();

}
