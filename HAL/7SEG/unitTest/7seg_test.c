/*
 * File:   7seg_test.c
 * Author: mahmo
 *
 * Created on July 1, 2020, 8:09 PM
 */
#include "../7seg.h"


void sevenSegSimulationTest(){
  {
    DIO_Init();
    sevenSegInit();
    while(1)
    {
        int i = 0 ;
        for( ;i< 99 ; i++)
        {
            int count = 50;
            while(count--)
            {
               sevenSegSendChar(i/10,SEVEN_SEG_ONE);
               __delay_ms(10); 
               sevenSegSendChar(i%10,SEVEN_SEG_TWO);
                __delay_ms(10); 

            }
            
        }
         

        //PORTB_REG = 0x00;
    }
}

}
void sevenSegSimulationSOSTest(){
  
    DIO_Init();
    sevenSegInit();
    static uint8_t u8_counter = 15;
    static uint8_t ch = 0;
    if(ch&1)
        sevenSegSendChar(u8_counter/10,SEVEN_SEG_TWO);
    else
        sevenSegSendChar(u8_counter%10,SEVEN_SEG_ONE);
    ch++;
    if(ch == 2)
    {
      u8_counter;
      ch = 0;
    }
}
