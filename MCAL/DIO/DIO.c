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
#define PIN_REG(PORT_ID)  (*(volatile uint8_t*)(gscu8_DIO_PortShift[PORT_ID] + PORT_SUBSYSTEM_BASE_ADDRESS))
#define PORT_REG(PORT_ID) (*(volatile uint8_t*)(gscu8_DIO_PortShift[PORT_ID] + PORT_SUBSYSTEM_BASE_ADDRESS))
#define DDR_REG(PORT_ID)  (*(volatile uint8_t*)(gscu8_DIO_PortShift[PORT_ID] + PORT_SUBSYSTEM_BASE_ADDRESS + DDR_REG_OFFSET))  
static const uint8_t gscu8_DIO_PortShift[PORT_NUM] = {0x00, 0x01, 0x02, 0x03 ,0x04};


void DIO_Init(void)
{
	uint8_t u8_loop;
	for(u8_loop = 0; u8_loop < DIO_NUM_OF_GROUPS; u8_loop++)
	{
		DDR_REG(DIO_ConfigParam[u8_loop].Port)  &= ~(DIO_ConfigParam[u8_loop].Mask);
        DDR_REG(DIO_ConfigParam[u8_loop].Port)  |= (DIO_ConfigParam[u8_loop].Mask & DIO_ConfigParam[u8_loop].Direction);
		PORT_REG(DIO_ConfigParam[u8_loop].Port) &= ~(DIO_ConfigParam[u8_loop].Mask);
		PORT_REG(DIO_ConfigParam[u8_loop].Port) |= (DIO_ConfigParam[u8_loop].Mask & DIO_ConfigParam[u8_loop].UsePullUp);
    }
}


void DIO_Write(uint8_t u8_GroupId, uint8_t u8_Data)
{
	uint8_t  u8_TempData;
	u8_TempData = PORT_REG(DIO_ConfigParam[u8_GroupId].Port);
	u8_TempData &= ~(DIO_ConfigParam[u8_GroupId].Mask);
	u8_TempData |= (DIO_ConfigParam[u8_GroupId].Mask & u8_Data);
	PORT_REG(DIO_ConfigParam[u8_GroupId].Port) = u8_TempData;
}

void DIO_Toggle(uint8_t u8_GroupId)
{
	uint8_t  u8_TempData;
	u8_TempData = PORT_REG(DIO_ConfigParam[u8_GroupId].Port);
	u8_TempData ^= (DIO_ConfigParam[u8_GroupId].Mask);
	
	PORT_REG(DIO_ConfigParam[u8_GroupId].Port) = u8_TempData;
}
void DIO_Read(uint8_t u8_GroupId,uint8_t * u8p_DataPtr)
{
	*u8p_DataPtr = PIN_REG(DIO_ConfigParam[u8_GroupId].Port) & DIO_ConfigParam[u8_GroupId].Mask;
}