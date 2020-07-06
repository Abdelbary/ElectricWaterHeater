#ifndef BTN_H
#define BTN_H
#include "../../MCAL/DIO/DIO.h"
#include "../../ServiceLayer/std_types.h"
#include "../../ServiceLayer/Config.h"
typedef enum {BUT_OFF, BUT_ON,BUT_PRE_PRESSED,BUT_PRE_RELEASED, BUT_PRSSED, BUT_RELEASED} BtnStateType;
typedef enum {ActiveLow, ActiveHigh} ActiveStateType;
typedef struct 
{
	uint8_t         u8_DioGroupId;
	ActiveStateType e_BtnActiveState;
}BtnConfigType;

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype: void BTN_Init(void)  </b></p>
  
    <p><b>Summary: initalize btns </b></p>

    <p><b>Description: init configured btns to it's initial state as OFF</b></p>

    <p><b>Precondition:</b> No preconditions required</p>

    <p><b>Parameters:</b> void </p>

    <p><b>Returns:</b> ERROR_STATE{OK,NOK} </p>

    <p><b>Example:</b> BTN_Init()</p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 **/
ERROR_STATE BTN_Init(void);


/**
    <p><b>Function prototype: ERROR_STATE BTN_GetState(BtnStateType *ep_BtnStatePtr, uint8_t u8_BtnId)  </b></p>
 * 
    <p><b>Summary: get specified btn state </b></p>

    <p><b>Description: return the specified btn as an output paramter pointer</b></p>

    <p><b>Precondition:</b> Module must be initalized through BTN_Init() function</p>

    <p><b>Parameters:</b> BtnStateType *: pointer to an enum of type BtnStatetype
 *                        uint8_t       : btn group id in the DIO configeration struct</p>

    <p><b>Returns:</b> ERROR_STATE{OK,NOK} </p>

    <p><b>Example:</b>  BTN_GetState(&u8_on_off_btn_status,BTN0) </p>
    <code>
 
    </code>
 **/
ERROR_STATE BTN_GetState(BtnStateType *ep_BtnStatePtr, uint8_t u8_BtnId);

/**
    <p><b>Function prototype: ERROR_STATE BTN_Manager(void)  </b></p>
 * 
    <p><b>Summary: serves as the btn dispatcher </b></p>

    <p><b>Description: periodic function that update the state of each btn, recommended call time 50ms </b></p>

    <p><b>Precondition:</b> Module must be initalized through BTN_Init() function</p>

    <p><b>Parameters:</b> void </p>

    <p><b>Returns:</b> ERROR_STATE{OK,NOK} </p>

    <p><b>Example:</b> BTN_Manager();</p>
    <code>
 
    </code>
 **/
ERROR_STATE BTN_Manager(void);

#endif