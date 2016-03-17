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

/*

   Two sorting algorithms, bubble_sort and search_sort.
   Both sort the global list called list, both need the function swap to
   exchange two elements.

   Function main calls both sorting routines in an endless loop with
   changing values in list. Values for list are produced by shake based on
   its two input variables. New values for the input variables are produced
   by function random.

*/

#include "defines.h"

/* To declare external variables and functions */
#define global extern



#undef global
/* To declare global variables and functions */
#define global

#include "sort.h"

#define COUNT         20u

#define COUNT_MIN_1   19u

static uint32_t list[COUNT];

static void random2(uint32_t *no1, uint32_t *no2)
{
	(*no1)++;
	*no2 += 13u;
}

static void shake(uint32_t random_no1, uint32_t random_no2)
{
uint8_t i;

	for (i = 0u; i < COUNT; i++)
	{
		list[i] = random_no1 + random_no2;

		random_no1 *= 25793u;

		random_no2 *= 12391u;
	}
}

static void swap(uint32_t *obj1, uint32_t *obj2)
{
uint32_t tmp;

	tmp   = *obj1;

	*obj1 = *obj2;

	*obj2 = tmp;
}

static void bubble_sort(void)
{
uint8_t i = 0u, j = 1u;

	for (i = 0u; i < COUNT_MIN_1; i++)
	{
		for (j = COUNT_MIN_1; j > i; j--)
		{
			if (list[j] < list[j-1u])
			{
				swap(&list[j], &list[j-1u]);
			}
		}
	}
}

static void search_sort(void)
{
uint8_t i = 1u, j = 1u;

uint16_t min_ind;

	for (i = 0u; i < COUNT_MIN_1; i++)
	{
		min_ind = i;

		for (j = i + 1u; j <= COUNT_MIN_1; j++)
		{
			if (list[min_ind] > list[j])
			{
				min_ind = j;
			}
			if (min_ind != i)
			{
				swap(&list[i], &list[min_ind]);
			}
		}
	}
}

void sort (void)
{
uint32_t random_no1 = 0u, random_no2 = 1u;

	shake(random_no1, random_no2);

	bubble_sort ();

	shake(random_no1, random_no2);

	search_sort ();

	random2(&random_no1, &random_no2);
}
