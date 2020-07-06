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
#define OK							     1
#define	NULL_PTR_ERROR					-1
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
#define NOK								 0
#define ERROR_BUFFER_SIZE				10

void error_handler(sint16_t error_ID);

#endif /* SYSTEMERRORS_H_ */