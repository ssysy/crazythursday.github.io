/* 
 * File:   uart1.h
 * Author: 86166
 *
 * Created on 22 November 2024, 15:56
 */

#ifndef UART1_H
#define UART1_H

// TX(PB3) RX(PB4)

void UART1_init(int prescale);

void UART1_send( unsigned char data);

void UART1_putstring(char* StringPtr);

unsigned char UART1_Receive(void);

#endif