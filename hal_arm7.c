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

/* printf(), stdout */
#include <stdio.h>

/* struct timeval, gettimeofday() */
#include <sys/time.h>

/* Basic project definitions  */
#include "modsim.h"

/* Type 'pTimepointType' definition */
#include "datastruct.h"

/* Time measurement variable to define current position on time scale */
struct timeval endtimePROC;

/* Var. defined in 'datastruct.c' */
extern struct timeval starttimePROC;

/* Check if raw value is USB 2.0 <logical 0> and return '1' if so, return '0' otherwise */
int iChkUsb20Lg0(QuasiFloatType qfltVal)
{
	/* USB 2.0 levels. Logical '0'. LOGIC_0_CURR. -10 mV .. 10 mV */
	if (
		/* -1.xxxxx e-02 mV */
		( (-1 == qfltVal.integer) && ('-' == qfltVal.sgn ) && (2 == qfltVal.power) )
		||
		/* -x.xxxxx e-03 mV, -x.xxxxx e-04 mV, .. , -x.xxxxx e-09 mV */
		( 0 > qfltVal.integer ) && ( ('-' == qfltVal.sgn ) && (3 <= qfltVal.power) )
		||
		/* +x.xxxxx e-03 mV, +x.xxxxx e-04 mV, .. , +x.xxxxx e-09 mV */
		( 0 < qfltVal.integer ) && ( ('+' == qfltVal.sgn ) && (3 <= qfltVal.power) )
		||
		/* +1.xxxxx e-02 mV */
		( (1 == qfltVal.integer) && ('+' == qfltVal.sgn ) && (2 == qfltVal.power) )
		|| 
		/* -0.00000 e+00, +0.00000 e+00 */
		( (0 == qfltVal.integer) && ('+' == qfltVal.sgn ) && (0 == qfltVal.power) && (0 == qfltVal.fraction) )
	) return;
}

/* Check if raw value is USB 2.0 <logical 1> and return '1' if so, return '0' otherwise */
int iChkUsb20Lg1(QuasiFloatType qfltVal)
{
	/* USB 2.0 levels. Logical '1'. LOGIC_1_CURR. 0.36V .. 0.44V */
	if (
		/* 3.60000e-01 .. 3.99999 e-01 */ /*last parenthesis: 600000-99999*/
		(3 == qfltVal.integer) && (  ('-' == qfltVal.sgn ) && (1 <= qfltVal.power) && (600000 <= qfltVal.fraction)  )
		||
		/* 4.00000e-01 .. 4.400000e-01 */ /*last parenthesis: 00000-400000*/
		(4 == qfltVal.integer) && (  ('-' == qfltVal.sgn ) && (1 <= qfltVal.power) && (400000 >= qfltVal.fraction)  )
	) return 1;

	return 0;
}

/* Process Realtime and Relative-time values: certain data to be oputput onto DAC/PWMI */
int ProcessPoint( pTimepointType pTimepoint)
{
	return P_SUCCESS;
}
