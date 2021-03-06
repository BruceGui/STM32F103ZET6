/****************************************************************************
* Copyright (C), 2010 奋斗嵌入式工作室 ourstm.5d6d.com
*
* 本例程在 奋斗版STM32开发板V2,2.1,V3,MINI上调试通过           
* QQ: 9191274, 旺旺：sun68, Email: sun68@163.com 
* 淘宝店铺：ourstm.taobao.com  
*
* 文件名: app.c
* 内容简述:
*       本例程操作系统采用ucos2.86a版本， 建立了5个任务
			任务名											 优先级
			APP_TASK_START_PRIO                               2	        主任务	  		
           
            Task_Led1_PRIO                                    7			LED1 闪烁任务
            Task_Led2_PRIO                                    8			LED2 闪烁任务
            
		 当然还包含了系统任务：
		    OS_TaskIdle                  空闲任务-----------------优先级最低
			OS_TaskStat                  统计运行时间的任务-------优先级次低
*
* 文件历史:
* 版本号  日期       作者    说明
* v0.1    2010-11-12 sun68  创建该文件
*
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#define GLOBALS


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

extern void fun_para(void);

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static  OS_STK App_TaskStartStk[APP_TASK_START_STK_SIZE];

static  OS_STK Task_Led1Stk[Task_Led_STK_SIZE]; 
		

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/
static  void App_TaskCreate(void);

static  void App_TaskStart(void* p_arg);

static  void Task_Led1(void* p_arg); 
//
//static  void App_TaskJoystick(void* p_arg);
#define LED_LED1_ON()   GPIO_SetBits(GPIOA, GPIO_Pin_2 );  	       //LED1  亮
#define LED_LED1_OFF()  GPIO_ResetBits(GPIOA, GPIO_Pin_2 ); 	   //LED1  灭 

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Argument : none.
*
* Return   : none.
*********************************************************************************************************
*/

void OS_CPU_SysTickInit(void);
void GPIO_Configuration(void);

volatile unsigned int milsec;

int main(void)
{
   CPU_INT08U os_err;

   //禁止CPU中断
   CPU_IntDis();
   
   //UCOS 初始化
   OSInit();                                                   /* Initialize "uC/OS-II, The Real-Time Kernel".         */
   
   //硬件平台初始化
	SystemInit();                                                  /* Initialize BSP functions.  */
	GPIO_Configuration();
   
   //默认LED闪烁间隔500ms
   milsec=200; 
   //建立主任务， 优先级最高  建立这个任务另外一个用途是为了以后使用统计任务
   os_err = OSTaskCreate((void (*) (void *)) App_TaskStart,	  		  		//指向任务代码的指针
                          (void *) 0,								  		//任务开始执行时，传递给任务的参数的指针
               (OS_STK *) &App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],	//分配给任务的堆栈的栈顶指针   从顶向下递减
               (INT8U) APP_TASK_START_PRIO);								//分配给任务的优先级
   
   //ucos的节拍计数器清0    节拍计数器是0-4294967295    对于节拍频率100hz时， 每隔497天就重新计数 
   OSTimeSet(0);
   OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II).  */
                                                 /* Start multitasking (i.e. give control to uC/OS-II).  */
 
   return (os_err);
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
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*                                          App_TaskStart()
*
* Description : The startup task.  The uC/OS-II ticker should only be initialize once multitasking starts.
*
* Argument : p_arg       Argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
*
* Return   : none.
*
* Caller   : This is a task.
*
* Note     : none.
*********************************************************************************************************
*/

static  void App_TaskStart(void* p_arg)
{
   


   (void) p_arg;

   //初始化ucos时钟节拍
   OS_CPU_SysTickInit();                                       /* Initialize the SysTick.       */

//使能ucos 的统计任务
#if (OS_TASK_STAT_EN > 0)
   //----统计任务初始化函数  
   OSStatInit();                                               /* Determine CPU capacity.                              */
#endif
   //建立其他的任务
   App_TaskCreate();

   while (1)
   {
      //1秒一次循环
	  OSTimeDlyHMSM(0, 0,10, 0);

   }
}


//LED1闪烁任务----------------------------------------
static  void Task_Led1(void* p_arg)
{
   


   (void) p_arg;	    
   while (1)
   {
      LED_LED1_ON();
      OSTimeDlyHMSM(0, 0, 0, milsec);
      
      LED_LED1_OFF();
      OSTimeDlyHMSM(0, 0, 0, milsec);	
   }
}
/*
*********************************************************************************************************
*                                            App_TaskCreate()
*
* Description : Create the application tasks.
*
* Argument : none.
*
* Return   : none.
*
* Caller   : App_TaskStart().
*
* Note     : none.
*********************************************************************************************************
*/

static  void App_TaskCreate(void)
{
    //LED1 闪烁任务------------------------------------------------------
   OSTaskCreateExt(Task_Led1,(void *)0,(OS_STK *)&Task_Led1Stk[Task_Led_STK_SIZE-1],Task_Led_PRIO,Task_Led_PRIO,(OS_STK *)&Task_Led1Stk[0],
                    Task_Led_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);
}
/*
*********************************************************************************************************
*********************************************************************************************************
*                                          uC/OS-II APP HOOKS
*********************************************************************************************************
*********************************************************************************************************
*/

#if (OS_APP_HOOKS_EN > 0)
/*
*********************************************************************************************************
*                                      TASK CREATION HOOK (APPLICATION)
*
* Description : This function is called when a task is created.
*
* Argument : ptcb   is a pointer to the task control block of the task being created.
*
* Note     : (1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

void App_TaskCreateHook(OS_TCB* ptcb)
{
}

/*
*********************************************************************************************************
*                                    TASK DELETION HOOK (APPLICATION)
*
* Description : This function is called when a task is deleted.
*
* Argument : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note     : (1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

void App_TaskDelHook(OS_TCB* ptcb)
{
   (void) ptcb;
}

/*
*********************************************************************************************************
*                                      IDLE TASK HOOK (APPLICATION)
*
* Description : This function is called by OSTaskIdleHook(), which is called by the idle task.  This hook
*               has been added to allow you to do such things as STOP the CPU to conserve power.
*
* Argument : none.
*
* Note     : (1) Interrupts are enabled during this call.
*********************************************************************************************************
*/

#if OS_VERSION >= 251
void App_TaskIdleHook(void)
{
}
#endif

/*
*********************************************************************************************************
*                                        STATISTIC TASK HOOK (APPLICATION)
*
* Description : This function is called by OSTaskStatHook(), which is called every second by uC/OS-II's
*               statistics task.  This allows your application to add functionality to the statistics task.
*
* Argument : none.
*********************************************************************************************************
*/

void App_TaskStatHook(void)
{
}

/*
*********************************************************************************************************
*                                        TASK SWITCH HOOK (APPLICATION)
*
* Description : This function is called when a task switch is performed.  This allows you to perform other
*               operations during a context switch.
*
* Argument : none.
*
* Note     : 1 Interrupts are disabled during this call.
*
*            2  It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                   will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the
*                  task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/

#if OS_TASK_SW_HOOK_EN > 0
void App_TaskSwHook(void)
{
}
#endif

/*
*********************************************************************************************************
*                                     OS_TCBInit() HOOK (APPLICATION)
*
* Description : This function is called by OSTCBInitHook(), which is called by OS_TCBInit() after setting
*               up most of the TCB.
*
* Argument : ptcb    is a pointer to the TCB of the task being created.
*
* Note     : (1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/

#if OS_VERSION >= 204
void App_TCBInitHook(OS_TCB* ptcb)
{
   (void) ptcb;
}
#endif

#endif
