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
#include "../MCAL/I2C/i2c.h"
#include <xc.h>
#include <pic16f877a.h>

#define LEDPIN 0
#define ON_OFF_BTN          0
#define UP_BTN              1
#define DOWN_BTN            2
#define USED_PATTERN        (0x3D)
#define TEMP_VLAUES         10
#define SSD_FLASHING_CYCLE  20
#define TEMP_STEP            5
#define MAX_TEMP             (75)
#define MIN_TEMP            (35)
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
     /**
     * if the desired temp have been set 
     *      get average temp from last 10 readings
     *      control the cooler/heater element based on themp
     */
     
    if(gu8_TempControlFlag)
    {
        static uint8_t u8_counter   = 0;
        static uint8_t u8_heaterOn  = FALSE;
        static uint8_t u8_coolerOn  = FALSE;
        uint8_t u8_i = 0;
        uint16_t u8_averageTempValue = 0;
        /**
         * display the average of the avaliable readins 
         * before getting ten readings the average = average of the last x reading
         * where x is <=10
         */
        for(; u8_i< TEMP_VLAUES &&  tempValues[u8_i] ;u8_i++)
          u8_averageTempValue +=  tempValues[u8_i];

        u8_averageTempValue/=u8_i;
        
        
        /**
        *       if both heater and cooler off
        *          if temp blow critical turn on heater
        *          else _temp above critical_ turn on cooler
        *      else if heater on and temp above critical
        *          turn heater off and turn cooler on and turn led on
        *      else if cooler on and temp below critical
        *           turn cooler off and turn heater on
        */
        if(u8_heaterOn == FALSE && u8_coolerOn == FALSE)
        {
            if(u8_averageTempValue <= gu8_tempToAchive-TEMP_STEP)
            {
                DIO_Write(HEATER_ELEMENT,HIGHL);///turn on the heater
                u8_heaterOn = TRUE;
            }
            else
            {
                DIO_Write(COOLER_ELEMENT,HIGHL);///turn on the cooler
                u8_coolerOn = TRUE;
            }
        }
        else if(u8_heaterOn && u8_averageTempValue >= gu8_tempToAchive+TEMP_STEP)
        {
            //turn off heater and turn on cooler
            DIO_Write(HEATER_ELEMENT,LOWL);
            DIO_Write(COOLER_ELEMENT,HIGHL);
            
            DIO_Write(TEMP_CONTROL_LED,HIGHL);  

            
            u8_heaterOn = FALSE;
            u8_coolerOn = TRUE;
        }
        else if(u8_coolerOn && u8_averageTempValue <= gu8_tempToAchive-TEMP_STEP)
        {
            //turn on heater and turn off cooler
            DIO_Write(HEATER_ELEMENT,HIGHL);
            DIO_Write(COOLER_ELEMENT,LOWL);
            

            u8_heaterOn = TRUE;
            u8_coolerOn = FALSE;
        }
        
        /**
         * if the heater on toggle the led state every 1000 ms 
         * we enter the function every 100ms so the counter count to 10 times befor
         * toggle
         */
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
    
    /**
     * if on/off button pressed rise disable system flag
     * we check on this flag on the main loop and disable the system
     * if the flag is up
     */
    if(u8_onOffBtnStatus == BUT_ON)
        gu8_deviceDisableFlag = !gu8_deviceDisableFlag;
    
    
    /**
     * functionality of the button depend on the current system mode {NORM_MODE,TEMP_SET_MODE}
     * in normal mode: if the up or down buttons pressed 
     *                      get temp from the external e2Prom
     *                      change mode to TEMP_SET_MIDE
     * in TEMP_SET_MODE: if up or down buttons pressed 
     *                      change temp by 5 with max 75 and min 35 degree
     *                      
     */
    if(gu8_mode == NORM_MODE)
    {
        if(u8_upBtnStatus == BUT_ON || u8_downBtnStatus == BUT_ON)
        {
            gu8_mode = TEMP_SET_MODE;
            gu8_settedTemp  =  e2pext_r(0x00);
        }
        
    }
    else if(gu8_mode == TEMP_SET_MODE)
    {
        static uint8_t u8_btnPressCntr = 0;
        if(u8_upBtnStatus == BUT_ON)
        {
           gu8_settedTemp+=(gu8_settedTemp == MAX_TEMP)? 0 :TEMP_STEP;
        }
        else if(u8_downBtnStatus == BUT_ON)
        {
            gu8_settedTemp-=(gu8_settedTemp == MIN_TEMP)? 0 :TEMP_STEP;
        }
        else if(u8_downBtnStatus  == BUT_OFF || u8_upBtnStatus == BUT_OFF)
        {
            /*
             * count the time no button has been pressed to implement a time out
             * to exit TEMP_SET_MODE
             */
            u8_btnPressCntr++;
        }
        /*
         * if either up or down buttons pressed reset timeout counter
         */
        if(u8_downBtnStatus == BUT_ON || u8_upBtnStatus == BUT_ON
           || u8_upBtnStatus == BUT_PRSSED || u8_downBtnStatus == BUT_PRSSED)
            u8_btnPressCntr = 0;
        /**
         * if time out counter reached time out counts _button task called every 50ms
         * so we need 50000/50 = 100 enterance befor change the state to normal state_
         * 
         * load setted value to e2prom
         * set temp for temp control task
         * fire temp control task flag
         * reset timeout counter
         * change state to normal state
         */
        if(u8_btnPressCntr >= 100)
        {
            e2pext_w(0x00,gu8_settedTemp);
            gu8_tempToAchive = gu8_settedTemp;
            gu8_TempControlFlag = TRUE;
            u8_btnPressCntr = 0;
            gu8_mode = NORM_MODE;

        }   
            
    }
   
    
}


void sevenSegTask()
{        
    /**
     * two modes for lcd {NORMAL MODE , TEMP SET MODE}
     * if NORMAL_MODE:
     *      display current temp to the SSD
     * else:
     *      blink SSD every 1 sec 
     *      display the stored temp in EXT_EEPROM
     *      update temp based on user input up-down  
     *
     */
    if(gu8_mode == NORM_MODE)
    {
        static uint8_t u8_selectedSevenSeg = SEVEN_SEG_TWO;

        if(u8_selectedSevenSeg == SEVEN_SEG_TWO )
            sevenSegSendChar(currentTemp%10,SEVEN_SEG_TWO);
        else
            sevenSegSendChar(currentTemp/10,SEVEN_SEG_ONE);
   
        /*
         *  update u8_selectedSevenSeg variable to alternate between the two seven seg.
         */
        u8_selectedSevenSeg = (u8_selectedSevenSeg == SEVEN_SEG_ONE)? SEVEN_SEG_TWO : SEVEN_SEG_ONE;
  
    }
    else if( gu8_mode == TEMP_SET_MODE)
    {
        static uint8_t u8_ssdDisableFalg = FALSE;
        static uint8_t u8_flashCounter = 0;
        static uint8_t u8_selectedSevenSeg = SEVEN_SEG_TWO;
        if(u8_selectedSevenSeg == SEVEN_SEG_TWO)
            sevenSegSendChar(gu8_settedTemp%10,SEVEN_SEG_TWO);
        else
            sevenSegSendChar(gu8_settedTemp/10,SEVEN_SEG_ONE);
   
         /*
         *  update u8_selectedSevenSeg variable to alternate between the two seven seg.
         */
        u8_selectedSevenSeg = (u8_selectedSevenSeg == SEVEN_SEG_ONE)? SEVEN_SEG_TWO : SEVEN_SEG_ONE;
        
        /**
         * control the ssd on/off time as 1 sec for each
         */
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
    float64_t clsius;
    uint16_t u16_ADC_value = 0;
    
    /**
     * trigger ADC on specific chanal
     * get temp from adc value
     * set adc value in it's pos in temp array
     */
    ADC_trigger(TEMP_ADC_CHNL);
    getADC_value(&u16_ADC_value);
    //this eq from the data sheet of the LM25 module
    clsius = (((float64_t)u16_ADC_value*4.88)/(10.00));
    currentTemp = (uint32_t)clsius; //this is for SSD 
    tempValues[u8_TempPos] = (uint32_t)clsius; //this is for temp control task
    u8_TempPos++;
    
    if(u8_TempPos == TEMP_VLAUES) //reach array boundry
        u8_TempPos = 0;
}

void appInit()
{
    
    ///init temp control element to zero
    /**
     * call MCAL functions from application as it is necessary to 
     * initialize the heater/cooler and led 
     * and there is no module implemented for them case their functionality
     * are very simple and don't need to be structured necessary overhead_
     */
    DIO_Init();
    DIO_Write(HEATER_ELEMENT,LOWL);
    DIO_Write(COOLER_ELEMENT,LOWL);
    //init temp control LED AS 0
    DIO_Write(TEMP_CONTROL_LED,LOWL);  
    BTN_Init();
    interruptsInit();
    sevenSegInit();
    disableSevenSeg();
    ADC_Init();
    i2c_init();
    SOS_Init();
    uint8_t u8_i = 0;
    for(; u8_i< TEMP_VLAUES ;u8_i++)
        tempValues[u8_i] = 0;
    
    /**
     * this block of code used to set the temp to 60 
     * in the first run
     * this can be simplified by checking for 0xff pattern
     * but this is work for now
     */
    uint8_t u8_usedPattern = e2pext_r(0x02);
    
    if(u8_usedPattern != USED_PATTERN)
    {
        e2pext_w(0x00,60);
        e2pext_w(0x02,USED_PATTERN);
    }
    ///set initial values for global variables
    gu8_mode = NORM_MODE;
    currentTemp   = 0;
    gu8_settedTemp = 60;
    gu8_TempControlFlag = 0 ;
    gu8_deviceDisableFlag = TRUE;   
}


void checkONBtnStatus()
{
    /*
     *  this function used to check on the on/off btn 
     * if the system on the off state
     */
    BtnStateType u8_onOffBtnStatus;
    BTN_Manager();
    BTN_GetState(&u8_onOffBtnStatus,ON_OFF_BTN);
    
    if(u8_onOffBtnStatus == BUT_ON)
        gu8_deviceDisableFlag = !gu8_deviceDisableFlag;
}

void OS_startFunction()
{
    tempTask();
}

void main(void){
    appInit();
    /**
     * task priority is set by thier order of call not thier
     * priority number as the priority feature is off 
     * from the small os.
     */
    SOS_createTask(1,tempTask,100,PERIODIC,1);
    SOS_createTask(3,buttonTask,50,PERIODIC,2);
    SOS_createTask(4,tempControlTask,100,PERIODIC,3);
    SOS_createTask(2,sevenSegTask,50,PERIODIC,4);
    
    //set the CBF to call at the start of the os
    SOS_StartProc(OS_startFunction);
    while(1)
    {
        /**
         * if the on off btn is pressed and the system is on disable system
         * disable system by call init function of the system 
         */
       
        if(gu8_deviceDisableFlag == TRUE)
        {
            appInit();
            
            /**
             * halt system and only check for the
             * on btn 
             */
            while(gu8_deviceDisableFlag == TRUE)
            {
                checkONBtnStatus();
            }
        }
        
        SOS_run();
       

    }
    
    return;
}