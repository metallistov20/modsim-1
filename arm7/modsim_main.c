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

#include "clock.h"
#include "func.h"
#include "date_cnt.h"
#include "util.h"
#include "sort.h"
#include "modsim_main.h"
#include "ioat91sam7x256.h"

void msec5_isr (void);
void init_isr  (void);
void init_piob(void);

extern void _enableInterrupts(void);
extern void _disableInterrupts(void);

void output_LED(void);
void tc0_interrupt(void);

#define	TIMER0_INTERRUPT_LEVEL		1
#define MEMBUF_LEN  			2048u

static uint16_t	nv;
static uint16_t	ran;
static int	i;
static int	led_state;

static void never_exec(void)
{
	while (nv)
	{
		nv--;

		if (0 == nv)

		  nv--;
	}
}

static void random_wait(uint8_t num)
{
	uint16_t loop1;

	loop1 = (uint16_t) num * 10u;

	while (loop1-- > 0u)	;
}

/* Produce quasi-random numbers and call the 'random_wait' function */

static void random(void)
{
	uint8_t rn;

	ran = ran * 1111u;
	rn 	= (uint8_t) ran;
	ran = ran / 13u;
	rn  = (uint8_t) (ran % 7u);

	random_wait(rn);
}


int modsim_main(void)
{
	static   uint8_t     Mem[MEMBUF_LEN];
	static   uint8_t     *MemPtr;
	static   uint8_t     MemIdx;

	static   uint16_t    GlobalCounter;

	static   uint8_t     ValInt8;
	static   uint16_t    ValInt16;
	static   uint32_t    ValInt32;

	MemIdx         = 0u;
	GlobalCounter  = 0u;
	nv             = 0u;
	ran            = 12345u;

	/* initialize hardware componenents . configure PIOB for LEDs*/
	init_piob();

	/* initialize AIC and TC0 */
	init_isr();

	/* initialize Clock structure */
	init_clock();

	/* endless loop */
	while (1u != 0u)
	{
        ValInt32  = func(GlobalCounter);
    
		if (nv != 0u)
		{
			/* function will be never execute */
		  	never_exec();              /* function will be never execute */
		}
	      	if (GlobalCounter > 0u)
	      	{
		 	inc_date(GlobalCounter);   /* function which counts days, months   */
		    	                       /* and years. This increment is not     */
		        	                   /* related to the Clock increment       */
			lnk_lst();                 /* linked lists */
		 	analog_clock();
	      	}

		sort();

		random();

      	if ((GlobalCounter % 100u) == 0u)
      	{
        	if (MemIdx + TIME_LEN > MEMBUF_LEN)
         	{
            	MemIdx = 0u;
         	}
         	MemPtr   = &Mem[MemIdx];
         	copy_clock(MemPtr);
         	MemIdx   += TIME_LEN;
      	}

      	GlobalCounter--;

      	ValInt16 = get_clock();
      	ValInt8  = (uint8_t)ValInt16;
   }
}


/* initialize AIC and TC0 */

void init_isr(void)
{
    uint32_t	temp;

	/* Write in EOICR 8 times to make AIC empty */
	*AT91C_AIC_EOICR = 0;                        /* end interrupt               */
	*AT91C_AIC_EOICR = 0;                        /* end interrupt               */
	*AT91C_AIC_EOICR = 0;                        /* end interrupt               */ 
	*AT91C_AIC_EOICR = 0;                        /* end interrupt               */
	*AT91C_AIC_EOICR = 0;                        /* end interrupt               */
	*AT91C_AIC_EOICR = 0;                        /* end interrupt               */
	*AT91C_AIC_EOICR = 0;                        /* end interrupt               */
	*AT91C_AIC_EOICR = 0;                        /* end interrupt               */

    // First, enable the clock of the TIMER
	AT91C_BASE_PMC->PMC_PCER = 1<< AT91C_ID_TC0;

    // Disable the TC0 and the interrupts
	AT91C_BASE_TC0->TC_CCR = AT91C_TC_CLKDIS ;
	AT91C_BASE_TC0->TC_IDR = 0xFFFFFFFF ;

    //   Clear status bit
    temp = AT91C_BASE_TC0->TC_SR;

    // Set the Mode of the Timer Counter
	AT91C_BASE_TC0->TC_CCR = AT91C_TC_CLKDIS;	// disable timer
	AT91C_BASE_TC0->TC_CMR = AT91C_TC_CLKS_TIMER_DIV5_CLOCK |	// 47,9MHz/1024= 46800 Hz
							 AT91C_TC_CPCTRG;	// compare RC trigger
	AT91C_BASE_TC0->TC_RC = 234;				// 5ms

    // Enable the clock
	AT91C_BASE_TC0->TC_CCR = AT91C_TC_CLKEN ;

    // Initialize Timer 0 interrupt
	temp = 1 << AT91C_ID_TC0;
	AT91C_BASE_AIC->AIC_IDCR = temp;
	AT91C_BASE_AIC->AIC_SVR[AT91C_ID_TC0] = (uint32_t)tc0_interrupt;
	AT91C_BASE_AIC->AIC_SMR[AT91C_ID_TC0] = TIMER0_INTERRUPT_LEVEL | 
											AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL;
	AT91C_BASE_AIC->AIC_ICCR = temp;
	 
	AT91C_BASE_TC0->TC_IER = AT91C_TC_CPCS;  // enable IRQ on compare RC
	AT91C_BASE_AIC->AIC_IECR = temp;

	// Start Timer 0
	AT91C_BASE_TC0->TC_CCR = AT91C_TC_SWTRG;

    _enableInterrupts();					// enable interrupts
}



/* Initialize PIOB for LEDs */
void init_piob(void)
{
uint32_t  temp = *AT91C_RSTC_RSR;

	led_state = 0;

	/* Enable User Reset and set its minimal assertion to 960 us */
	temp = AT91C_RSTC_URSTEN    |   /* NRST triggers User Reset */
	(0x4 << 8)            |   /* 960µs */
	(unsigned int)(0xA5<<24);

	*AT91C_RSTC_RMR = temp;

	/* First, enable the clock of the PIOB */
	AT91C_BASE_PMC->PMC_PCER = 1<< AT91C_ID_PIOB;

	/* Enable PIOB pins 19-22 */
	*AT91C_PIOB_PER 	= 0x0F << 19;

	/* Set PIOB as output, pins 19-22 */
	*AT91C_PIOB_OER 	= 0x0F << 19;

	/* Clear the LED's. On the EK we must apply a "1" to turn off the LEDs  turn off all LED's */
	*AT91C_PIOB_SODR = 0x0F << 19;
}




/* Interrupt service for timer TC0 */
void tc0_interrupt(void)
{
	uint32_t temp;
	msec5_isr();
	i++;

	/* read TC0 status to enable timer interrupts again */
	temp = *AT91C_TC0_SR;	
}	

			 
void msec5_isr(void)
{
	static int led_count = 0;
	if (++led_count == 40)
	{
		/* 200ms blink frequnce */
		output_LED();

		led_count = 0;		
	}
//	 _disableInterrupts();
	inc_clock();                        /* clock increment  */
	convert_clock();                    /*   */
	signal_clock();                     /*   */
//	_enableInterrupts();
}

/* Table with LED values */
const unsigned long	LED_TAB[6] = { 0x1, 0x2, 0x4, 0x8, 0x4, 0x2 };

/* generate running light */
void output_LED(void)
{
	if (led_state > 5)
		led_state = 0;

	/* clear PB19-PB22 */
   	*AT91C_PIOB_SODR = 0xf << 19;

	*AT91C_PIOB_CODR = LED_TAB[led_state] << 19;

	led_state++;
}
