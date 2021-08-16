#pragma once

// ********************************************************************************
// Macros and Defines
// ********************************************************************************
#define BAUD 19600
#define MYUBRR F_CPU/16/BAUD-1
// ********************************************************************************
// Function Prototypes
// ********************************************************************************
char usart_getchar( void );
void usart_putchar( char data );
void usart_pstr (char *s);
unsigned char usart_kbhit(void);
void printf_init(void);
