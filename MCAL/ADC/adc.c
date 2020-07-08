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
#include "adc_Cfg.h"
#define ADC_IDLE_STATE       0
#define ADC_INIT_STATE       1
#define MAX_SUPPORTED_CHANAL 8

#define ADC_CHANAL_SHIFT     (3)
#define ADC_CHANAL_MASK      (0x38)
#define ADC_ON_BIT_MASK      (0X01)
uint16_t gu8_ADC_ADCValue;
uint8_t gu8_ADC_State =ADC_IDLE_STATE ;



ERROR_STATE ADC_Init(void)
{
    /*check
     *  if the module already initlaized
     *      then change function state to NOK  and report error
     *  else
     *      load ADCON1 AND ADCON0 WITH RESPECTIVE SETTING VALUES
     */
    ERROR_STATE e_funStatus = OK;
    if(gu8_ADC_ADCValue == ADC_INIT_STATE)
    {
      e_funStatus = NOK;  
      error_handler(ADC_MODULE_ERROR_NUM+MULTIPLE_INITALIZATION);
    }
    else
    {
        ADCON0 = gstr_ADC_Config.ADC_CNTRL_0;
        ADCON1 = gstr_ADC_Config.ADC_CNTRL_1;
        
        gu8_ADC_ADCValue = 0;
        gu8_ADC_State = ADC_INIT_STATE;
    }
    return e_funStatus;
}


ERROR_STATE ADC_UpdateValue(uint8_t u8_canal)
{
    /*check
     *  if the module is not  initlaized
     *      then change function state to NOK  and report error
     *  else if not supported chanal number 
     *      then change function state to NOK  and report error
     *  else
     *      check if the prevoius reading is done 
     *              then trigger another conversion for the selected chanal
     *      load ADCON0 WITH RESPECTIVE SETTING VALUES to the selected chanal
     */
    ERROR_STATE e_funStatus = OK;
    
    if(gu8_ADC_State == ADC_IDLE_STATE)
    {
        e_funStatus = NOK; 
        error_handler(ADC_MODULE_ERROR_NUM+MODULE_NOT_INITALIZED);
    }
    else if(u8_canal > MAX_SUPPORTED_CHANAL)
    {
        e_funStatus = NOK; 
        error_handler(ADC_MODULE_ERROR_NUM+INVALAD_PARAMETER);
    }
    else
    {
         if(ADCON0bits.GO == 0)
        {
            gu8_ADC_ADCValue = ((((uint8_t)ADRESH)<<2)|(ADRESL>>6));
            
            ADCON0 |= ((u8_canal<<ADC_CHANAL_SHIFT)&(ADC_CHANAL_MASK))|(ADC_ON_BIT_MASK);
            ADCON0bits.GO=1;
        }
    }
    
    
    return e_funStatus;
}
