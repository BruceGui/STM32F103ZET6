
#include "ownprintf.h"
#include <stm32f10x.h>

void UartPutchar(char Txdata)
{
  
  if(Txdata == '\n') {
      USART1->DR   = '\r';
      while(!(USART1->SR & 0x40));
  }
  
  USART1->DR   = Txdata;
    while(!(USART1->SR & 0x40));
  
}

void UartSendString(char *pt)
{
    while(*pt)
      UartPutchar(*pt++);
}

void IntToString(int temp, char *pchar)
{
    char ch, *p = pchar;
    
    while(temp != 0) {
        *pchar++ = (char)(temp%10 + 0x30);
        temp /= 10;
    }
    
    *pchar-- = '\0';
    
    while(pchar > p) {
        ch = *p;
        *p++ = *pchar;
        *pchar-- = ch;
    }
}

void Vs_Printf(char *s, char *format, va_list arg)
{
    char *pchar;
    char *temp;
    
    for(pchar=format; *pchar; pchar++) {
      if(*pchar != '%') {
        *s++ = *pchar;
        continue;
      }
      
      switch(*++pchar) {
      case 'd': {
        IntToString(va_arg(arg, int), s);
        while(*s++);
        *--s = '0';
        break;
      }
      
      case 's': {
        temp = va_arg(arg, char *);
        while(*s)
          *s++ = *temp++;
        *--s = '0';
        break;
      }
      
      case 'c': {
        *s++ = va_arg(arg, int);
        break;
      }
      default:
        break;
      }
    }
    
    *s = '\0';
}

void ownprintf(char *fmt, ...)
{
    va_list ap;
    char string[256];
    
    va_start(ap, fmt);
    
    Vs_Printf(string, fmt, ap);
    UartSendString(string);
    
    va_end(ap);
}

