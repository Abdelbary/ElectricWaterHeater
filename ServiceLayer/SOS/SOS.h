/*
 * SOS.h
 *
 * Created: 2/24/2020 11:51:33 AM
 *  Author: mahmo
 */ 


#ifndef SOS_H_
#define SOS_H_

#include "../std_types.h"
#include "../common_macros.h"
#include "../Config.h"
#include "../Error_Handler/SystemErrors.h"
#include "../../MCAL/TIMER/timer.h"


#define SOS_TIMER_CH0      0
#define SOS_TIMER_CH1	   1
#define SOS_TIMER_CH2	   2

#define PERIODIC    0
#define ON_SHOT     1

typedef void (*CBF)(void);

typedef struct SOS_cfg_str
{
	uint8_t u8_timer_ch;
	uint8_t u8_tick_reslution; /* in milesec */    
}gstr_SOS_cfg_t;



/**
* \b Description: SOS_init initialize SOS module with timer canal specified and 
*				  and time tick reslution as multiple of  timer tick.
*@param: void 
* <br>
* <b> NOTE: use linking configuration of sruct SOS_cfg </b>
*@return: ERROR_STATUS {OK,NOK}
*
*
*@see     SOS_cfg in SOS_Cfg.c
*/
ERROR_STATE    SOS_Init(void);

/**
*\b Description: SOS_start add new SOS_object to be served on periodic time 
*				
* 
* @param[in]   id               from 0 to SOS_OBJ_BUFFER_SIZE.
* @param[in] 	 callB_fun_ptr  addresss of the rotuen to be called.
* @param[in] 	 lap_time       form 0 to SOS_MAX_LAP_TIME
* @param[in] 	 type           [PERIODIC,ONE_SHOT]
* 
* @return: ERROR_STATUS [OK,NOK]
*/
ERROR_STATE	SOS_createTask(uint8_t Id,void (*callB_fun_ptr)(void),uint16_t lap_time,uint8_t type,uint8_t periority);

/**
*\b Description: SOS_stop delete task with specified id number by removing it from
*			   SOS Buffer.
* @param[in] Id  id of the task to be removed from SOS buffer<br>
*					0<=id<SOS_BUFFER_MAX_SIZE
* @return: ERROR_STATUS [OK,NOK]
*/
ERROR_STATE	SOS_deletTask(uint8_t Id);

/**
*\b Description: SOS_dispatcher is the main SOS procedure, dispatcher check for new time
*				tick then loop through the SOS_objBuffer and execute tasks at their 
*				specified time.
* @param: void
* @return: ERROR_STATUS  [OK,NOK]
*/
ERROR_STATE	SOS_run(void);

/**
*\b Description: SOS_DeInit Danit SOS.
* 
* @param: void
* 
* @return: ERROR_STATUS [OK,NOK]
*/
ERROR_STATE	SOS_Deinit(void);


/**
*\b Description: SOS_StartProc run the start of os procedure
* by calling the call back function pointer passed as a paramter
* from the application
* 
* @param[in] CBF function to call at the start of the os
* 
* @return: ERROR_STATUS [OK,NOK]
*/
ERROR_STATE SOS_StartProc(CBF callBackFun);

#endif /* SOS_H_ */