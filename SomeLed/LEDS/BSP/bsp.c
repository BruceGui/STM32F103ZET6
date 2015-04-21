#include "..\App\includes.h"
//extern void USART_Config(USART_TypeDef* USARTx,u32 baud);
/*
*********************************************************************************************************

*/

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

////////////////////////////////////////////////////////
void RCC_Configuration(void)
{	
	
	
  SystemInit(); 

}

//关闭调试接口，作GPIO使用
void UnableJTAG(void)
{
   
   RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;                     // enable clock for Alternate Function
   AFIO->MAPR &= ~(7UL<<24); // clear used bit
   AFIO->MAPR |= (4UL<<24); // set used bits
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : PB5: LED1 (mini and V3)
                   PD6：LED2 (only V3)
				   PD3：LED3 (only V3)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /*对控制LED指示灯的IO口进行了初始化，将端口配置为推挽上拉输出，口线速度为50Mhz。PA9,PA10端口复用为串口1的TX，RX。
  在配置某个口线时，首先应对它所在的端口的时钟进行使能。否则无法配置成功，由于用到了端口B，D,E， 因此要对这几个端口的时钟
  进行使能，同时由于用到复用IO口功能用于配置串口。因此还要使能AFIO（复用功能IO）时钟。*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOA , ENABLE); 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				                 //LED1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);					 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_3;		 			//LED2, LED3
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}


/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{

  //EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  #if defined (VECT_TAB_RAM)
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#elif defined(VECT_TAB_FLASH_IAP)
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x2000);
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif 

  /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

  
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			     	//设置串口1中断
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
 
}

void BSP_Init(void)
{
  /* System Clocks Configuration --72M*/
  //RCC_Configuration();   

  GPIO_Configuration();
  
  /* NVIC configuration */
  /*嵌套向量中断控制器 
      说明了USART1抢占优先级级别0（最多1位） ，和子优先级级别0（最多7位） */ 
  //NVIC_Configuration();
  
  //USART_Config(USART1,115200);		  //串口1初始化
  
}



////SYSTICK 配置， 10ms一次systick中断，产生ucos 调度节拍， 1KHz
void  OS_CPU_SysTickInit(void)
{
    RCC_ClocksTypeDef  rcc_clocks;
    INT32U         cnts;


    RCC_GetClocksFreq(&rcc_clocks);

    cnts = (INT32U)rcc_clocks.HCLK_Frequency/OS_TICKS_PER_SEC;

	SysTick_Config(cnts);

  
}


#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

