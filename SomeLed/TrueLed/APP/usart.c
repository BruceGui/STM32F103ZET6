
#include "os_cpu.h"
#include "ucos_ii.h"

#include "usart.h"

/*
 * USART1_IRQHandler 
 * Handler the Revice and Trans IRQ
 */
 
struct __FILE {
	int handle;
};
FILE __stdout;
FILE __stdin;

volatile unsigned int RxCounter;
volatile unsigned int TxCounter;
unsigned int TxBuffer[TX_SIZE];
 
void USART1_IRQHandler()
{ 
	unsigned char msg[MSG_SIZE]; 
	
	//printf("IRQHandler");
	
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();
	OSIntNesting ++;
	OS_EXIT_CRITICAL();
	
	/*
	printf("IRQ_Handler\r\n");
	*/
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		 //msg[RxCount++] = USART_ReceiveData(USART1);
		msg[RxCounter++] = USART_ReceiveData(USART1);
	}  
	
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)                     // 
  	{ 
     	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
  	}	
    OSIntExit(); 
}

/*
 * initialization the usart_1
 */

void USARTConfig(USART_TypeDef* USARTx, u32 baud)
{
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate 			  = baud;
	USART_InitStructure.USART_WordLength 		  = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits   		  = USART_StopBits_1;
	USART_InitStructure.USART_Parity     		  = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode  			  = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USARTx, &USART_InitStructure);
	
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	USART_Cmd(USARTx, ENABLE);
	
	/*
	printf("Initialization.....\r\n");
	*/
}

int SendChar(int ch)
{
	USART_SendData(USART1, (unsigned char)ch);
	while (!(USART1->SR & USART_FLAG_TXE));
	return ch;
}

int fputc(int ch, FILE *f) 
{
	return SendChar(ch);
}

void ttywrch(int ch)
{
	SendChar(ch);
}

int ferror(FILE *f)
{
	return EOF;
}

void _sys_exit(int return_code)
{
	label: goto label;
}

void prompt(void)
{
	printf("------Welcome To The STM32 world------\r\n");
	printf("------Select A Function:\r\n");
	printf("1.Change The Light Frequency.\r\n");
	printf("Input the number then press Enter:");
}
