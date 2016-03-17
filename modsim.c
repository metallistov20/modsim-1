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

/* stdout, NULL */
#include <stdio.h>

/* Own interface, definitions */
#include "modsim.h"

/* Data structures, mocros */
#include "datastruct.h"

/* Port D definitions and prototypes */
#include "port_d.h"

/* Pointer to raw data file */
FILE *fp = NULL;

/* Bufer to store scanned raw data */
char cBuf [LARGE_BUF_SZ];

#if !defined(QUASIFLOAT) 
	float fltTM, fltDIn, fltDOut;
#else
	QuasiFloatType qfltTM, qfltDIn, qfltDOut;
#endif /* !defined(QUASIFLOAT) */

/* Pointer to a dynamic structure to store raw data to */
pTimepointType pTimeChain;

/* Aux. var to tell whether it's a first time we output a message for this 'second'  */
int iOldSec;

int main ()
{
#if defined(UCSIMM) //TODO: remove this HW testing workaround.
PortD_Prepare( );
while (1) { PortD_Toggle(0xF0 ); usleep(10) ; }
#endif /* (defined(UCSIMM) ) */


	/* Try to open Raw Data file at place defined by 'FILE_NAME' */
	if ( NULL == (fp = fopen (FILE_NAME, "r") ) )
	{
		printf("[%s] %s: can't open file <%s> \n", __FILE__, __func__ , FILE_NAME);

		return P_ERROR;
	}

	printf("[%s] %s: loading USB-curve-data via NFS from file <%s>\n", __FILE__, __func__, FILE_NAME);

	/* For each string of Raw Data file */
	while ( ! (feof (fp) ) ) 
	{
		/* Try to scan a whole string into temp. buffer */
		if (0 > fscanf (fp, "%s", cBuf ) )
		{
			// eof reached
		}
		else
		{
		/* Aux. buffer to keep results of parsing */
		char * cpTmp = cBuf;
#if DEBUG_DATA
			printf("[%s] %s: scanned: < %s >\n", __FILE__, __func__, cBuf);
#endif /* (DEBUG_DATA) */

#if !defined(QUASIFLOAT) 
			/* Set default values. MISRA RULE #TODO */
			fltTM = fltDIn = fltDOut = 0.0f;
#else
			/* Set default values. MISRA RULE #TODO */
			memset (&qfltTM, 0, sizeof (QuasiFloatType) ) ;
			memset (&qfltDIn, 0, sizeof (QuasiFloatType) ) ;
			memset (&qfltDOut, 0, sizeof (QuasiFloatType) ) ;

#endif /* !defined(QUASIFLOAT) */

			/* For each character in aux. buffer */
			while (*cpTmp)

				/* replace all commas with spaces, to let the <scanf()> parse it */
				{ if (',' == *cpTmp) *cpTmp = ' '; cpTmp++; }
	

#if !defined(QUASIFLOAT) 
			/* Find 3 floats separated by spaces in aux. buffer */
			sscanf(cBuf, "%f %f %f,", &fltTM,     &fltDIn,   &fltDOut );
#else
			/* Find 3 floats separated by spaces in aux. buffer. Each float represented as <INT>.<INT>e<SIGN>0<INT> */
			sscanf(cBuf, "%d.%de%c0%d %d.%de%c0%d %d.%de%c0%d,",
					&(qfltTM.integer),&(qfltTM.fraction),&(qfltTM.sgn),&(qfltTM.power),
					&(qfltDIn.integer),&(qfltDIn.fraction),&(qfltDIn.sgn),&(qfltDIn.power),
					&(qfltDOut.integer),&(qfltDOut.fraction),&(qfltDOut.sgn),&(qfltDOut.power)  );

/* For each 'second' value do output. Only once. */ /* TODO: remove? */
if (0 == qfltTM.power) if (iOldSec!= qfltTM.integer){iOldSec=qfltTM.integer; printf("sec: %d; ", iOldSec); fflush(stdout); }

#endif /* !defined(QUASIFLOAT) */

#if DEBUG_DATA
#if !defined(QUASIFLOAT) 
			printf("[%s] %s: parsed :  <%f> <%f> <%f>\n", __FILE__, __func__, fltTM, fltDIn, fltDOut );
#else
			printf("[%s] %s: parsed :  <%d.%de%c0%d>  <%d.%de%c0%d>  <%d.%de%c0%d> \n",
					__FILE__, __func__, 
					qfltTM.integer,qfltTM.fraction,qfltTM.sgn,qfltTM.power,
					qfltDIn.integer,qfltDIn.fraction,qfltDIn.sgn,qfltDIn.power,
					qfltDOut.integer,qfltDOut.fraction,qfltDOut.sgn,qfltDOut.power  );
#endif /* !defined(QUASIFLOAT) */
#endif /* (DEBUG_DATA) */

#if !defined(QUASIFLOAT) 
			/* Attach just scanned data (three floats) to tail of dynamic structure */
			EnrollPoint(&pTimeChain, &fltTM, &fltDIn, &fltDOut, "N/A");
#else
			/* Attach just scanned data (three floats represented as <INT>.<INT>e<SIGN>0<INT>) to tail of dynamic structure */
			EnrollPoint(&pTimeChain, &qfltTM, &qfltDIn, &qfltDOut, "N/A");
#endif /* !defined(QUASIFLOAT) */
		}
	}

	/* Dispose pointer to Raw Data file */
	fclose(fp);

	printf("\n[%s] %s: issuing USB-curve-data on Pin #0 Port 'D'\n", __FILE__, __func__);

#if defined(UCSIMM) 
	/* Set pins of Port D as inputs/outputs, sets rest platform registers */
	PortD_Prepare( );
#endif /* (defined(UCSIMM) ) */

	/* Process data stored in dynamic structure pointed by 'pTimeChain' */
	ProcessPoints(pTimeChain);

	printf("\n[%s] %s: disposing memory allocations\n", __FILE__, __func__);

	/* Free memory occupied by dynamically stored raw data */
	DeletePoints(&pTimeChain);

	printf("[%s] %s: done (success) \n", __FILE__, __func__); fflush(stdout);

	return P_SUCCESS;
}

/* 17-03-2016. TODO: Implement these UCLIBC-/GLIBC-functions  */
_fopen:

_fscanf:

_calloc:

_memcpy:

_strlen:

_calloc:

_strcpy:

_printf:

_free:

