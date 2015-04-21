/*----------------------------------------------------------------------------
 * Name:    Gpio.c
 * Purpose: GPIO usage for STM32
 * Version: V1.00
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * Copyright (c) 2005-2007 Keil Software. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stm32f10x.h>                        // STM32F10x Library Definitions
#include "STM32_Init.h"                           // STM32 Initialization
#include "usart.h"
#include <stdio.h>

/*-----------------------------------------------------------------------------
  delay some time 	
*------------------------------------------------------------------------------*/

char recvbuf;

void delay(unsigned int time) {
  while(time--);
}


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {

    buffer_Init();
    stm32_Init ();                                  // STM32 setup
	GPIOA->ODR &= ~(1 << 2);

    while (1) {  					  // Loop forever
        GPIOA->ODR &= ~(1 << 2);
        //printf("Led is onr\n");
        delay(100000);
        
        GPIOA->ODR |= (1 << 2);
        //printf("Led is offr\n");
        delay(100000);
        //printf("STM32F103ZETr\n");
  } 
} 

/*
int  SendChar(int ch)
{
  USART1->DR   = ch;
    while(!(USART1->SR & 0x40));
  
  return 0;
}

int  GetKey(void)
{
    return recvbuf;
}
*/

/*
void USART1_IRQHandler (void) 
{
  volatile unsigned int IIR;

    IIR = USART1->SR;
    if (IIR & USART_SR_RXNE) {                  // read interrupt
      USART1->SR &= ~USART_SR_RXNE;	          // clear interrupt
      
      recvbuf = (USART1->DR & 0x1FF);
      
      printf("can do %c\n\r", recvbuf);
      ownprintf("RECEIVE DATA IS %c\n", recvbuf);
    }

    if (IIR & USART_SR_TXE) {
      USART1->SR &= ~USART_SR_TXE;	          // clear interrupt
    }
}
*/
