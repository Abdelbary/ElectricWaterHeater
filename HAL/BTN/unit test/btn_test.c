/*
 * File:   7seg_test.c
 * Author: mahmo
 *
 * Created on July 1, 2020, 8:09 PM
 */
#include "../btn.h"
#include "../../7SEG/7seg.h"
#define BTN0 0 
#define BTN1 1 
#define BTN2 2 
BtnStateType u8_on_off_btn_status;
BtnStateType u8_up_btn_status;
BtnStateType u8_down_btn2_status;


void btnSimulationTest(void){
  {
    DIO_Init();
    sevenSegInit();
    BTN_Init();
    int i = 0 ;

    while(1)
    {
        BTN_Manager();
        BTN_GetState(&u8_on_off_btn_status,BTN0);
        BTN_GetState(&u8_up_btn_status,BTN1);
        BTN_GetState(&u8_down_btn2_status,BTN2);
        
        if(u8_on_off_btn_status == BUT_ON)
            i= 0;
        else if(u8_up_btn_status == BUT_ON)
            i++;
        else if(u8_down_btn2_status == BUT_ON)
            i--;
        sevenSegSendChar(i,SEVEN_SEG_ONE);
         __delay_ms(50);
        //PORTB_REG = 0x00;
    }
}

}
