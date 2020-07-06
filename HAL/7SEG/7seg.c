/*
 * File:   DIO.c
 * Author: mahmo
 *
 * Created on June 29, 2020, 4:21 PM
 */
#include "7seg.h"
#include "7seg_Cfg.h"



//MOUDLE ERROR NUM -101

#define DIO_NUM_OF_GROUPS                   3
#define SEVENSEG_MODULE_UNINTALIZED         0
#define SEVENSEG_MODULE_INIT                1
#define NUM_OF_DISP_VALUES                  10
#define IS_VISIABLE_VALUE(x)                (x < 10)                  
#define SINGLE_DIGIT_VALUE(x)               (x<10)
#define IS_VALID_DISPLAY(x)                 (x == SEVEN_SEG_ONE || x == SEVEN_SEG_TWO)
static uint8_t gu8_7seg_moduleState = SEVENSEG_MODULE_UNINTALIZED;

//this array provide values correspond to each digit shap on the SSD.
const uint8_t gcau8_sevenSegment_valueTable[NUM_OF_DISP_VALUES]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

ERROR_STATE sevenSegInit(void)
{
    ERROR_STATE u8_funState = OK;
    
    ///SET SEVEN SEG DISPLAY PORT TO 0
    DIO_Write(sevenSegConfigParam[SEVEN_SEG_ONE].u8_selectorPinGroupId,HIGHL);
    ///MODULE STATE INIT
    gu8_7seg_moduleState = SEVENSEG_MODULE_INIT;
    return    u8_funState;
}

static void selectSevenSeg(const uint8_t u8_selec)
{

    if(u8_selec == SEVEN_SEG_ONE)
    {
       DIO_Write(sevenSegConfigParam[SEVEN_SEG_ONE].u8_selectorPinGroupId,HIGHL);
       DIO_Write(sevenSegConfigParam[SEVEN_SEG_TWO].u8_selectorPinGroupId,LOWL);
    }
    else
    {
       DIO_Write(sevenSegConfigParam[SEVEN_SEG_ONE].u8_selectorPinGroupId,LOWL);
       DIO_Write(sevenSegConfigParam[SEVEN_SEG_TWO].u8_selectorPinGroupId,HIGHL);
    }
}

ERROR_STATE disableSevenSeg()
{
    DIO_Write(sevenSegConfigParam[SEVEN_SEG_ONE].u8_selectorPinGroupId,LOWL);
    DIO_Write(sevenSegConfigParam[SEVEN_SEG_TWO].u8_selectorPinGroupId,LOWL);
}

ERROR_STATE sevenSegSendChar(const uint8_t u8_value,const uint8_t u8_SSD_selector)
{
    ERROR_STATE u8_funState = OK;
    if(gu8_7seg_moduleState == SEVENSEG_MODULE_UNINTALIZED)
    {
        //report error module uninit
        u8_funState = NOK;
    }
    else if(!IS_VISIABLE_VALUE(u8_value) || !IS_VALID_DISPLAY(u8_SSD_selector) ) //check for invalid paramter
    {
        //report invalid parameter
        u8_funState = NOK;
    }
    else //no problem
    {
        //select seven segment 
        //write value 
        selectSevenSeg(u8_SSD_selector);
        DIO_Write(sevenSegConfigParam[u8_SSD_selector].u8_sevenSegGroupId
                ,gcau8_sevenSegment_valueTable[(u8_value)]);
    }
    return u8_funState;
}
