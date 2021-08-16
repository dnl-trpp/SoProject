#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "./avr_common/uart.h" // this includes the printf and initializes it
#include "TWIlib.h"
#define SAMPLE 1
#define GET 2
#define SET 3
#define APPLY 4

int main(void){
  // this initializes the printf/uart thingies
  printf_init(); 
  TWIInitMaster();
  sei();

  //printf("Send char to Sending something\n");
  //printf("Ready\n");
  char get=usart_getchar(); 
  uint8_t command[3];
  command[0] = (1<<1); //Address or general call + W bit (0)
  command[1] =  SET;// Sample, get ,set or apply
  command[2] = 0b00001010;//Pins to set
  TWIMasterTransmitData(command,3,0);
  printf("Hello: %c\n",get);
  //printf("Char received: %c, sent command\n",get);
  
  
}
