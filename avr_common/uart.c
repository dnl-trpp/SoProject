// ********************************************************************************
// Includes
// ********************************************************************************
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
#include "uart.h"

void usart_init(uint16_t ubrr);


// ********************************************************************************
// usart Related
// ********************************************************************************
void usart_init( uint16_t ubrr) {
    // Set baud rate
    UBRR0H = (uint8_t)(ubrr>>8);
    UBRR0L = (uint8_t)ubrr;

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */ 
    UCSR0B = _BV(RXEN0) | _BV(TXEN0) |_BV(RXCIE0); //| _BV(UDRIE0);   /* Enable RX and TX */  

    USART_TXBuffIndex = 0;
    USART_RXBuffIndex = 0;
    USART_RXBuffLen = 0;
}
void usart_putchar(char data) {
    // Wait for empty transmit buffer
    USART_TransmitBuffer[0] = data;
    USART_TXBuffLen = 1;
    USART_TXBuffIndex = 0;
    UCSR0B |=_BV(UDRIE0); //Enable Transmit buffer interrupt (fires immediatly)

}
//tx interrupt service routine
ISR(USART0_UDRE_vect) {
  // copies values from common tx_buffer to UART register
  if(USART_TXBuffIndex<USART_TXBuffLen){
     UDR0 = USART_TransmitBuffer[USART_TXBuffIndex++]; 
  }else{
     UCSR0B &= ~(_BV(UDRIE0)); //Disable Transmit buffer interrupt
  }
}

char usart_getchar(void) {
    // Wait for incoming data
    while ( USART_RXBuffLen==0);
    // Return the data
    return USART_ReceiveBuffer[(USART_RXBuffIndex-USART_RXBuffLen--)%20];

}

// rx interrupt service routine
ISR(USART0_RX_vect) {
  // read the UART register UDR0
  uint8_t c = UDR0;
  // copies values from UART register to common rx_buffer
  if(USART_RXBuffIndex>=20) USART_RXBuffIndex=0;
  USART_ReceiveBuffer[USART_RXBuffIndex++] = c;
  if(USART_RXBuffLen<20) USART_RXBuffLen++;
}

void usart_init_default(void){
  
  // fire up the usart
  usart_init ( MYUBRR );
}


