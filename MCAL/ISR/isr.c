#include "isr.h"
#include <xc.h>

void __interrupt() MyISR(void) {


    if (TMR1IF == HIGH_BIT) {
        gu8_timer_ticks++;
        TMR1L = 0x8E;
        TMR1H = 0xFD;
        TMR1IF = LOW_BIT;
    }
 
}

void interruptsInit(void) {
    GIE = 1; //Enable Global Interrupt
    PEIE = 1; //Enable the Peripheral Interrupt
    ei();

}