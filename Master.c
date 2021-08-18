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
  printf("Ready to test SET...\n");
  usart_getchar(); 

  uint8_t command[3];
  command[0] = (1<<1); //Address or general call + W bit (0)
  command[1] =  SET;// Sample, get ,set or apply
  command[2] = 0b00001101;//Pins to set
  TWIMasterTransmitData(command,3,0);
   while(!isTWIReady()) {
     printf("mode:%x,status:%x\n",TWIInfo.mode,getTWIErrorCode());
     _delay_us(1);}
   if(getTWIErrorCode() == TWI_SUCCESS){
      printf("Successfully transmitted");
   }
   else{
     printf("Something went wrong: %x", getTWIErrorCode());
   }

     //printf("Send char to Sending something\n");
  printf("Ready to test GET...\n");
  usart_getchar(); 

  command[0] = (1<<1); //Address or general call + W bit (0)
  command[1] =  GET;// Sample, get ,set or apply
  command[2] = 0x00;//Pins to set
  TWIMasterTransmitData(command,3,0);
   while(!isTWIReady()) {
     printf("mode:%x,status:%x\n",TWIInfo.mode,getTWIErrorCode());
     _delay_us(1);}
   if(getTWIErrorCode() == TWI_SUCCESS){
      printf("Successfully transmitted");
   }
   else{
     printf("Something went wrong: %x", getTWIErrorCode());
   }
   _delay_ms(1000);
   
   TWIMasterReadData(1,1,0);
    while(!isTWIReady()) {
     printf("mode:%x,status:%x\n",TWIInfo.mode,getTWIErrorCode());
     _delay_us(1);}
   if(getTWIErrorCode() == TWI_SUCCESS){
      printf("Successfully Got data: %x",TWIReceiveBuffer[0]);
   }
   else{
     printf("Something went wrong: %x", getTWIErrorCode());
   }
  printf("END");
}
