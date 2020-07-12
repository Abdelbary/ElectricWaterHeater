/*
 * File:   DIO.c
 * Author: Mahmoud Abdelbary
 *
 * Created on June 29, 2020, 4:21 PM
 */
#include "DIO.h"
#include "DIO_Cfg.h"


#define DDR_REG_OFFSET              0x80
#define PORT_SUBSYSTEM_BASE_ADDRESS 0x05
#define PORT_NUM                    5
#define DIO_IDLE_STATE              0
#define DIO_INIT_STATE              1   
#define PIN_REG(PORT_ID)  (*(volatile uint8_t*)(gscu8_DIO_PortShift[PORT_ID] + PORT_SUBSYSTEM_BASE_ADDRESS))
#define PORT_REG(PORT_ID) (*(volatile uint8_t*)(gscu8_DIO_PortShift[PORT_ID] + PORT_SUBSYSTEM_BASE_ADDRESS))
#define DDR_REG(PORT_ID)  (*(volatile uint8_t*)(gscu8_DIO_PortShift[PORT_ID] + PORT_SUBSYSTEM_BASE_ADDRESS + DDR_REG_OFFSET))  

static const uint8_t gscu8_DIO_PortShift[PORT_NUM] = {0x00, 0x01, 0x02, 0x03 ,0x04};
static uint8_t       gu8_DIO_moduleStatuc = DIO_IDLE_STATE;

ERROR_STATE DIO_Init(void)
{
     /*check
     *  if the module already initlaized
     *      then change function state to NOK  and report error
     *  else
     *      apply configration of the config struct
     */
    ERROR_STATE e_funStatus = OK;
    if(gu8_DIO_moduleStatuc == DIO_INIT_STATE)
    {
       e_funStatus   = NOK;
       error_handler(DIO_MODULE_ERROR_NUM+MULTIPLE_INITALIZATION); 
    }
    else
    {
        //turn off compartor mode for port b pins
        CMCON = 0x07;
        //zero all ports
        
        /*
         * for each configure struct 
         *      configure DIO_GROUP as specified in the config struct
         *  mark module as initalized
         */
        uint8_t u8_loop;
        for(u8_loop = 0; u8_loop < DIO_NUM_OF_GROUPS; u8_loop++)
        {
            DDR_REG(gstr_DIO_ConfigParam[u8_loop].Port)  &= ~(gstr_DIO_ConfigParam[u8_loop].Mask);
            DDR_REG(gstr_DIO_ConfigParam[u8_loop].Port)  |= (gstr_DIO_ConfigParam[u8_loop].Mask & gstr_DIO_ConfigParam[u8_loop].Direction);
            PORT_REG(gstr_DIO_ConfigParam[u8_loop].Port) &= ~(gstr_DIO_ConfigParam[u8_loop].Mask);
            PORT_REG(gstr_DIO_ConfigParam[u8_loop].Port) |= (gstr_DIO_ConfigParam[u8_loop].Mask & gstr_DIO_ConfigParam[u8_loop].UsePullUp);
        }
        gu8_DIO_moduleStatuc = DIO_INIT_STATE;
    }
    return e_funStatus;
	
}


ERROR_STATE DIO_Write(uint8_t u8_GroupId, uint8_t u8_Data)
{
     /*check
     *  if the module is not  initlaized
     *      then change function state to NOK  and report error
     *  else if group id > available groups       
     *      then change function state to NOK  and report error
     * else
     *      then  write given data to the specified group id  
     *      
     */
    ERROR_STATE e_funStatus = OK;
    if(gu8_DIO_moduleStatuc == DIO_IDLE_STATE)
    {
        e_funStatus   = NOK;
        error_handler(DIO_MODULE_ERROR_NUM+MULTIPLE_INITALIZATION); 
    }
    else if(u8_GroupId >= DIO_NUM_OF_GROUPS)
    {
        e_funStatus   = NOK;
        error_handler(DIO_MODULE_ERROR_NUM+MODULE_NOT_INITALIZED); 
    }
    else
    {
        uint8_t  u8_TempData;
        u8_TempData = PORT_REG(gstr_DIO_ConfigParam[u8_GroupId].Port);
        u8_TempData &= ~(gstr_DIO_ConfigParam[u8_GroupId].Mask);
        u8_TempData |= (gstr_DIO_ConfigParam[u8_GroupId].Mask & u8_Data);
        PORT_REG(gstr_DIO_ConfigParam[u8_GroupId].Port) = u8_TempData; 
        
        gu8_DIO_moduleStatuc = DIO_INIT_STATE;
        
    }
    return e_funStatus;
	
}

ERROR_STATE DIO_Toggle(uint8_t u8_GroupId)
{  
     /*check
     *  if the module is not  initlaized
     *      then change function state to NOK  and report error
     *  else if group id > available groups       
     *      then change function state to NOK  and report error
     * else
     *      then  toggle specified group id
     *      
     */
    ERROR_STATE e_funStatus = OK;
    if(gu8_DIO_moduleStatuc == DIO_IDLE_STATE)
    {
        e_funStatus   = NOK;
        error_handler(DIO_MODULE_ERROR_NUM+MULTIPLE_INITALIZATION); 
    }
    else if(u8_GroupId >= DIO_NUM_OF_GROUPS)
    {
        e_funStatus   = NOK;
        error_handler(DIO_MODULE_ERROR_NUM+INVALAD_PARAMETER); 
    }
    else
    {
	uint8_t  u8_TempData;
	u8_TempData = PORT_REG(gstr_DIO_ConfigParam[u8_GroupId].Port);
	u8_TempData ^= (gstr_DIO_ConfigParam[u8_GroupId].Mask);
	
	PORT_REG(gstr_DIO_ConfigParam[u8_GroupId].Port) = u8_TempData;
    }
    return e_funStatus;
}



ERROR_STATE DIO_Read(uint8_t u8_GroupId,uint8_t * u8p_DataPtr)
{
    /*check
     *  if the module is not  initlaized
     *      then change function state to NOK  and report error
     *  else if group id > available groups       
     *      then change function state to NOK  and report error
     *  else if given data pointer is null
     *      then change function state to NOK  and report error
     *  else
     *      then  update given output pointer with group_id value 
     *      
     */
    ERROR_STATE e_funStatus = OK;
    if(gu8_DIO_moduleStatuc == DIO_IDLE_STATE)
    {
        e_funStatus   = NOK;
        error_handler(DIO_MODULE_ERROR_NUM+MULTIPLE_INITALIZATION); 
    }
    else if(u8_GroupId >= DIO_NUM_OF_GROUPS)
    {
        e_funStatus   = NOK;
        error_handler(DIO_MODULE_ERROR_NUM+INVALAD_PARAMETER); 
    }
    else if(u8p_DataPtr == NULL)
    {
        e_funStatus   = NOK;
        error_handler(DIO_MODULE_ERROR_NUM+INVALAD_PARAMETER);
    }
    else
    {
	*u8p_DataPtr = PIN_REG(gstr_DIO_ConfigParam[u8_GroupId].Port) & gstr_DIO_ConfigParam[u8_GroupId].Mask;
    }
    return e_funStatus;
 }