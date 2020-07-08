/*
 * File:   main.c
 * Author: Mahmoud Abdelbary
 *
 * Created on June 29, 2020, 10:51 AM
 */
#include "../MCAL/Registers.h"
#include "../ServiceLayer/std_types.h"
#include "../ServiceLayer/Config.h"
#include "../HAL/7SEG/7seg.h"
#include "../MCAL/DIO/DIO.h"
#include "../HAL/7SEG/unitTest/7seg_test.h"
#include "../HAL/BTN/unit test/btn_test.h"
#include "../HAL/BTN/btn.h"
#include "../HAL/eeprom_ext/eeprom_ext.h"
#include "../MCAL/ADC/adc.h"
#include "../MCAL/ISR/isr.h"
#include "../MCAL/TIMER/timer.h"
#include "../ServiceLayer/SOS/SOS.h"


#include <xc.h>

#define LEDPIN 0
#define ON_OFF_BTN     0
#define UP_BTN         1
#define DOWN_BTN       2
#define USED_PATTERN  (0x3D)
#define TEMP_VLAUES   10
#define SSD_FLASHING_CYCLE  20
typedef enum{TEMP_SET_MODE,NORM_MODE}operationMode_t;


static uint8_t tempValues[TEMP_VLAUES];
static uint8_t currentTemp;
static operationMode_t gu8_mode;
static uint8_t gu8_settedTemp; 
static uint8_t gu8_TempControlFlag;
static uint8_t gu8_tempToAchive;
static uint8_t gu8_deviceDisableFlag;



void tempControlTask()
{
    if(gu8_TempControlFlag)
    {
        static uint8_t u8_counter   = 0;
        static uint8_t u8_heaterOn  = FALSE;
        static uint8_t u8_coolerOn  = FALSE;
        uint8_t u8_i = 0;
        uint16_t u8_averageTempValue = 0;

        for(; u8_i< TEMP_VLAUES ;u8_i++)
          u8_averageTempValue +=  tempValues[u8_i];

        u8_averageTempValue/=TEMP_VLAUES;
        
        if(u8_heaterOn == FALSE && u8_coolerOn == FALSE)
        {
            if(u8_averageTempValue <= gu8_tempToAchive-5)
            {
                DIO_Write(HEATER_ELEMENT,HIGHL);///turn on the heater
                u8_heaterOn = TRUE;
            }
            else
            {
                DIO_Write(COOLER_ELEMENT,HIGHL);///turn on the heater
                u8_coolerOn = TRUE;
            }
            
            
           
        }
        else if(u8_heaterOn && u8_averageTempValue >= gu8_tempToAchive+5)
        {
            //turn off heater and turn on cooler
            DIO_Write(HEATER_ELEMENT,LOWL);
            DIO_Write(COOLER_ELEMENT,HIGHL);
            
            DIO_Write(TEMP_CONTROL_LED,HIGHL);  

            
            u8_heaterOn = FALSE;
            u8_coolerOn = TRUE;
        }
        else if(u8_coolerOn && u8_averageTempValue <= gu8_tempToAchive-5)
        {
            //turn on heater and turn off cooler
            DIO_Write(HEATER_ELEMENT,HIGHL);
            DIO_Write(COOLER_ELEMENT,LOWL);
            

            u8_heaterOn = TRUE;
            u8_coolerOn = FALSE;
        }
        
        if(u8_heaterOn)
        {
            u8_counter++;
            if(u8_counter == 10)
            {
                DIO_Toggle(TEMP_CONTROL_LED); 
                u8_counter = 0;
            }
        }
        
    }
  
}

void buttonTask()
{
    BtnStateType u8_onOffBtnStatus;
    BtnStateType u8_upBtnStatus;
    BtnStateType u8_downBtnStatus;
    BTN_Manager();
    BTN_GetState(&u8_onOffBtnStatus,ON_OFF_BTN);
    BTN_GetState(&u8_upBtnStatus,UP_BTN);
    BTN_GetState(&u8_downBtnStatus,DOWN_BTN);

    if(u8_onOffBtnStatus == BUT_ON)
        gu8_deviceDisableFlag = !gu8_deviceDisableFlag;
    
    
    if(gu8_mode == NORM_MODE)
    {
        if(u8_upBtnStatus == BUT_ON || u8_downBtnStatus == BUT_ON)
        {
            gu8_mode = TEMP_SET_MODE;
            gu8_settedTemp  =  eeprom_read(0x00);
        }
        
    }
    else if(gu8_mode == TEMP_SET_MODE)
    {
        static uint8_t u8_btnPressCntr = 0;
        if(u8_upBtnStatus == BUT_ON)
        {
           gu8_settedTemp+=(gu8_settedTemp == 75)? 0 :5;
        }
        else if(u8_downBtnStatus == BUT_ON)
        {
            gu8_settedTemp-=(gu8_settedTemp == 35)? 0 :5;
        }
        else if(u8_downBtnStatus  == BUT_OFF || u8_upBtnStatus == BUT_OFF)
        {
            u8_btnPressCntr++;
        }
        
        if(u8_downBtnStatus == BUT_ON || u8_upBtnStatus == BUT_ON
           || u8_upBtnStatus == BUT_PRSSED || u8_downBtnStatus == BUT_PRSSED)
            u8_btnPressCntr = 0;
        
        if(u8_btnPressCntr >= 100)
        {
            eeprom_write(0x00,gu8_settedTemp);
            gu8_tempToAchive = gu8_settedTemp;
            gu8_TempControlFlag = TRUE;
            u8_btnPressCntr = 0;
            gu8_mode = NORM_MODE;

        }   
            
    }
   
    
}


void sevenSegTask()
{
    if(gu8_deviceDisableFlag)
    {
         disableSevenSeg();
         return;
    }
           
    if(gu8_mode == NORM_MODE)
    {
        static uint8_t u8_sevenSegFlag = 0;

        if(u8_sevenSegFlag&1)
            sevenSegSendChar(currentTemp%10,SEVEN_SEG_TWO);
        else
            sevenSegSendChar(currentTemp/10,SEVEN_SEG_ONE);
   
        u8_sevenSegFlag++;
        if(u8_sevenSegFlag == 2)
           u8_sevenSegFlag = 0;
    }
    else if( gu8_mode == TEMP_SET_MODE)
    {
        static uint8_t u8_ssdDisableFalg = FALSE;
        static uint8_t u8_flashCounter = 0;
        static uint8_t u8_sevenSegFlag = 0;
        if(u8_sevenSegFlag&1)
            sevenSegSendChar(gu8_settedTemp%10,SEVEN_SEG_TWO);
        else
            sevenSegSendChar(gu8_settedTemp/10,SEVEN_SEG_ONE);
   
        u8_sevenSegFlag++;
        if(u8_sevenSegFlag == 2)
           u8_sevenSegFlag = 0; 
        
        if(u8_flashCounter == SSD_FLASHING_CYCLE)
        {
            u8_ssdDisableFalg = !u8_ssdDisableFalg;
            u8_flashCounter   =  0 ;
        }
        
        if(u8_ssdDisableFalg)
            disableSevenSeg();
        
        
        u8_flashCounter++;
    }   
     
}

void tempTask()
{
    static uint8_t u8_TempPos = 0;
    
    ADC_UpdateValue(TEMP_ADC_CHNL);
    double clsius;
    clsius = (((gu8_ADC_ADCValue)*4.88)/(10.00));
    currentTemp = (int)clsius;
    tempValues[u8_TempPos] = (int)clsius;
    u8_TempPos++;
    
    if(u8_TempPos == TEMP_VLAUES) //reach array boundry
        u8_TempPos = 0;
}

void appInit()
{
    CMCON = 0x07;
    DIO_Init();
    ///init temp control element to zero
    DIO_Write(HEATER_ELEMENT,LOWL);
    DIO_Write(COOLER_ELEMENT,LOWL);
    //ONOT LED AS 0
    DIO_Write(TEMP_CONTROL_LED,LOWL);  
    BTN_Init();
    interruptsInit();
    sevenSegInit();
    disableSevenSeg();
    ADC_Init();
    SOS_Init();
    uint8_t u8_i = 0;
    for(; u8_i< TEMP_VLAUES ;u8_i++)
        tempValues[u8_i] = 0;
    
    uint8_t u8_usedPattern = eeprom_read(0x02);
    
    if(u8_usedPattern != USED_PATTERN)
    {
        eeprom_write(0x00,60);
        eeprom_write(0x02,USED_PATTERN);
    }
    gu8_mode = NORM_MODE;
    currentTemp   = 0;
    gu8_settedTemp = 60;
    gu8_TempControlFlag = 0 ;
    gu8_deviceDisableFlag = TRUE;
    
 
   
}
void checkONBtnStatus()
{
    BtnStateType u8_onOffBtnStatus;

    BTN_Manager();
    BTN_GetState(&u8_onOffBtnStatus,ON_OFF_BTN);

    if(u8_onOffBtnStatus == BUT_ON)
        gu8_deviceDisableFlag = !gu8_deviceDisableFlag;
}

void main(void) {
    //btnSimulationTest();
    /*
    int v = 5;
    eeprom_write(0x00,v);
    DIO_Init();
    if(v == eeprom_read(0x00))
    {
        DIO_Write(6,HIGHL);
        while(1);
    }
     */
    appInit();
    SOS_createTask(1,tempTask,100,PERIODIC);
    SOS_createTask(2,sevenSegTask,50,PERIODIC);
    SOS_createTask(3,buttonTask,50,PERIODIC);
    SOS_createTask(4,tempControlTask,100,PERIODIC);

    while(1)
    {
        if(gu8_deviceDisableFlag == TRUE)
        {
            appInit();
            while(gu8_deviceDisableFlag == TRUE)
            {
                checkONBtnStatus();
            }
        }
      
            SOS_run();

    }
    
    return;
}
