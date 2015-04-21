/*
    main function for driver 
    
    Author: Bruce Gui
    Date:   2014-06-26
*/

#include <stm32f10x.h>
#include <stdio.h>
#include "STM32_Init.h"
#include "usart.h"

#define LEDON  do{GPIOA->ODR &= ~(1 << 2);}while(0)
#define LEDOFF do{GPIOA->ODR |=  (1 << 2);}while(0)

void delay(unsigned int time) {
  while(time--);
}

int main(void)
{
    
    stm32_Init ();                                  // STM32 setup
	GPIOA->ODR &= ~(1 << 2);

    while (1) {  					  // Loop forever
        LEDON;
        printf("Led is on\n");
        delay(100000);
        LEDOFF;
        printf("Led is off\n");
        delay(100000);
        printf("STM32F103ZET\n");
    } 
  
  /*
    
    buffer_Init();
    stm32_Init();
    
    printf("Interrupt driven Serial I/O Example\r\n");
    
    while(1) {
        printf("OWN P\r\n");
    }
  */
}
