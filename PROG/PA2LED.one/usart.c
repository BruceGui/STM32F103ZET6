/*
 *  Purpose: The implement of USART function
 *  Author:  Bruce Gui
 *  Date  :  2014-06-26
 */
 
#include <stm32f10x.h>
#include <stdio.h>
#include "usart.h"

void buffer_Init(void)
{
    tbuf.in    = 0;
    tbuf.out   = 0;
    tx_restart = 1;
    
    rbuf.in    = 0;
    rbuf.out   = 0;
}

int SendChar(int c)
{
    struct buf_st *p = &tbuf;
    
    if(SIO_TBUFLEN >= TBUF_SIZE)
        return -1;
    
    p->buf [p->in & (TBUF_SIZE - 1)] = c;
    p->in ++;
    
    if(tx_restart) {
        tx_restart = 0;
        USART1->CR1 |= USART_CR1_TXEIE;
    }
    
    return 0;
}

int GetKey(void)
{
    struct buf_st *p = &rbuf;
    
    if(SIO_RBUFLEN == 0)
        return -1;
    
    return p->buf[(p->out++) & (RBUF_SIZE - 1)];
}


void USART1_IRQHandler(void)
{
    volatile unsigned int IIR;
    
    struct buf_st *p;
    
    IIR = USART1->SR;
	
	/*
	printf("USART_IRQ\r\n");
	*/
    
    if(IIR & USART_SR_RXNE) {
        USART1->SR &= ~USART_SR_RXNE;
        
        p = &rbuf;
        
        if(((p->in - p->out) & ~(RBUF_SIZE)) == 0) {
            p->buf[p->in & (RBUF_SIZE - 1)] = (USART1->DR & 0x1FF);
            p->in ++;
        }
    }
    
    if (IIR & USART_SR_TXE) {
      USART1->SR &= ~USART_SR_TXE;	          // clear interrupt

      p = &tbuf;

      if (p->in != p->out) {
        USART1->DR = (p->buf [p->out & (TBUF_SIZE-1)] & 0x1FF);
        p->out++;
        tx_restart = 0;
      }
      else {
        tx_restart = 1;
		USART1->CR1 &= ~USART_CR1_TXEIE;		      // disable TX interrupt if nothing to send

      }
    }
}

