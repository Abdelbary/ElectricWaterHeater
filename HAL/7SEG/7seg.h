#ifndef SEVEN_SEG_H
#define SEVEN_SEG_H

#include "../../ServiceLayer/std_types.h"
#include "../../ServiceLayer/Config.h"
#include "../../MCAL/DIO/DIO.h"


#define SEVEN_SEG_ONE   0
#define SEVEN_SEG_TWO   1
typedef struct 
{
	uint8_t u8_selectorPinGroupId;
    uint8_t u8_sevenSegGroupId;
}sevenSegConfigStruct;


// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype: void sevenSegInit(void) </b></p>
  
    <p><b>Summary: initalize 7 SEG</b></p>

    <p><b>Description: init 7 segmet based on linking configration paramters</b></p>

    <p><b>Precondition:</b> No preconditions required</p>

    <p><b>Parameters:</b> void </p>

    <p><b>Returns:</b> ERROR_STATE{OK,NOK} </p>

    <p><b>Example:</b> sevenSegInit()</p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
ERROR_STATE sevenSegInit(void);

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype: void sevenSegSendChar(const uint8_t value) </b></p>
  
    <p><b>Summary: write to 7 SEG</b></p>

    <p><b>Description:write w digit value to twi 7 segment display</b></p>

    <p><b>Precondition:</b> 7 segment must be initalized</p>

    <p><b>Parameters:</b> const uint8_t value </p>

    <p><b>Returns:</b> ERROR_STATE{OK,NOK} </p>

    <p><b>Example:</b> sevenSegSendChar(15)</p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
ERROR_STATE sevenSegSendChar(const uint8_t u8_value,const uint8_t u8_SSD_selector);
ERROR_STATE disableSevenSeg(void);

#endif