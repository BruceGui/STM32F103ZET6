/****************************************************************************
* Copyright (C), 2010 �ܶ�Ƕ��ʽ������ ourstm.5d6d.com
*
* �������� �ܶ���STM32������V2,2.1,V3,MINI�ϵ���ͨ��           
* QQ: 9191274, ������sun68, Email: sun68@163.com 
* �Ա����̣�ourstm.taobao.com  
*
* �ļ���: app.c
* ���ݼ���:
*       �����̲���ϵͳ����ucos2.86a�汾�� ������5������
			������											 ���ȼ�
			APP_TASK_START_PRIO                               2	        ������	  		
           
            Task_Led1_PRIO                                    7			LED1 ��˸����
            Task_Led2_PRIO                                    8			LED2 ��˸����
            
		 ��Ȼ��������ϵͳ����
		    OS_TaskIdle                  ��������-----------------���ȼ����
			OS_TaskStat                  ͳ������ʱ�������-------���ȼ��ε�
*
* �ļ���ʷ:
* �汾��  ����       ����    ˵��
* v0.1    2010-11-12 sun68  �������ļ�
*
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#define GLOBALS

#include "stdarg.h"
 
#include "includes.h"
#include "globals.h"

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

static  OS_STK Task_Led1Stk[Task_Led1_STK_SIZE];
//static  OS_STK Task_Led2Stk[Task_Led2_STK_SIZE];
		

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/
static  void App_TaskCreate(void);

static  void App_TaskStart(void* p_arg);

static  void Task_Led1(void* p_arg);
//static  void Task_Led2(void* p_arg);
//
//static  void App_TaskJoystick(void* p_arg);
#define LED_LED1_ON()   GPIO_SetBits(GPIOA, GPIO_Pin_2 );  	       //LED1  ��
#define LED_LED1_OFF()  GPIO_ResetBits(GPIOA, GPIO_Pin_2 ); 	   //LED1  ��

/*
#define LED_LED2_ON()   GPIO_SetBits(GPIOD, GPIO_Pin_6 );  	       //LED2  ��
#define LED_LED2_OFF()  GPIO_ResetBits(GPIOD, GPIO_Pin_6 ); 	   //LED2  ��
*/

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

void GPIO_Configuration(void);

int main(void)
{
   CPU_INT08U os_err;

   //��ֹCPU�ж�
   CPU_IntDis();
   
   //UCOS ��ʼ��
   OSInit();                                                   /* Initialize "uC/OS-II, The Real-Time Kernel".         */
   
		SystemInit();
		GPIO_Configuration();
	
   //Ӳ��ƽ̨��ʼ��
	
  /* 
	BSP_Init();                                                 Initialize BSP functions.
   */
	
   //Ĭ��LED��˸���500ms
   milsec1=200,milsec2=200;   
   //���������� ���ȼ����  ���������������һ����;��Ϊ���Ժ�ʹ��ͳ������
   os_err = OSTaskCreate((void (*) (void *)) App_TaskStart,	  		  		//ָ����������ָ��
                          (void *) 0,								  		//����ʼִ��ʱ�����ݸ�����Ĳ�����ָ��
               (OS_STK *) &App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],	//���������Ķ�ջ��ջ��ָ��   �Ӷ����µݼ�
               (INT8U) APP_TASK_START_PRIO);								//�������������ȼ�
   
   //ucos�Ľ��ļ�������0    ���ļ�������0-4294967295    ���ڽ���Ƶ��100hzʱ�� ÿ��497������¼��� 
   OSTimeSet(0);
   OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II).  */
                                                 /* Start multitasking (i.e. give control to uC/OS-II).  */
 
   return (os_err);
}

/*
 Config PortA Pin_2 to led
*/

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /*�Կ���LEDָʾ�Ƶ�IO�ڽ����˳�ʼ�������˿�����Ϊ������������������ٶ�Ϊ50Mhz��PA9,PA10�˿ڸ���Ϊ����1��TX��RX��
  ������ĳ������ʱ������Ӧ�������ڵĶ˿ڵ�ʱ�ӽ���ʹ�ܡ������޷����óɹ��������õ��˶˿�B��D,E�� ���Ҫ���⼸���˿ڵ�ʱ��
  ����ʹ�ܣ�ͬʱ�����õ�����IO�ڹ����������ô��ڡ���˻�Ҫʹ��AFIO�����ù���IO��ʱ�ӡ�*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOA , ENABLE); 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				                 //LED1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);					 
  
	/*
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_3;		 			//LED2, LED3
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	*/
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

   //��ʼ��ucosʱ�ӽ���
  // OS_CPU_SysTickInit();                                       /* Initialize the SysTick.       */

//ʹ��ucos ��ͳ������
#if (OS_TASK_STAT_EN > 0)
   //----ͳ�������ʼ������  
   OSStatInit();                                               /* Determine CPU capacity.                              */
#endif
   //��������������
   App_TaskCreate();

   while (1)
   {
      //1��һ��ѭ��
	  OSTimeDlyHMSM(0, 0,10, 0);

   }
}


//LED1��˸����----------------------------------------
static  void Task_Led1(void* p_arg)
{
   


   (void) p_arg;	    
   while (1)
   {
      LED_LED1_ON();
      OSTimeDlyHMSM(0, 0, 0, milsec1);
      
      LED_LED1_OFF();
      OSTimeDlyHMSM(0, 0, 0, milsec1);	
   }
}

//LED2��˸����----------------------------------------
/*
static  void Task_Led2(void* p_arg)
{  

   (void) p_arg;	    
   while (1)
   {
      LED_LED2_ON();
      OSTimeDlyHMSM(0, 0, 0, milsec2);
      
      LED_LED2_OFF();
      OSTimeDlyHMSM(0, 0, 0, milsec2);	
   }
}
*/
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
    //LED1 ��˸����------------------------------------------------------
   OSTaskCreateExt(Task_Led1,(void *)0,(OS_STK *)&Task_Led1Stk[Task_Led1_STK_SIZE-1],Task_Led1_PRIO,Task_Led1_PRIO,(OS_STK *)&Task_Led1Stk[0],
                    Task_Led1_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);
   
   //LED2 ��˸����------------------------------------------------------
	/*
   OSTaskCreateExt(Task_Led2,(void *)0,(OS_STK *)&Task_Led2Stk[Task_Led2_STK_SIZE-1],Task_Led2_PRIO,Task_Led2_PRIO,(OS_STK *)&Task_Led2Stk[0],
                    Task_Led2_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR); 
										*/
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
