/*
 * Author: Bruce Gui
 * Date  : 2014-06-27
 */

#ifndef _ILI_H
#define _ILI_H

#include <stm32f10x.h>

#define LCD_DATA(data) do{GPIOD->ODR |= (data)}while(0)

#define LCD_RS_CLR   do{GPIOC->ODR &= ~(1 << 0);}while(0)
#define LCD_RS_SET   do{GPIOA->ODR |=  (1 << 0);}while(0)

#define LCD_WR_CLR   do{GPIOC->ODR &= ~(1 << 1);}while(0)
#define LCD_WR_SET   do{GPIOA->ODR |=  (1 << 1);}while(0)

#define LCD_RD_CLR   do{GPIOC->ODR &= ~(1 << 2);}while(0)
#define LCD_RD_SET   do{GPIOA->ODR |=  (1 << 2);}while(0)

#define LCD_RST_CLR  do{GPIOC->ODR &= ~(1 << 3);}while(0)
#define LCD_RST_SET  do{GPIOA->ODR |=  (1 << 3);}while(0)

#define LCD_CS_CLR   do{GPIOC->ODR &= ~(1 << 4);}while(0)
#define LCD_CS_SET   do{GPIOA->ODR |=  (1 << 4);}while(0)

#endif
