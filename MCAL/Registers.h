/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:             Resgister.h
 * Author:           Mahmoud Abdelbary
 * Comments:    
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef REGISTERS_H
#define	REGISTERS_H

/*******************************************************************************
 *                              Included Files                               *
 *******************************************************************************/


#include "../ServiceLayer/std_types.h"
/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* Definition for PORTA Registers */
#define PORTA_REG	(*(volatile uint8 *)0x005)
#define DDRA_REG	(*(volatile uint8 *)0x085)
#define PINA_REG 	(*(volatile uint8 *)0x005)

/* Definition for PORTB Registers */
#define PORTB_REG	(*(volatile uint8 *)0x006)
#define DDRB_REG	(*(volatile uint8 *)0x086)
#define PINB_REG 	(*(volatile uint8 *)0x006)

/* Definition for PORTC Registers */
#define PORTC_REG	(*(volatile uint8 *)0x007)
#define DDRC_REG	(*(volatile uint8 *)0x087)
#define PINC_REG 	(*(volatile uint8 *)0x007)

/* Definition for PORTD Registers */
#define PORTD_REG	(*(volatile uint8 *)0x008)
#define DDRD_REG	(*(volatile uint8 *)0x088)
#define PIND_REG 	(*(volatile uint8 *)0x008)

/* Definition for PORTE Registers */
#define PORTE_REG	(*(volatile uint8 *)0x009)
#define DDRE_REG	(*(volatile uint8 *)0x089)
#define PINE_REG 	(*(volatile uint8 *)0x009)


#endif //REGISTERS_H