/*
 * File:   timer.c
 * Author: mahmo
 *
 * Created on July 5, 2020, 11:57 AM
 */


#include <xc.h>
#include "timer.h"
#define TIMER1_MAX_TICKS  (0xF63Bu)

volatile uint8_t gu8_timer_ticks;
void timer_init()
{  
    TMR1IE=1 ;
    PEIE=1 ;
    GIE=1 ;
    TMR1=0 ;
    TMR1CS=0 ;
    T1CKPS0 = 1 ;
    T1CKPS1= 1 ;
    gu8_timer_ticks = 0;
}

void start_timer()
{
    TMR1L = 0x8E;
    TMR1H = 0xFD;
    TMR1ON = 1 ; 

}