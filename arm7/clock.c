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

#include "example.h"
#include "clock.h"
#include "func.h"
#include "date_cnt.h"
#include "util.h"
#include "sort.h"

extern void _enableInterrupts(void);
extern void _disableInterrupts(void);

struct TIME
{
	uint16_t MilliSec;
	uint8_t  Sec;
	uint8_t  Min;
	uint8_t  Hour;
	int8_t   Time[TIME_LEN];
} Clock;

struct WATCH
{
	uint16_t  Sec;
	uint16_t  Min;
	uint16_t  Hour;
} Watch;

void init_clock(void)
{
	Clock.MilliSec   = 0u;
	Clock.Sec       = 0u;
	Clock.Min       = 0u;
	Clock.Hour      = 0u;
	convert_clock();
}

void inc_clock(void)
{
	/* always called from interrupt routine, no critical section */

	if (Clock.MilliSec >= 1000u)
	{
		Clock.MilliSec = 0u;
		Clock.Sec++;
		if (Clock.Sec >= 60u)
		{
			 Clock.Sec   = 0u;
			 Clock.Min++;
			 if (Clock.Min >= 60u)
			 {
				Clock.Min   = 0u;
				Clock.Hour++;
				if (Clock.Hour >= 24u)
				{
					Clock.Hour  = 0u;
				}
			}
		}
	}
	else
	{
		Clock.MilliSec += 5u;
	}
}

void convert_clock(void)
{
	/* always called from interrupt routine, so no critical section */

	Clock.Time[0u]    = '0' + Clock.Hour / 10u;
	Clock.Time[1u]    = '0' + Clock.Hour % 10u;
	Clock.Time[2u]    = ':';
	Clock.Time[3u]    = '0' + Clock.Min / 10u;
	Clock.Time[4u]    = '0' + Clock.Min % 10u;
	Clock.Time[5u]    = ':';
	Clock.Time[6u]    = '0' + Clock.Sec / 10u;
	Clock.Time[7u]    = '0' + Clock.Sec % 10u;
	Clock.Time[8u]    = '.';
	Clock.Time[9u]    = '0' + Clock.MilliSec / 100u;
	Clock.Time[10u]   = '0' + (Clock.MilliSec / 10u) % 10u;
	Clock.Time[11u]   = '\0';
}

void signal_clock(void)
{
	int16_t   Count1=100u, Count2=200u, Count3=300u;

	if (((Clock.Sec%3u)==0u) && (Clock.MilliSec==0u))
	{
		while (Count1-- != 0u)
		{
		}
	}

	/* every 3 seconds: action 1 */

	if (((Clock.Sec%5u)==0u) && (Clock.MilliSec==0u))
	{
		while (Count2-- != 0u)
		{
		}
	}

	/* every 5 seconds: action 2 */

	if (((Clock.Sec%10u)==0u) && (Clock.MilliSec==0u))
	{
		while (Count3-- != 0u)
		{
		}
	}
	/* every 10 seconds: action 3 */
}


void analog_clock(void)
{
	struct TIME tmpclock;
	uint16_t     tmpvalue;

	/* disable ISR */
//TODO: resolve	_disableInterrupts();

	/*critical copy: interrupt could come in between*/
	mem_copy ((uint8_t*)&tmpclock, (uint8_t*)&Clock, sizeof(struct TIME));

	/* enable ISR */
//TODO: resolve  _enableInterrupts();        

	tmpvalue    = (tmpclock.MilliSec);
	tmpvalue   /= 1000u;
	tmpvalue   += (uint16_t)(tmpclock.Sec);
	tmpvalue   /= 60u;
	Watch.Sec   = 360u * tmpvalue;

	/*second-hand in degrees of the full circle*/

	tmpvalue   += (uint16_t)(tmpclock.Min);
	tmpvalue   /= 60u;
	Watch.Min   = 360u * tmpvalue;

	/*minute-hand in degrees of the full circle*/

	tmpvalue   += (uint16_t)(tmpclock.Hour);
	tmpvalue   /= 24u;
	Watch.Hour  = 360u * tmpvalue;

	/* hour-hand in degrees of the full circle */
}

void copy_clock(uint8_t *Buf)
{
	/* disable ISR */
//TODO: resolve	_disableInterrupts();

	/* critical copy: interrupt could come in between */
	mem_copy(Buf,(uint8_t*)&(Clock.Time),TIME_LEN);

	/* enable ISR */
//TODO: resolve	_enableInterrupts();
}

uint16_t get_clock(void)
{
   uint16_t tmp;

	/* disable ISR */
//TODO: resolve	_disableInterrupts();

	/* critical section: interrupt could come in between */
	tmp       = Clock.Hour * 24u;
	tmp      += Clock.Min;
	tmp      *= 60u;
	tmp      += Clock.Sec;

	/* enable ISR */
//TODO: resolve	_enableInterrupts();

	return (tmp);
}
