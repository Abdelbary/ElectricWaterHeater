/*
 * SOS.c
 *
 * Created: 2/24/2020 11:50:29 AM
 *  Author: mahmo
 */ 

#include "SOS.h"
#include "SOS_cfg.h"

#define TAKEN				1
#define NO_TASKS           -1
typedef struct SOS_obj_str
{
	/*object to hold data to the call back functions to call periodically*/
	uint8_t Id;
	void (*callB_fun)(void);
	uint16_t fire_tick;
	uint16_t current_ticks;
	uint8_t priority;
	uint8_t type;  /*periodic or one shot*/
	
}gstr_SOS_obj_t;

STATIC uint8_t SOS_Init_flag = FALSE;
STATIC uint8_t SOS_Timer_ch;
STATIC gstr_SOS_obj_t gastr_SOS_ObjBuffer[SOS_OBJ_BUFFER_SIZE];
STATIC sint8_t u8_SOS_objBufferHead;
STATIC uint8_t taken_Ids[SOS_OBJ_BUFFER_SIZE+_ONE]; /*mark taken ids*/
STATIC CBF      gp_OS_StartProc;
ERROR_STATE SOS_Init(void)
{
    ERROR_STATE e_funStatus = OK;
	if(SOS_Init_flag || SOS_linkCfg.u8_tick_reslution > SOS_MAX_TIMER_RESLUTION || SOS_linkCfg.u8_timer_ch > SOS_TIMER_CH2 ) /*check for all errors*/
	{
		/************************************************************************ 
		 *	-module initialized before                          
		 *	-input validation.                                 
		 ************************************************************************/
		if (SOS_Init_flag)
		{
			e_funStatus = NOK;
            error_handler(SOS_MODULE_ERROR_NUM+MULTIPLE_INITALIZATION);
		}
		else if (SOS_linkCfg.u8_tick_reslution > SOS_MAX_TIMER_RESLUTION 
				|| SOS_linkCfg.u8_timer_ch > SOS_TIMER_CH2)
		{
            e_funStatus = NOK;
			error_handler(SOS_MODULE_ERROR_NUM+INVALAD_PARAMETER);
		}
	}
	else
	{
		SOS_Timer_ch = SOS_linkCfg.u8_timer_ch;
        u8_SOS_objBufferHead = _ZERO-_ONE;
		gp_OS_StartProc = NULL;
        //enable timer 1
        system_timer_init();
        SOS_Init_flag = TRUE;

	}
	return e_funStatus;
}

ERROR_STATE SOS_createTask(uint8_t Id,void (*callB_fun_ptr)(void),uint16_t lap_time,uint8_t type,uint8_t periority)
{
	 ERROR_STATE e_funStatus = OK;
	/*lot of condition but it centralize error checking*/
	if(Id > SOS_OBJ_BUFFER_SIZE || callB_fun_ptr == NULL || lap_time > SOS_MAX_LAP_TIME
	|| type > ON_SHOT || SOS_Init_flag == FALSE || taken_Ids[Id] == TAKEN
	|| (lap_time%(SOS_linkCfg.u8_tick_reslution)) != _ZERO /*lap_time not multiple of reslution*/
	|| u8_SOS_objBufferHead ==(SOS_OBJ_BUFFER_SIZE-_ONE) )
	{
		if ( SOS_Init_flag == FALSE)/*module unintalized*/
		{
            e_funStatus = NOK;
			error_handler(SOS_MODULE_ERROR_NUM+MODULE_NOT_INITALIZED);
		}
		else if (callB_fun_ptr == NULL)
		{
            e_funStatus = NOK;
			error_handler(SOS_MODULE_ERROR_NUM+NULL_PTR_ERROR);
		}
		else if(Id > SOS_OBJ_BUFFER_SIZE  ||taken_Ids[Id]  == TAKEN
		|| type > ON_SHOT  || lap_time > SOS_MAX_LAP_TIME
		|| (lap_time%(SOS_linkCfg.u8_tick_reslution))
		|| u8_SOS_objBufferHead ==(SOS_OBJ_BUFFER_SIZE-_ONE))/*invalid parameter*/
		{
            e_funStatus = NOK;
			error_handler(SOS_MODULE_ERROR_NUM+INVALAD_PARAMETER);
		}
		
	}
	else
	{
		uint8_t u8_tempCounter = ZERO;
        
		
		/*
		*	1-get lap time eq
		*	2-setup new task struct
		*	3-load it in place correspond to its priority
		*
		*/
		/*set how many resolution time it takes to fire event*/
		lap_time = (lap_time/(SOS_linkCfg.u8_tick_reslution));
		
		/*SOS_obj_str {id,callBack fun , ticks ,current_ticks,type[periodic,onshot]} */
		gstr_SOS_obj_t tobj  = {Id,callB_fun_ptr,lap_time,ZERO,periority,type};
        
#ifdef PERIORITY
		/*if there is no tasks put task in first pos*/
		if(u8_SOS_objBufferHead == NO_TASKS)
		{
			u8_SOS_objBufferHead++; /*increase buffer head to next empty position*/
			gastr_SOS_ObjBuffer[u8_SOS_objBufferHead] = tobj;
		}
		else
		{
			for (;u8_tempCounter <= u8_SOS_objBufferHead ; u8_tempCounter++)
			{
				if (gastr_SOS_ObjBuffer[u8_tempCounter].priority >= periority)
				{
					uint8_t u8_shiftCounter = (u8_SOS_objBufferHead + _ONE);
					while(u8_shiftCounter > u8_tempCounter)
					{
                         //on two steps due to compiler limitations
						 gstr_SOS_obj_t temp   =  gastr_SOS_ObjBuffer[(u8_shiftCounter-_ONE)];
                         gastr_SOS_ObjBuffer[u8_shiftCounter] = temp;
						u8_shiftCounter--;
					}
					gastr_SOS_ObjBuffer[u8_shiftCounter] = tobj;
					u8_SOS_objBufferHead++;
					break;
				}
			}
			/*check if the task is the lowest priority*/
			if(u8_tempCounter > u8_SOS_objBufferHead)
			{
				u8_SOS_objBufferHead++; /*increase buffer head to next empty position*/
				gastr_SOS_ObjBuffer[u8_SOS_objBufferHead] = tobj;
			}
		}
#else    
		u8_SOS_objBufferHead++; /*increase buffer head to next empty position*/
	    gastr_SOS_ObjBuffer[u8_SOS_objBufferHead] = tobj;
#endif
        
        taken_Ids[Id] = TAKEN ;/*mark this id as taken*/
	}
	return e_funStatus;
}



ERROR_STATE SOS_deletTask(uint8_t Id)
{
	ERROR_STATE e_funStatus = OK;
	
	if (SOS_Init_flag == FALSE)
	{
        e_funStatus = NOK;
		error_handler(SOS_MODULE_ERROR_NUM+MODULE_NOT_INITALIZED);
	}
	else if (Id>SOS_OBJ_BUFFER_SIZE  || taken_Ids[Id] != TAKEN)
	{
        e_funStatus = NOK;
		error_handler(SOS_MODULE_ERROR_NUM+INVALAD_PARAMETER);
	}
	else
	{
		/*
		*  -loop for element in the buffer until reach head
		*				- copy header obj to it 
		*				- decrement header
		*/
		for(uint8_t u8_bufferCnt = 0 ; u8_bufferCnt <=u8_SOS_objBufferHead ; u8_bufferCnt++)
		{
			if(Id == gastr_SOS_ObjBuffer[u8_bufferCnt].Id)
			{
                //on two steps due to compiler limitations
                gstr_SOS_obj_t temp   = gastr_SOS_ObjBuffer[u8_SOS_objBufferHead];
                gastr_SOS_ObjBuffer[u8_bufferCnt]  = temp;
				u8_SOS_objBufferHead--;
				taken_Ids[Id] = ZERO; /*mark id as not taken*/
			}
		}	
	}
	
	

	return e_funStatus;
	
}



ERROR_STATE	SOS_run(void)
{
	ERROR_STATE e_funStatus = OK;

	if (SOS_Init_flag == FALSE)
	{
        e_funStatus = NOK;
		error_handler(SOS_MODULE_ERROR_NUM+MODULE_NOT_INITALIZED);
	}
	else
	{
		STATIC uint16_t current_ticks = _ZERO;
		STATIC uint8_t new_tick_flag = FALSE;
	
        STATIC uint8_t fun_frstTime_flag = FALSE;

        /*check for first time entrance to start timer*/
		if(!fun_frstTime_flag)
		{
			/*start timer*/
			
			start_system_timer();
			gp_OS_StartProc();
			fun_frstTime_flag = TRUE;
		}
		/*
		*	-check for new time tick
		*	-if true loop throw the SOS OBJ Buffer
		*	-increment current ticks 
		*	-check if the current ticks == tick time if so call the CBF
		*	-keep going until finish with all obj in SOS buffer
		*/
	
		/*	check for new time tick
		*	for each timer tick increase current tick and make timer0_MS_flag false
		*	if current tick == lap_time
		*		-zero current tick
		*		-fire new_tick_flag
		*/
        
		if (gu8_timer_ticks)
		{
            current_ticks++;
			gu8_timer_ticks = FALSE;
			if (current_ticks == SOS_linkCfg.u8_tick_reslution)
			{
				current_ticks = _ZERO;
				new_tick_flag = TRUE;
			}
		}
		if(new_tick_flag)
		{
			new_tick_flag = FALSE;
			for(uint8_t bufferCnt = 0 ; bufferCnt <=u8_SOS_objBufferHead ; bufferCnt++)
			{
				(gastr_SOS_ObjBuffer[bufferCnt].current_ticks)++;
				/*check for CBF call time*/
				if((gastr_SOS_ObjBuffer[bufferCnt].current_ticks)==(gastr_SOS_ObjBuffer[bufferCnt].fire_tick))
					{
						gastr_SOS_ObjBuffer[bufferCnt].current_ticks = _ZERO;
						gastr_SOS_ObjBuffer[bufferCnt].callB_fun();
					
						/*handle one_shot functions*/
						if (gastr_SOS_ObjBuffer[bufferCnt].type == ON_SHOT)
						{
							SOS_deletTask(gastr_SOS_ObjBuffer[bufferCnt].Id);
						}

					}
			}
		 }

	}
	
	return e_funStatus;
}


ERROR_STATE SOS_Deinit(void)
{
	
	/*
	*	1-deinit timer
	*	2-set SOS_init_flag to zero
	*/
	ERROR_STATE e_funStatus = OK;
	
	if(SOS_Init_flag == FALSE)
	{
        e_funStatus = NOK;
		/*report error*/
		error_handler(SOS_MODULE_ERROR_NUM+MODULE_NOT_INITALIZED);
	}

	/*do deinit procedure anyway*/
	uint8_t u8_counter = 0;
	
    //deinit timer module
	SOS_Init_flag = FALSE;
	u8_SOS_objBufferHead = _ZERO-_ONE;
		
	/*free taken ids*/
	for (;u8_counter <= SOS_OBJ_BUFFER_SIZE ; u8_counter++)
	{
		taken_Ids[u8_counter] = _ZERO;
	}
	/*set default paramter*/
	SOS_linkCfg.u8_tick_reslution = _ONE;
	SOS_linkCfg.u8_timer_ch = SOS_TIMER_CH0;
	
	return e_funStatus;
}


ERROR_STATE SOS_StartProc(CBF callBackFun)
{
    ERROR_STATE e_funStatus = OK;
	
	if(SOS_Init_flag == FALSE)
	{
        e_funStatus = NOK;
		/*report error*/
		error_handler(SOS_MODULE_ERROR_NUM+MODULE_NOT_INITALIZED);
	}
    else if(callBackFun == NULL)
    {
         e_funStatus = NOK;
		/*report error*/
		error_handler(SOS_MODULE_ERROR_NUM+NULL_PTR_ERROR);
    }
    else
    {
       gp_OS_StartProc = callBackFun;
    }
}

