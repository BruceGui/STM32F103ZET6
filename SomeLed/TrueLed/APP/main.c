/*
	The Mina function for light LED
	Author: Gui Zhou
	Data: 2015-04-15
*/

#include <stm32f10x_conf.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

/*
 * include uCos source function
 */ 
#include "ucos_ii.h"

/*
 * include the header about STM32 cpu board 
 */
#include "app_cfg.h"
#include "os_cfg.h"
#include "os_cpu.h"
#include "cpu.h"
#include "usart.h"
#include "board.h"

/*
 * Some define to operator the led state
 */
 #define GOON	1
 #define LED_ON()  GPIO_SetBits(GPIOA, GPIO_Pin_2);
 #define LED_OFF() GPIO_ResetBits(GPIOA, GPIO_Pin_2);

/*
 * Some local global variables and functional 
 * about uCos task 
 */
 
 static OS_STK App_TaskStartStk[APP_TASK_START_STK_SIZE];
 static OS_STK Task_LedStk[Task_Led_STK_SIZE];
 static OS_STK Task_UsartStk[Task_USART_STK_SIZE];
 
 static void App_TaskCreate(void);
 static void App_TaskStart(void* p_arg);
 static void Task_LedLight(void* p_arg);
 static void Task_Usart(void* p_arg);
 
 /*
  * define the function about initialize cpu gpio or so on 
 	*/



volatile unsigned int  milsec;
volatile unsigned char LedState;
OS_EVENT* USART1_MBOX;

/*
 *  The main function entry 
 *  
 */

int main(void)
{
	CPU_INT08U os_err;
	
	CPU_IntDis();
	OSInit();
	
	SystemInit();
	GPIO_Configuration();
	NVIC_Configuration();
	
	USARTConfig(USART1, BAUDRATE);
	
	LedState = 0;
	/*
	prompt();
	*/
	
	
	milsec = 100;
	
	os_err = OSTaskCreate((void (*) (void *)) App_TaskStart,
		(void *)0,
			(OS_STK *)&App_TaskStartStk[APP_TASK_START_STK_SIZE-1],
		(INT8U) APP_TASK_START_PRIO);
	
	OSTimeSet(0);
	OSStart();
	
	return os_err;
	
}


void SysTickHandler(void)
{
	OS_CPU_SR cpu_sr;
	
	OS_ENTER_CRITICAL();
	OSIntNesting ++;
	OS_EXIT_CRITICAL();
	
	OSTimeTick();
	OSIntExit();
}


/*
 *  The Task to accomplish led light
 */
static void Task_LedLight(void* p_arg)
{
	(void) p_arg;
	
	while(GOON) {
		/*
		LED_ON();
		OSTimeDlyHMSM(0, 0, 0, milsec);
		LED_OFF();
		OSTimeDlyHMSM(0, 0, 0, milsec);
		*/
	}
}

/*
 * USART_Task
 */

static void Task_Usart(void* p_arg)
{
	INT8U err;
	unsigned char* msg;
	(void)p_arg;
	
	while(GOON) {
		msg=(unsigned char *)OSMboxPend(USART1_MBOX, 0, &err);
		/*
		printf("Message is %s\r\n", msg);
		*/
		if((*msg) == 'O') {
			LED_ON();
		} else {
			LED_OFF();
		}
	}
}

/*
 * Create the task to light led
 */

static void App_TaskCreate(void)
{
	USART1_MBOX = OSMboxCreate((void *) 0);
	
	
	
	OSTaskCreateExt(Task_Usart,
					(void *)0,
					(OS_STK*)&Task_UsartStk[Task_USART_STK_SIZE-1],
					Task_Usart_PRIO,
					Task_Usart_PRIO,
					(OS_STK *)&Task_UsartStk[0],
					Task_USART_STK_SIZE,
					(void*)0,
					OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
	
	
	OSTaskCreateExt(Task_LedLight, 
					(void*)0, 
					(OS_STK *)&Task_LedStk[Task_Led_STK_SIZE-1],
					Task_Led_PRIO,
					Task_Led_PRIO,
					(OS_STK *)&Task_LedStk[0],
					Task_Led_STK_SIZE,
					(void *)0,
					OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
	
}

/*
 * start the tack 
 */

static void App_TaskStart(void* p_arg)
{
	(void) p_arg;
	
	OS_CPU_SysTickInit();

#if (OS_TASK_STAT_EN > 0)
		OSStatInit();
#endif
	App_TaskCreate();
	
	while(GOON) {
		OSTimeDlyHMSM(0, 0, 10, 0);
	}
}


/*
 * write the hook function
 */

#if (OS_APP_HOOKS_EN > 0)

void App_TaskCreateHook(OS_TCB* ptcb)
{
}

void App_TaskDelHook(OS_TCB* ptcb)
{
	(void) ptcb;
}

#if OS_VERSION >= 251
void App_TaskIdleHook(void)
{
	
}
#endif

void App_TaskStatHook(void)
{

}

#if OS_TASK_SW_HOOK_EN > 0
void App_TaskSwHook(void)
{

}
#endif

#if OS_VERSION >= 204
void App_TCBInitHook(OS_TCB* ptcb)
{
   (void) ptcb;
}
#endif

void App_TimeTickHook(void)
{

}

#endif
