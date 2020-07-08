#ifndef DIO_H
#define DIO_H

#include "../../ServiceLayer/std_types.h"

//define ports
#define PORT_A 0x00
#define PORT_B 0x01
#define PORT_C 0x02
#define PORT_D 0x03
#define PORT_E 0x04


//define PINS for mask value
#define PIN0 0x01
#define PIN1 0x02
#define PIN2 0x04
#define PIN3 0x08
#define PIN4 0x10
#define PIN5 0x20
#define PIN6 0x40
#define PIN7 0x80





typedef struct 
{
	uint8_t Mask; //enable us to configure each pin seperatly 
	uint8_t Port;
	uint8_t Direction;
	uint8_t UsePullUp;
}DioConfigParamType;


// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype: void DIO_Init(void)</b></p>
  
    <p><b>Summary: initalize all project pins for init values</b></p>

    <p><b>Description: loop through the array of pin confegration and 
 *                     configure each pin as specified</b></p>

    <p><b>Precondition:</b> No preconditions required</p>

    <p><b>Parameters:</b> void </p>

    <p><b>Returns:</b> void </p>

    <p><b>Example:</b> DIO_Init()</p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
#include "../../ServiceLayer/Config.h"
ERROR_STATE DIO_Init(void);
ERROR_STATE DIO_Write(uint8_t GroupId, uint8_t Data);
ERROR_STATE DIO_Toggle(uint8_t u8_GroupId);
ERROR_STATE DIO_Read(uint8_t GroupId, uint8_t * DataPtr);


#endif