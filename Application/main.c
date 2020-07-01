/*
 * File:   main.c
 * Author: Mahmoud Abdelbary
 *
 * Created on June 29, 2020, 10:51 AM
 */
#include "../MCAL/Registers.h"
#include "../ServiceLayer/std_types.h"
#include "../ServiceLayer/Config.h"
#include "../MCAL/DIO/DIO.h"
#include <xc.h>

#define LEDPIN 0
void main(void) {
    DIO_Init();
    while(1)
    {
        DIO_Write(LEDPIN,HIGH);
        __delay_ms(10);
        DIO_Write(LEDPIN,LOW);
         __delay_ms(10);

        //PORTB_REG = 0x00;
    }
    
    return;
}
