/*
 * File:   timer.c
 * Author: mahmo
 *
 * Created on July 5, 2020, 11:57 AM
 */


#include <xc.h>
#include "timer.h"
#define TIMER1_MAX_TICKS  (0xF63Bu)
#define SYSTEM_TIMER_IDLE_STATE     0
#define SYSTEM_TIMER_INITALIZED     1  

static uint8_t gu8_systemTimerModuleState = SYSTEM_TIMER_IDLE_STATE;    
volatile uint8_t gu8_timer_ticks;


ERROR_STATE system_timer_init(void)
{    
    ERROR_STATE e_funStatus = OK;
    
    if(gu8_systemTimerModuleState == SYSTEM_TIMER_INITALIZED)
    {
        e_funStatus = NOK; 
        error_handler(TIMER_MODULE_ERROR_NUM+MULTIPLE_INITALIZATION);
    }
    else
    {
        TMR1IE=1 ;
        PEIE=1 ;
        GIE=1 ;
        TMR1=0 ;
        TMR1CS=0 ;
        T1CKPS0 = 1 ;
        T1CKPS1= 1 ;
        gu8_timer_ticks = 0;
        gu8_systemTimerModuleState = SYSTEM_TIMER_INITALIZED;
    }
    return e_funStatus;
}

ERROR_STATE start_system_timer(void)
{
    ERROR_STATE e_funStatus = OK;
    
    if(gu8_systemTimerModuleState == SYSTEM_TIMER_IDLE_STATE)
    {
        e_funStatus = NOK; 
        error_handler(TIMER_MODULE_ERROR_NUM+MODULE_NOT_INITALIZED);
    }
    else
    {
        //time to calculate 50ms
        TMR1L = 0x83;
        TMR1H = 0xC7;
        TMR1ON = 1 ;    
    }
    return e_funStatus;
}