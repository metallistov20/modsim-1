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

#include <stdio.h>

#if defined(UCSIMM)

/* Platform definitions */
#include <asm/MC68EZ328.h>

#endif /* (UCSIMM) */

#if defined (ARM7_TDMI)
/* On platform(s) usleep() declared in <unistd.h>, on NEWLIB-based baremetal(s) we do it ourselves */
int usleep()
{
volatile int iTmoCritVal = 10;

	do { ; } while (iTmoCritVal--);
	
}
#endif /* (ARM7_TDMI) */


/* Own interface, definitions */
#include "dport.h"

/* Prepare Port 'D' */
void PortD_Prepare()
{
#if defined(UCSIMM)

#if !defined(DIN_FEEDBACK)
	PDSEL = PD0 | PD1 | PD2 | PD3;

	PDDIR = PD0 | PD1 | PD2 | PD3;

	printf ("Bits <%08bb> <%08bb> of Port D initialized as OUT\n", (unsigned char)PD0, (unsigned char)PD1);
#else

#if 0
    	SPIMCONT = 0x0277;  // SPIMCONT = PUT_FIELD(SPIMCONT_DATA_RATE, 2) | SPIMCONT_PHA | PUTFIELD(SPIMCONT_BIT_COUNT, 15);
    	SPIMCONT = PUT_FIELD(SPIMCONT_DATA_RATE, 2) | SPIMCONT_PHA | PUTFIELD(SPIMCONT_BIT_COUNT, 15);
#else
    	SPIMCONT = 0x402f;
    	SPIMCONT |= SPIMCONT_ENABLE;

	printf ("Port D : bit <%08bb> OUT, bit <%08bb> IN \n", (unsigned char)PD0, (unsigned char)PD1);
#endif /* (0) */ 

#endif /* (!defined(DIN_FEEDBACK)) */

#endif /* (UCSIMM) */
}

/* Toggle bits defined by bitmask 'uchBit' in Port D*/
void PortD_Toggle(unsigned char uchBit)
{
#if defined(UCSIMM)
	PDDATA ^= uchBit;
#endif /* (UCSIMM) */
}

/* Switch off bits defined by bitmask 'uchBit' in Port D*/
void PortD_Down(unsigned char uchBit)
{
#if defined(UCSIMM)
	PDDATA &= ~uchBit;
#endif /* (UCSIMM) */
}

/* Switch on bits defined by bitmask 'uchBit' in Port D*/
void PortD_Up(unsigned char uchBit)
{
#if defined(UCSIMM)
	PDDATA |= uchBit;
#endif /* (UCSIMM) */
}

/* Port D probe routine, two bits are enough for current task */
void PortD_Probe( )
{
#if defined(UCSIMM)
	PortD_Prepare( );

	PortD_Toggle(  PD0 );

	while (1)
	{
		PortD_Toggle(  PD1 | PD0 );

		/* Dubious */
		usleep (10);
	}
#endif /* (UCSIMM) */
}

#if defined(DIN_FEEDBACK)

/* Return contents of Port D (as integer) as its data ready */
unsigned char PortD_Read(unsigned char uchBit)
{
	while (!(SPIMCONT & SPIMCONT_IRQ))

		usleep(1);

	return (PDDATA & uchBit) ;

}

/* Check if bit(s) defined by 'uchBit' of Port E is zero */
int PortD_CheckL0( unsigned char uchBit )
{
	return (0 == PortD_Read (~uchBit) );
}

/* Check if bit(s) defined by 'uchBit' of Port E is one */
int PortD_CheckL1( unsigned char uchBit )
{
	return (0 < PortD_Read (uchBit) );
}

#endif /* (defined(DIN_FEEDBACK)) */ 
