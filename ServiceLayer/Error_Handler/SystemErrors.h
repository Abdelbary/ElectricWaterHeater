/*
 * SystemErrors.h
 *
 * Created: 2/25/2020 10:35:32 AM
 *  Author: mahmo
 */ 


#ifndef SYSTEMERRORS_H_
#define SYSTEMERRORS_H_
#include "..\common_macros.h"
#include "../std_types.h"

#define DIO_MODULE_ERROR_NUM            -100
#define SEVENSEG_MOUDLE_ERROR_NUM       -200
#define ADC_MODULE_ERROR_NUM            -300
#define TIMER_MODULE_ERROR_NUM          -400
#define	MODULE_NOT_INITALIZED			-2
#define	MULTIPLE_INITALIZATION			-3
#define	STOP_WITHOUT_START				-4
#define	INVALAD_PARAMETER				-5
#define	MULTIPLE_START					-6
#define	FULL_BUFFER						-7
#define MODULE_BUSY						-8
#define INVALID_STATE					-9
#define MODULE_NOT_EN					-10
#define MULTIPLE_EN						-11
#define	NULL_PTR_ERROR					-12
#define ERROR_BUFFER_SIZE				10

typedef enum {OK=0,NOK=-1}ERROR_STATE;

/**
*\b Description: error_handler save error id to error buffer
*				
* 
* @param[in]   error_ID  [MODULE ERROR + ERROR_TYPE]
* 
* @return: void
* 
*/
void error_handler(sint16_t error_ID);

#endif /* SYSTEMERRORS_H_ */