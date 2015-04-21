/*
 * name:    usart.h
 * purpose: define some function on using usart 
 * 
 * Author:  Bruce Gui
 * Date:    2014-06-26
 */

#ifndef __USART_H 
#define __USART_H

#define TBUF_SIZE   256
#define RBUF_SIZE   256

struct buf_st {
    unsigned int in;
    unsigned int out;
    char buf [RBUF_SIZE];
};

static struct buf_st rbuf = {0, 0, };
#define SIO_RBUFLEN ((unsigned short) (rbuf.in - rbuf.out))
    
static struct buf_st tbuf = {0, 0, };
#define SIO_TBUFLEN ((unsigned short) (tbuf.in - tbuf.out))
    
static unsigned int tx_restart = 1;

void buffer_Init(void);
int  SendChar(int c);
int  GetKey(void);

#endif 


