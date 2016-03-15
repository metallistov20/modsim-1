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

/* stdout */
#include <stdio.h>

//f.o. #include <string.h>

/* calloc() */
#include <stdlib.h>

/* struct timeval, gettimeofday() */
#include <sys/time.h>

/* Data structure type definition */
#include "dstruct.h"

/* Port D definitions, prototypes */
#include "dport.h"

int iOldSecPRC;

/* Time measurement variables */
struct timeval starttimePROC, endtimePROC;

/* Attach 3 floats to tail of dynamic structure 'pTimepointType' */
int _EnrollPoint(const char * caller, pTimepointType * ppThisPointChain, 
#if !defined(QUASIFLOAT) 
	float * pfltTm, float * pfltX, float * pfltY, 
#else
	pQuasiFloatType pqfltTm, pQuasiFloatType pqfltX, pQuasiFloatType pqfltY, 
#endif /* !defined(QUASIFLOAT) */
	char * pcMrq)

{
pTimepointType pChild, pTempPointChain;

	if (NULL == *ppThisPointChain)
	{
		/* only one chain, for beginning */
		*ppThisPointChain = (pTimepointType) calloc ( 1, sizeof (TimepointType) );

		/* check if successful */
		if (NULL == *ppThisPointChain)
		{
#if !defined(QUASIFLOAT)
			printf("[%s] %s:%s : ERROR: can't allocate memory for first element. %f: [X(%f),Y(%f)]  \n",
			__FILE__, caller, __func__,
			*pfltTm, *pfltX, *pfltY);
#else
			printf("[%s] %s:%s : ERROR: can't allocate memory for first element. \n",
			__FILE__, caller, __func__);
#endif /* !defined(QUASIFLOAT) */

			return (-8);
		}

#if !defined(QUASIFLOAT)
		(*ppThisPointChain)->fltXval = *pfltX;
		(*ppThisPointChain)->fltYval = *pfltY;
		(*ppThisPointChain)->fltAbsTime = *pfltTm;
#else
		memcpy(& ((*ppThisPointChain)->qfltXval), pqfltX, sizeof(QuasiFloatType) );
		memcpy(& ((*ppThisPointChain)->qfltYval), pqfltY, sizeof(QuasiFloatType) );
		memcpy(& ((*ppThisPointChain)->qfltAbsTime), pqfltTm, sizeof(QuasiFloatType) );
#endif /* !defined(QUASIFLOAT) */

		(*ppThisPointChain)->pcMarquee = calloc (1, strlen (pcMrq) +1 );
		strcpy( (*ppThisPointChain)->pcMarquee, pcMrq);

#if DEBUG_DATA
#if !defined(QUASIFLOAT)
		printf("[%s] %s:%s : FIRST <%f> <%f> <%f> <%s> \n", __FILE__, caller, __func__,
			(*ppThisPointChain)->fltAbsTime,
			(*ppThisPointChain)->fltXval,
			(*ppThisPointChain)->fltYval,
			(*ppThisPointChain)->pcMarquee
		);
#else
		printf("[%s] %s:%s : FIRST <%d.%dE%c0%d> <%d.%dE%c0%d> <%d.%dE%c0%d> <%s> \n", __FILE__, caller, __func__,

			(*ppThisPointChain)->qfltAbsTime.integer,(*ppThisPointChain)->qfltAbsTime.fraction,
			(*ppThisPointChain)->qfltAbsTime.sgn,(*ppThisPointChain)->qfltAbsTime.power,

			(*ppThisPointChain)->qfltXval.integer,(*ppThisPointChain)->qfltXval.fraction,
			(*ppThisPointChain)->qfltXval.sgn,(*ppThisPointChain)->qfltXval.power,

			(*ppThisPointChain)->qfltYval.integer,(*ppThisPointChain)->qfltYval.fraction,
			(*ppThisPointChain)->qfltYval.sgn,(*ppThisPointChain)->qfltYval.power,

			(*ppThisPointChain)->pcMarquee
		);
#endif /* !defined(QUASIFLOAT) */

#endif /* (DEBUG_DATA) */

	}
	else
	{
		/* point with first temporary element to head of chain */
		pChild = *ppThisPointChain;

		pTempPointChain = (pTimepointType) calloc (1, sizeof (TimepointType) );

		if (NULL == pTempPointChain)
		{
#if !defined(QUASIFLOAT)
			printf("[%s] %s:%s : ERROR: can't allocate memory for next element. %f: [X(%f),Y(%f)]  \n", 
			__FILE__, caller, __func__,
			*pfltTm, *pfltX, *pfltY);
#else
			printf("[%s] %s:%s : ERROR: can't allocate memory for next element.\n", 
			__FILE__, caller, __func__);
#endif /* !defined(QUASIFLOAT) */

			return (-7);
		}

#if !defined(QUASIFLOAT)
		pTempPointChain->fltXval = *pfltX;
		pTempPointChain->fltYval = *pfltY;
		pTempPointChain->fltAbsTime = *pfltTm;
#else
		memcpy(& ( pTempPointChain->qfltXval), 	pqfltX, sizeof(QuasiFloatType) );
		memcpy(& ( pTempPointChain->qfltYval), 	pqfltY, sizeof(QuasiFloatType) );
		memcpy(& ( pTempPointChain->qfltAbsTime), pqfltTm, sizeof(QuasiFloatType) );
#endif /* !defined(QUASIFLOAT) */

		pTempPointChain->pcMarquee = calloc (1, strlen (pcMrq) +1 );
		strcpy( pTempPointChain->pcMarquee, pcMrq);

#if DEBUG_DATA
#if !defined(QUASIFLOAT)
		printf("[%s] %s:%s : NEXT <%f> <%f> <%f> <%s> \n", __FILE__, caller, __func__,
			pTempPointChain->fltAbsTime,
			pTempPointChain->fltXval,
			pTempPointChain->fltYval,
			pTempPointChain->pcMarquee
		);
#else
		printf("[%s] %s:%s : NEXT <%d.%dE%c0%d> <%d.%dE%c0%d> <%d.%dE%c0%d> <%s> \n", __FILE__, caller, __func__,

			pTempPointChain->qfltAbsTime.integer,pTempPointChain->qfltAbsTime.fraction,
			pTempPointChain->qfltAbsTime.sgn,pTempPointChain->qfltAbsTime.power,

			pTempPointChain->qfltXval.integer,pTempPointChain->qfltXval.fraction,
			pTempPointChain->qfltXval.sgn,pTempPointChain->qfltXval.power,

			pTempPointChain->qfltYval.integer,pTempPointChain->qfltYval.fraction,
			pTempPointChain->qfltYval.sgn,pTempPointChain->qfltYval.power,

			pTempPointChain->pcMarquee
		);
#endif /* !defined(QUASIFLOAT) */
#endif /* (DEBUG_DATA) */

		/* Skip everything, except last entry */
		while ( (NULL != pChild) && (NULL != pChild->pNext ) )
		{
			/* . . */
			pChild = pChild->pNext;
		}

		/* before while we've already ensured that next chunk was created allright */

		/* attach a new chain entry to the end of existing chain */
		pChild->pNext = pTempPointChain;

	}
	return (0);
}

/* Check if raw value is USB 1.0 <logical 0> and return '1' if so, return '0' otherwise */
int iChkUsb10Lg0(QuasiFloatType qfltVal)
{
	/* USB 1.0 levels. Logical '0'. LOGIC_0_CURR */
	if (
		('+' == qfltVal.sgn) && (0 == qfltVal.power) &&

		 (  (0 == qfltVal.integer)&&(4 >= qfltVal.fraction)   ) 

	) return 1;

	return 0;
}

/* Check if raw value is USB 1.0 <logical 1> and return '1' if so, return '0' otherwise */
int iChkUsb10Lg1(QuasiFloatType qfltVal)
{

	/* USB 1.0 levels. Logical '1'.  LOGIC_1_CURR */
	if (
		('+' == qfltVal.sgn) && (0 == qfltVal.power) &&
		 (  (2 == qfltVal.integer)&&(4 <= qfltVal.fraction)   ) ||  (    3 <= qfltVal.integer   )    
	) return 1;

	return 0;
}

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

/* Process Realtime and Relative-time values: certain data to be oputput onto Port D */
int ProcRealAndRel(
#if !defined(QUASIFLOAT)
	float fltRealTime,
	float fltXval
#else
#if defined(DIN_FEEDBACK)
	QuasiFloatType qfltRealTime,
	QuasiFloatType qfltXval,
	QuasiFloatType qfltYval,
	char * pcMarq,
	QuasiFloatType qfltAbsTime
#else
	QuasiFloatType qfltRealTime,
	QuasiFloatType qfltXval,
	QuasiFloatType qfltAbsTime
#endif /* (defined(DIN_FEEDBACK)) */
#endif /* !defined(QUASIFLOAT) */
	)
{
#if !defined(QUASIFLOAT)
float fltRelTime;
float _left, _right;
float fltJiffy = 1.0;
#else
QuasiFloatType qfltRelTime;
int _left, _right;
QuasiFloatType qfltJiffy; 
qfltJiffy.fraction = 1;
#endif /* !defined(QUASIFLOAT) */

/*
1. On both PC, and MC68EZ328-based HW, going across list allocated in memory runs faster than
time process defined by raw data. 

2. For this reason we wait a little (do-while-loop below) before issuing current value
onto Port D. Thus we shift 'current' time a bit ahead.

3. Once, after shifting, we see that 'current' time is not less than time defined by raw data
value , it's OK to issue current value onto Port D.
*/

#if !defined(QUASIFLOAT)

	/* Operate uSeconds multiplied by 10e6 because <usleep> accepts	integer parameters only */
	fltRealTime = fltRealTime*1000000;

	printf("[%s] : <BEFORE TIME SHIFTING> real tm.: %f\n", __FILE__, /* caller, */	fltRealTime	);

	/* Don't proceed with this fuction once given an unappicable input data */
	if (0.0 == fltRealTime ) return;

	do 
	{	/* Take current time */
		gettimeofday(&endtimePROC,0);

		/* Compute how much time elapsed since head of list processing till now */
		fltRelTime = 1000000*(endtimePROC.tv_sec - starttimePROC.tv_sec - 6.0) 
			+ endtimePROC.tv_usec - starttimePROC.tv_usec;

		/* If relative time stays on the left from 0 */
		if (fltRelTime < 0)

			/* then '_right' is relative time, '_left' is real time */
			_right = fltRelTime, _left = fltRealTime;

		/* If relative time stays on the right from 0, or is exctly 0 */
		else
			/* then '_right' is real time, '_left' is relative time */
			_left = fltRelTime, _right = fltRealTime;



		/* Wait for relative time <fltRelTime> to catch up with real time <fltRealTime>  */
		usleep (fltJiffy);

		printf("[%s] : <TIME SHIFTING> real tm.: %f, shiftable tm.: %f \n", __FILE__, fltRealTime,	fltRelTime ); 

	} while (_right < _left);
	
	/* Now they're equal or least 'relative tm' is not less than 'real tm' */
	printf("[%s] : <AFTER TIME SHIFTING> will pretend like <%f>, is same as <%f> \n", __FILE__,
		fltRealTime,
		fltRelTime );

	/* Hardware Port 'D' processing */

#if !defined(USB20)
	/* USB 1.0 levels. Logical '1' */
	if (fltXval >= LOGIC_1_CURR) 
#else
	/* USB 2.0 levels. Logical '1' */
	if (fltXval >= LOGIC_1_CURR && fltXval <= LOGIC_1up_CURR) 
#endif /* (!defined(USB20)) */

		PortD_Up( PD0 );
	else
#if !defined(USB20)
		/* USB 1.0 levels. Logical '0' */
		if (fltXval <= LOGIC_0_CURR && fltXval <= LOGIC_0_CURR) 
#else
		/* USB 2.0 levels. Logical '0' */
		if (fltXval >= LOGIC_0_CURR && fltXval <= LOGIC_0up_CURR) 
#endif /* (!defined(USB20)) */

			PortD_Down( PD0 );
		else
		{
/* Leave excessive current (OVERDOSE_CURR) unprocessed, and leave other 'dubious' cases unprocessed:
			pPointChain->pcMarquee = calloc (1, strlen (NPROC) +1 );
			strcpy( pPointChain->pcMarquee, NPROC); */
#if DEBUG_DATA
				printf("[%s] %s:%s :  <%s> \n", __FILE__, caller, __func__,
					pPointChain->pcMarquee	);
#endif /* (DEBUG_DATA) */
		}


#else


	qfltRealTime.integer = 			(qfltRealTime.integer < 0)?
		(qfltRealTime.integer * 1000000) - (qfltRealTime.fraction / 10):
		(qfltRealTime.integer * 1000000) + (qfltRealTime.fraction / 10);

	/* Take current time */
	gettimeofday(&endtimePROC,0);

	/* Compute time elapsed since head of list processing till now */
	qfltRelTime.integer = 1000000*(endtimePROC.tv_sec - starttimePROC.tv_sec - 6.0) 
		+ endtimePROC.tv_usec - starttimePROC.tv_usec;


#if defined(FAST_UCSIMM)
	printf("[%s] : <BEFORE TIME SHIFTING> real tm.: %d, shiftable tm.: %d \n", __FILE__,
		qfltRealTime.integer,	qfltRelTime.integer );
#else
#endif /* defined(FAST_UCSIMM) */

	/* TODO: resolve bug. On (-1.0 .. 1.0) comparing <qfltRelTime.integer> against 0 is not sufficient. */

	/* If relative time stays on the left from 0 */
	if (qfltRelTime.integer < 0)

		/* then '_right' is relative time, '_left' is real time */
		_right = qfltRelTime.integer, _left = qfltRealTime.integer;

	/* If relative time stays on the right from 0, or is exctly 0 */
	else
		/* then '_right' is real time, '_left' is relative time */
		_left = qfltRelTime.integer, _right = qfltRealTime.integer;

	if (0 == qfltRealTime.integer) return;

	/* TODO: make <do-while> instead of <while-do>, thus avoid a code duplication */
	while (_right < _left )
	{
		/* Wait for relative <fltRelTime> to catch up with absolute <fltRealTime>  */
		usleep (qfltJiffy.fraction);

		/* Take current time */
		gettimeofday(&endtimePROC,0);

		/* Compute how much time elapsed since head of list processing till now */
		qfltRelTime.integer = 1000000*(endtimePROC.tv_sec - starttimePROC.tv_sec - 6.0) 

			+ endtimePROC.tv_usec - starttimePROC.tv_usec;

		/* If relative time stays on the left from 0 */
		if (qfltRelTime.integer < 0)

			/* then '_right' is relative time, '_left' is real time */
			_right = qfltRelTime.integer, _left = qfltRealTime.integer;

		/* If relative time stays on the right from 0, or is exctly 0 */
		else

			/* then '_right' is real time, '_left' is relative time */
			_left = qfltRelTime.integer, _right = qfltRealTime.integer;

#if defined(FAST_UCSIMM)
		printf("[%s] : <TIME SHIFTING> real tm.: %d, shiftable tm.: %d \n", __FILE__,
		qfltRealTime.integer,	qfltRelTime.integer );
#else
#endif /* defined(FAST_UCSIMM) */
	}

#if defined(FAST_UCSIMM)
	/* Now they're equal or least 'relative tm' is not less than 'real tm' */
	printf("[%s] : <AFTER TIME SHIFTING> will pretend like <%d>, is same as <%d> \n", __FILE__,
		qfltRealTime.integer,
		qfltRelTime.integer );
#else
#endif /* defined(FAST_UCSIMM) */

		/* Put marquee 'secPRC: xxx;' on the screen, so we are sure platform is still not hanged */
		if (0 == qfltAbsTime.power)

			if (iOldSecPRC!= qfltAbsTime.integer)

				{iOldSecPRC=qfltAbsTime.integer; printf("secPRC: %d; ", iOldSecPRC); fflush(stdout); }

#if !defined(USB20)
		/* USB 1.0 levels. Logical '1'.  LOGIC_1_CURR */
		if ( iChkUsb10Lg1(qfltXval) )
#else		
		/* USB 2.0 levels. Logical '1'. LOGIC_1_CURR. 0.36V .. 0.44V */
		if ( iChkUsb20Lg1(qfltXval) )
#endif /* (!defined(USB20)) */
			PortD_Up( PD0 );

		else
#if !defined(USB20)			
			/* USB 1.0 levels. Logical '0'. LOGIC_0_CURR */
			if ( iChkUsb10Lg0(qfltXval) )

				PortD_Down( PD0 );
			else
				/* TODO: REWORK! A lot of logical zeroes will come with negative power of 10 (i.e. 'sgn' is '-'). */
				PortD_Down( PD0 );/* TODO: REWORK! */

			/* Attention: overvoltage, U = 3.6++ Volts will be processed as logical zero, too. */
#else		
			/* USB 2.0 levels. Logical '0'. LOGIC_0_CURR. -10 mV .. 10 mV */
			if ( iChkUsb20Lg0(qfltXval) )
	
				PortD_Down( PD0 );
			else
				; /* rest voltage levels: unprocessed */
#endif /* (!defined(USB20)) */

#if defined(DIN_FEEDBACK)
#if defined(USB20)
		if ( iChkUsb20Lg1(qfltYval) )
		{
			if ( 0 == PortD_CheckL1( PD1 ) )
			{	
				/* No 'logical 1' signal coincedence with USB 2.0 */
				pcMarq = calloc (1, strlen ("[2.0]") +1 );
				strcpy( pcMarq, "[2.0]");
			}
		}
		else
			if ( iChkUsb20Lg0(qfltYval) )
			{	
				if ( 1 == PortD_CheckL0( PD1 ) )
				{	
					/* No 'logical 0' signal coincedence with USB 2.0 */
					pcMarq = calloc (1, strlen ("[2.0]") +1 );
					strcpy( pcMarq, "[2.0]");
				}
			}

#endif /* (defined(USB20)) */
#endif /* (DIN_FEEDBACK) */

#endif /* !defined(QUASIFLOAT) */

}

/* Process data stored in dynamic structure pointed by 'pPointChainPar' */
int _ProcessPoints(const char * caller, pTimepointType pPointChainPar)
{
pTimepointType pPointChain = pPointChainPar;
float fltAbsTime;

double timeusePROC;

	/* Take initial time. Current time values will be taken in 'ProcRealAndRel()' */
	gettimeofday(&starttimePROC, 0);

	/* Process each entry of chain */
	while (NULL != pPointChain)
	{
#if DEBUG_DATA
#if !defined(QUASIFLOAT)
		printf("[%s] %s:%s : <%f> <%f> <%f> <%s> \n", __FILE__, caller, __func__,
			pPointChain->fltAbsTime,
			pPointChain->fltXval,
			pPointChain->fltYval,
			pPointChain->pcMarquee
		);
#else
		printf("[%s] %s:%s : <%d.%dE%c0%d> <%d.%dE%c0%d> <%d.%dE%c0%d> <%s> \n", __FILE__, caller, __func__,

			pPointChain->qfltAbsTime.integer,pPointChain->qfltAbsTime.fraction,
			pPointChain->qfltAbsTime.sgn,pPointChain->qfltAbsTime.power,

			pPointChain->qfltXval.integer,pPointChain->qfltXval.fraction,
			pPointChain->qfltXval.sgn,pPointChain->qfltXval.power,

			pPointChain->qfltYval.integer,pPointChain->qfltYval.fraction,
			pPointChain->qfltYval.sgn,pPointChain->qfltYval.power,

			pPointChain->pcMarquee
		);
#endif /* !defined(QUASIFLOAT) */
#endif /* (DEBUG_DATA) */


#if !defined(QUASIFLOAT)
		ProcRealAndRel(pPointChain->fltAbsTime, pPointChain->fltXval);
#else

#if defined(DIN_FEEDBACK)
		ProcRealAndRel(pPointChain->qfltAbsTime, pPointChain->qfltXval, pPointChain->qfltYval, pPointChain->pcMarquee , pPointChain->qfltAbsTime);
#else
		ProcRealAndRel(pPointChain->qfltAbsTime, pPointChain->qfltXval, pPointChain->qfltAbsTime);
#endif /* (defined(DIN_FEEDBACK)) */ 

#endif /* !defined(QUASIFLOAT) */

		/* Go to next record of chain */
		pPointChain = pPointChain->pNext;
	}

	return 0;
}

/* Free memory occupied by '*ppThisPointChain' */
void _DeletePoints(const char * caller, pTimepointType * ppThisPointChain)
{
pTimepointType pChild, pThisPointChain = *ppThisPointChain;

	/* Walk through entire list and delete each chain */
	while (NULL != pThisPointChain)
	{
		/* if space to keep item's name is allocated */
		if (pThisPointChain->pcMarquee)
		
		    /* then release this space */
		    free(pThisPointChain->pcMarquee);

		/* preserve a pointer to next record */		    
		pChild = pThisPointChain->pNext;
		
		/* free space occupied by current record */
		free (pThisPointChain);
		
		/* Go to next record */
		pThisPointChain = pChild;
	}

	/* Dispose first element of chain */
	*ppThisPointChain = NULL;
}
