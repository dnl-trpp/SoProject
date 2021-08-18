#include <util/delay.h>
#include <stdlib.h>
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
  
  //m sample s set a apply g get
  while(1){
    printf("Waiting for command...\n");
    char usartData=usart_getchar(); 
    uint8_t TXData[3];
    char addr[2];
    addr[1]='\0';
    switch(usartData){

      case 's':
        addr[0] =usart_getchar();
        TXData[0] = (atoi(addr)<<1); //Address + W bit (0)
        TXData[1] =  SET;                 // Sample, get ,set or apply
        TXData[2] = usart_getchar();      //Pins to set
        TWIMasterTransmitData(TXData,3,0);
        while(!isTWIReady()) {_delay_us(1);}
        if(getTWIErrorCode() == TWI_SUCCESS){
            printf("Successfully transmitted\n");
        }
        else{
            printf("Something went wrong: %x\n", getTWIErrorCode());
        }
        break;
      

      case 'm':
        TXData[0] =   0x00;                  //General call + W bit (0)
        TXData[1] =  SAMPLE;                 // Sample, get ,set or apply
        TXData[2] =   0x00;                  //No payload for SAMPLE
        TWIMasterTransmitData(TXData,3,0);
        while(!isTWIReady()) {_delay_us(1);}
        if(getTWIErrorCode() == TWI_SUCCESS){
            printf("Successfully transmitted\n");
        }
        else{
          printf("Something went wrong: %x\n", getTWIErrorCode());
        }
        break;


      case 'g':
        addr[0] =usart_getchar();
        TXData[0] = (atoi(addr)<<1); //Address + W bit (0)
        TXData[1] =  GET;                 // Sample, get ,set or apply
        TXData[2] = 0x00;                 //No payload for Get
        TWIMasterTransmitData(TXData,3,0);
        while(!isTWIReady()) {_delay_us(1);}
        _delay_ms(100);
        TWIMasterReadData(1,1,0);
        while(!isTWIReady()) { _delay_us(1);}
        if(getTWIErrorCode() == TWI_SUCCESS){    
            printf("Successfully Got data: %x\n",TWIReceiveBuffer[0]);
        }
        else{
          printf("Something went wrong: %x\n", getTWIErrorCode());
        }
        break;


      case 'a':
        TXData[0] =   0x00;                  //General call + W bit (0)
        TXData[1] =  APPLY;                 // Sample, get ,set or apply
        TXData[2] =   0x00;                  //No payload for APPLY
        TWIMasterTransmitData(TXData,3,0);
        while(!isTWIReady()) {_delay_us(1);}
        if(getTWIErrorCode() == TWI_SUCCESS){
            printf("Successfully transmitted\n");
        }
        else{
          printf("Something went wrong: %x\n", getTWIErrorCode());
        }
        break;
      
      default:
         printf("Unknown command\n");
    }
  }
}


