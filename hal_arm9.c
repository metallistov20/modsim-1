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

/* Process Realtime and Relative-time values: certain data to be oputput onto Port D */
int ProcRealAndRel(
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
	)
{
QuasiFloatType qfltRelTime;
int _left, _right;
QuasiFloatType qfltJiffy; 
qfltJiffy.fraction = 1;


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


		/* USB 1.0 levels. Logical '1'.  LOGIC_1_CURR */
		if ( iChkUsb10Lg1(qfltXval) )

			PortD_Up( PD0 );

		else
			/* USB 1.0 levels. Logical '0'. LOGIC_0_CURR */
			if ( iChkUsb10Lg0(qfltXval) )

				PortD_Down( PD0 );
			else
				/* TODO: REWORK! A lot of logical zeroes will come with negative power of 10 (i.e. 'sgn' is '-'). */
				PortD_Down( PD0 );/* TODO: REWORK! */

			/* Attention: overvoltage, U = 3.6++ Volts will be processed as logical zero, too. */

#if defined(DIN_FEEDBACK)

	;

#endif /* (DIN_FEEDBACK) */

#endif /* !defined(QUASIFLOAT) */

}
