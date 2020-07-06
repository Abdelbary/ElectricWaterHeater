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


typedef struct SOS_cfg_str
{
	uint8_t timer_ch;
	uint8_t tick_reslution; /* in milesec */
}gstr_SOS_cfg_t;



/*
* Description: SOS_init initialize SOS module with timer canal specified and 
*				and time tick reslution as multiple of 1 ms.
* @pram: SOS_cfg: timer_ch-> 
*							-SOS_TIMER_CH0
*							-SOS_TIMER_CH1
*							-SOS_TIMER_CH2
*				  tick_reslution->
*							-multiple of 1 ms
*@return: ERROR_STATUS [negative number describe module and type of errors]
*/
ERROR_STATE    SOS_Init(void);

/*
* Description: SOS_start add new SOS_object to be served on specific time 
*				if it is the first time to enter function timer  is started.
* @pram: id -> from 0 to SOS_OBJ_BUFFER_SIZE.
*		 callB_fun_ptr-> addresss of the rotuen to be called.
*		 lap_time-> form 0 to SOS_MAX_LAP_TIME
*		 type->[PERIODIC,ONE_SHOT]
*@return: ERROR_STATUS [negative number describe module and type of errors]
*/
ERROR_STATE	SOS_createTask(uint8_t Id,void (*callB_fun_ptr)(void),uint16_t lap_time,uint8_t type);

/*
* Description: SOS_stop stop task with specified id number by removing it from
*			   SOS Buffer.
* @pram: Id:  id of the task to be removed from SOS buffer 
*					0<=id<SOS_BUFFER_MAX_SIZE
*@return: ERROR_STATUS [negative number describe module and type of errors]
*/
ERROR_STATE	SOS_deletTask(uint8_t Id);

/*
* Description: SOS_dispatcher is the main SOS procedure, dispatcher check for new time
*				tick then loop through the SOS_objBuffer and execute tasks at their 
*				specified time.
* @pram: void
* @return: ERROR_STATUS [negative number describe module and type of errors]
*/
ERROR_STATE	SOS_run(void);

/*
* Description: SOS_DeInit Danit SOS unite.
* @pram: void
* @return: ERROR_STATUS [negative number describe module and type of errors]
*/
ERROR_STATE	SOS_Deinit(void);


#endif /* SOS_H_ */