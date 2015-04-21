
#ifndef OWN_PRINTF
#define OWN_PRINTF

#include <stdarg.h>

void IntToString(int temp, char *pchar);
void Vs_Printf(char *s, char *format, va_list arg);
void ownprintf(char *fmt, ...);
void UartPutchar(char Txdata);
void UartSendString(char *pt);

void USART1_IRQHandler(void);

#endif
