#pragma once

// ********************************************************************************
// Macros and Defines
// ********************************************************************************
#define BAUD 19600
#define MYUBRR F_CPU/16/BAUD-1


volatile uint8_t USART_TransmitBuffer[20];
volatile uint8_t USART_ReceiveBuffer[20];
volatile int USART_TXBuffIndex;
volatile int USART_RXBuffIndex;
volatile int USART_TXBuffLen; 
volatile int USART_RXBuffLen;
// ********************************************************************************
// Function Prototypes
// ********************************************************************************
char usart_getchar( void );
void usart_putchar( char data );
void usart_init_default(void);
