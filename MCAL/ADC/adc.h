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

#include "../../ServiceLayer/std_types.h"
#include "../../ServiceLayer/Error_Handler/SystemErrors.h"

extern uint16_t gu8_ADC_ADCValue;

typedef struct 
{
	uint8_t ADC_CNTRL_0; //CONFIGRATION VALUE OF ADC0N0 REGISTER
	uint8_t ADC_CNTRL_1; //CONFIGRATION VALUE OF ADC0N1 REGISTER

}gstr_ADC_ConfigParam_t;


/**********************************************************************
* Function : ADC_Init()
*//**
* \b Description:
*
* This function is used to initialize the adc based on the configuration
*  in adc_cfg module.
*
* PRE-CONDITION: Module must be idle _not initalized befor_,  DIO must be intalized 
*               
*
* POST-CONDITION: 
* 
*
* @return ERROR_STATE{OK,NOK}.
*
* \b Example Example:
* @code
*
* ADC_Init();
* @endcode
*
* @see Dio_Init
**********************************************************************/
ERROR_STATE ADC_Init(void);


/**********************************************************************
* Function : ADC_UpdateValue(uint8_t u8_cana)
*//**
* \b Description:
*
* This function is used to trigger adc on specified chanal
*  
*
* PRE-CONDITION: Module must be initalized,  DIO must be intalized 
*               
*
* POST-CONDITION: when converstion done gu8_ADC_State upadate with the conversion value 
* 
*
* @return ERROR_STATE{OK,NOK}.
*
* \b Example Example:
* @code
*
* ADC_UpdateValue(2);
* @endcode
*
* @see Dio_Init
* @see ADC_Init
**********************************************************************/
ERROR_STATE ADC_UpdateValue(uint8_t u8_canal);
