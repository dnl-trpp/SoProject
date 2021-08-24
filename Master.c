#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "./avr_common/uart.h"
#include "TWIlib.h"
#define SAMPLE 1
#define GET 2
#define SET 3
#define APPLY 4

int main(void){
  cli();
  usart_init_default();  // initializes uart
  TWIInitMaster();
  sei();
  
  while(1){
 
    char usartData=usart_getchar(); 
    uint8_t TXData[4];
    uint8_t addr;

    switch(usartData){

      case 's':
        
        TXData[0] = (usart_getchar()<<1);  //Address + W bit (0)
        TXData[1] =  SET;                 // Sample, get ,set or apply
        TXData[2] = usart_getchar();      //Pins to set
        TXData[3] = TXData[1]+TXData[2];  //Checksum
        TWIMasterTransmitData(TXData,4,0);
        while(!isTWIReady()) {_delay_us(1);}
        if(getTWIErrorCode() == TWI_SUCCESS){
            usart_putchar(0xFF);
        }
        else{
            usart_putchar(getTWIErrorCode());
        }
        break;
      

      case 'm':
        TXData[0] =   0x00;                  //General call + W bit (0)
        TXData[1] =  SAMPLE;                 // Sample, get ,set or apply
        TXData[2] =   0x00;                  //No payload for SAMPLE
        TXData[3] = TXData[1]+TXData[2];  //Checksum
        TWIMasterTransmitData(TXData,4,0);   
        while(!isTWIReady()) {_delay_us(1);}
        if(getTWIErrorCode() == TWI_SUCCESS){
            usart_putchar(0xFF);
        }
        else{
            usart_putchar(getTWIErrorCode());
        }
        break;


      case 'g':

        addr = usart_getchar();
        TXData[0] = (addr<<1);            //Address + W bit (0)
        TXData[1] =  GET;                 // Sample, get ,set or apply
        TXData[2] = 0x00;                 //No payload for Get
        TXData[3] = TXData[1]+TXData[2];  //Checksum
        TWIMasterTransmitData(TXData,4,0);
        while(!isTWIReady()) {_delay_us(1);}
        if(getTWIErrorCode() == TWI_SUCCESS){
          _delay_ms(100);
          TWIMasterReadData(addr,1,0);
          while(!isTWIReady()) { _delay_us(1);}
          if(getTWIErrorCode() == TWI_SUCCESS){    
              usart_putchar(0xFF);
              usart_putchar(TWIReceiveBuffer[0]);
          }
          else{
              usart_putchar(getTWIErrorCode());
          }
        }else{
           usart_putchar(getTWIErrorCode());
        }
        
        break;


      case 'a':
        TXData[0] =   0x00;                  //General call + W bit (0)
        TXData[1] =  APPLY;                 // Sample, get ,set or apply
        TXData[2] =   0x00;                  //No payload for APPLY
        TXData[3] = TXData[1]+TXData[2];  //Checksum
        TWIMasterTransmitData(TXData,4,0);
        while(!isTWIReady()) {_delay_us(1);}
        if(getTWIErrorCode() == TWI_SUCCESS){
            usart_putchar(0xFF);
        }
        else{
            usart_putchar(getTWIErrorCode());
        }
        break;
      
      default:
            usart_putchar(0xFF);
    }
  }
}


