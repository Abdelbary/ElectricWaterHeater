#include "btn.h"
#include "btn_Cfg.h"
#define IDLE 0
#define INIT 1
static BtnStateType gae_BTN_Array_state[BTN_NUM_OF_BUTTONS];
static uint8_t gu8_BTN_moduleState = IDLE;
ERROR_STATE BTN_Init(void)
{
    ERROR_STATE e_funStatus = OK;
    if(gu8_BTN_moduleState == INIT)
    {
        e_funStatus = NOK;
        //rport error
    }
    else
    {
        /***Initializing first state for all buttons****/
        uint8_t i=0;
        for(i=0;i<BTN_NUM_OF_BUTTONS ;i++)
        {
            gae_BTN_Array_state[i] = BUT_OFF;

        }
        gu8_BTN_moduleState = INIT;
    }
	return e_funStatus;
}

ERROR_STATE BTN_GetState(BtnStateType *ep_BtnStatePtr, uint8_t BtnId)
{
    ERROR_STATE e_funStatus = OK;
    if(gu8_BTN_moduleState == IDLE)
    {
        e_funStatus = NOK;
        ///report error
    }
    else if(BtnId >= BTN_NUM_OF_BUTTONS || ep_BtnStatePtr == NULL )
    {
         e_funStatus = NOK;
        ///report error
    }
    else
    {
        	*ep_BtnStatePtr = gae_BTN_Array_state[BtnId];
    }
    return e_funStatus;
	
}
ERROR_STATE BTN_Manager(void)
{
    ERROR_STATE e_funStatus = OK;
    if(gu8_BTN_moduleState == IDLE)
    {
        e_funStatus = NOK;
        ///report error
    }
    else
    {
        uint8_t i=0;
        BtnStateType e_state;
        uint8_t u8_Data;
        for(i=0;i<BTN_NUM_OF_BUTTONS ;i++) 					//for all buttons
        {
            e_state = gae_BTN_Array_state[i]; 						//put previous state in state
            DIO_Read(gcae_BUT_ConfigParam[i].u8_DioGroupId , &u8_Data);
            switch(gcae_BUT_ConfigParam[i].e_BtnActiveState)		//switching on Btn active state is it active low or active high
            {
                case ActiveLow:
                {
                    switch(e_state)
                    {
                        case BUT_OFF:
                        {
                            if(u8_Data == 0)
                            {
                                e_state = BUT_PRE_PRESSED;
                            }
                            else
                            {
                                e_state = BUT_OFF;
                            }
                        }
                        break;
                        case BUT_PRE_PRESSED:
                        {
                            if(u8_Data == 0)
                            {
                                e_state = BUT_ON;
                            }
                            else
                            {
                                e_state = BUT_OFF;
                            }
                        }

                        break;
                        case BUT_ON:
                        {
                            if(u8_Data == 0)
                            {
                                e_state = BUT_PRSSED;
                            }
                            else
                            {
                                e_state = BUT_RELEASED;
                            }
                        }
                        
                        break;
                        case BUT_PRSSED:
                        {
                            if(u8_Data == 0)
                            {
                                e_state = BUT_PRSSED;
                            }
                            else
                            {
                                e_state = BUT_PRE_RELEASED;
                            }
                        }

                        
                        break;
                        
                        case BUT_PRE_RELEASED:
                        {
                            if(u8_Data == 0)
                            {
                                e_state = BUT_PRE_PRESSED;
                            }
                            else
                            {
                                e_state = BUT_RELEASED;
                            }
                        }

                        
                        break;
                        case BUT_RELEASED:
                        {
                            if(u8_Data == 0)
                            {
                                e_state = BUT_ON;
                            }
                            else
                            {
                                e_state = BUT_OFF;
                            }
                        }
                        break;
                    }

                    break;
                }
                case ActiveHigh:
                {
                    switch(e_state)
                    {
                        case BUT_OFF:
                        {
                            if(u8_Data != 0)
                            {
                                e_state = BUT_PRE_PRESSED;
                            }
                            else
                            {
                                e_state = BUT_OFF;
                            }
                        }
                        break;
                        case BUT_PRE_PRESSED:
                        {
                            if(u8_Data != 0)
                            {
                                e_state = BUT_ON;
                            }
                            else
                            {
                                e_state = BUT_OFF;
                            }
                        }

                        break;
                        case BUT_ON:
                        {
                            if(u8_Data != 0)
                            {
                                e_state = BUT_PRSSED;
                            }
                            else
                            {
                                e_state = BUT_RELEASED;
                            }
                        }
                        
                        break;
                        case BUT_PRSSED:
                        {
                            if(u8_Data != 0)
                            {
                                e_state = BUT_PRSSED;
                            }
                            else
                            {
                                e_state = BUT_PRE_RELEASED;
                            }
                        }

                        
                        break;
                        
                        case BUT_PRE_RELEASED:
                        {
                            if(u8_Data != 0)
                            {
                                e_state = BUT_PRE_PRESSED;
                            }
                            else
                            {
                                e_state = BUT_RELEASED;
                            }
                        }

                        
                        break;
                        case BUT_RELEASED:
                        {
                            if(u8_Data != 0)
                            {
                                e_state = BUT_ON;
                            }
                            else
                            {
                                e_state = BUT_OFF;
                            }
                        }
                        break;
                    }

                    break;
                }

                break;
            }
            gae_BTN_Array_state[i] = e_state;
        }
    }
	    return e_funStatus;

}
