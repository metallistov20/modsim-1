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

#ifndef _CLOCK_H_
#define _CLOCK_H_

void init_clock(void);

//global void inc_clock(void);
void inc_clock(void);

//global void signal_clock(void);
void signal_clock(void);

//global void convert_clock(void);
void convert_clock(void);

//global void analog_clock(void);
void analog_clock(void);

//global void copy_clock(uint8_t *Buf);
void copy_clock(uint8_t *Buf);

//global uint16_t get_clock(void);
uint16_t get_clock(void);

#endif /* _CLOCK_H_ */
