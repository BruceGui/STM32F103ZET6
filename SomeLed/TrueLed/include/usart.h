
/*
 * Author:  Gui Zhou
 * Date  :  2015-04-16
 * Define some thing that can used on usart and usart_task
 */
 
#ifndef USART_H
#define USART_H


#include <stdio.h>
#include <stdarg.h>
#include <rt_misc.h>

#include <stm32f10x_usart.h>

#include "ucos_ii.h"

#pragma import(__use_no_semihosting_swi)

#define MSG_SIZE	50
#define TX_SIZE     256
#define BAUDRATE    115200

extern OS_EVENT*  USART1_MBOX;

void USARTConfig(USART_TypeDef* USARTx, u32 baud);
int  SendChar(int ch);

/*
void vs_printf(char *s, char *format, va_list arg);
*/

void prompt(void);

#endif
