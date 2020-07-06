/* ########################################################################

   PICsim - PIC simulator http://sourceforge.net/projects/picsim/

   ########################################################################

   Copyright (c) : 2015  Luis Claudio Gambôa Lopes

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   For e-mail suggestions :  lcgamboa@yahoo.com
   ######################################################################## */

#include <xc.h>
#include "adc.h"

uint16_t gu8_ADC_ADCValue;
void adc_init(void)
{
  ADCON1=0x02;
  ADCON0=0x41; 
  gu8_ADC_ADCValue = 0;
}

void adc_amostra(unsigned char canal)
{
    if(ADCON0bits.GO == 0)
    {
        gu8_ADC_ADCValue = ((((unsigned int)ADRESH)<<2)|(ADRESL>>6));
        switch(canal)
        {
         case 0:
        ADCON0=0x01;
        break;
        case 1:
        ADCON0=0x09;
        break;
         case 2:
        ADCON0=0x11;
        break;
        }      
        ADCON0bits.GO=1;
    }
}
