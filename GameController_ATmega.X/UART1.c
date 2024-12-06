/* 
 * File:   uart1.c
 * Author: 86166
 *
 * Created on 22 November 2024, 15:56
 */

#include <stdio.h>
#include <stdlib.h>

#include "UART1.h"
#include <avr/io.h>

void UART1_init(int BAUD_PRESCALER)
{
  /*Set baud rate */
  UBRR1H = (unsigned char)(BAUD_PRESCALER>>8);
  UBRR1L = (unsigned char)BAUD_PRESCALER;
  //Enable receiver and transmitter 
  UCSR1B = (1<<RXEN1)|(1<<TXEN1);
  /* Set frame format: 2 stop bits, 8 data bits */
  UCSR1C = (1<<UCSZ11) | (1<<UCSZ10); // 8 data bits
  UCSR1C |= (1<<USBS1); // 2 stop bits
}


void UART1_send(unsigned char data)// send data
{
  // Wait for empty transmit buffer
  while(!(UCSR1A & (1<<UDRE1)));
  // Put data into buffer and send data
  UDR1 = data;
  
}

void UART1_putstring(char* StringPtr)
{
  while(*StringPtr != 0x00)
  {
    UART1_send(*StringPtr);
    StringPtr++;
  }
}

unsigned char UART1_Receive(void) {//receive data
    while (!(UCSR1A & (1<<RXC1))); // Wait for data to be received
    return UDR1;
}
