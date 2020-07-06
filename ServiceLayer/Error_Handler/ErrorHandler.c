/*
 * ErrorHandler.c
 *
 * Created: 2/25/2020 10:35:59 AM
 *  Author: mahmo
 */ 
#include "SystemErrors.h"

#define MODULE_ERROR_HANDLER_ERROR	-100

STATIC uint16_t error_buffer_head = -1 ; /*algorithem increase position first*/
STATIC uint16_t error_Buffer[ERROR_BUFFER_SIZE]; /*buffer to hold system errors*/

void error_handler(sint16_t error_ID)
{
	sint16_t u8_fun_error_status = OK;
	
	if (error_ID != OK)
	{
		if (error_ID > 0)	/*error id must be negative*/
		{
			u8_fun_error_status = (MODULE_ERROR_HANDLER_ERROR+INVALAD_PARAMETER);
		}
		else if (error_buffer_head == (ERROR_BUFFER_SIZE-_ONE)) /*check for buffer capacity*/
		{
			u8_fun_error_status = (MODULE_ERROR_HANDLER_ERROR+FULL_BUFFER);
			/*must report system somehow that the buffer is full*/
		}
		else
		{
			error_buffer_head++; /*increase buffer head to next position*/
			error_Buffer[error_buffer_head] = error_ID;
		}
		
		/*report error in error_handler module*/
		if (u8_fun_error_status != OK)
		{
			error_buffer_head++; /*increase buffer head to next position*/
			error_Buffer[error_buffer_head] = error_ID;
		}
	}
}