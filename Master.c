#include <util/delay.h>
#include <stdio.h>
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


  printf("Sending something\n");
  _delay_ms(5000); // from delay.h, wait 1 sec
  uint8_t command[3];
  command[0] = (1<<1); //Address or general call + W bit (0)
  command[1] =  SET;// Sample, get ,set or apply
  command[2] = 0b00001010;//Pins to set
  TWIMasterTransmitData(command,3,0);
  printf("\n");
  
  
}
