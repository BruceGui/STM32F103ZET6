
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_usart.h>

#include "board.h"
#include "ucos_ii.h"

/*
 * init the uCos system tick
 */

void OS_CPU_SysTickInit(void)
{
	RCC_ClocksTypeDef 	rcc_clocks;
	INT32U			 	cnts;

	RCC_GetClocksFreq(&rcc_clocks);
	
	cnts = (INT32U)rcc_clocks.HCLK_Frequency/OS_TICKS_PER_SEC;
	
	SysTick_Config(cnts);
}


void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
}

/*
 * Configures Vector Table base location.
 */

void NVIC_Configuration()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	#if defined (VECT_TAB_RAM)
		NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
	#elif defined (VECT_TAB_FLASH_IAR)
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x2000);
	#else
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
	#endif
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}
