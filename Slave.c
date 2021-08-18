#include <util/delay.h>
#include <stdio.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include "./avr_common/uart.h" // this includes the printf and initializes it
#include "TWIlib.h"
#define ADDR 1
#define SAMPLE 1
#define GET 2
#define SET 3
#define APPLY 4

int main(void){
  // this initializes the printf/uart thingies
  //printf_init(); 
  DDRB |= 0xFF; //Port B used as output
  DDRA &= 0x00;
  PORTB =  0;
  TWIInitSlave(ADDR);

  TWISlaveReadData(2);

  //printf("Going to sleep");
  _delay_ms(1000);
  sei();
  //set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_mode();

  while(!isTWIReady()) {
    //printf("I'm locked");
    _delay_us(1);}
  if(getTWIErrorCode()==TWI_SUCCESS){
    uint8_t command= TWIReceiveBuffer[0];
    uint8_t payload= TWIReceiveBuffer[1];
    switch(command){
      case SET:
        PORTB = payload;
        //printf("SET command received an PORTB set");
        break;
       
      case SAMPLE:
      
      case APPLY:

      case GET: ;
        PORTB = payload;
        uint8_t data[1];
        data[0]= PINA;
        TWISlaveSendData(data,1);
        break;
      //default:
        //printf("Unknown command");
    }
  }
  else{
    //printf("Something went wrong: %x", getTWIErrorCode());
  }
  
  TWISlaveReadData(2);

  //printf("Going to sleep");
  _delay_ms(1000);
  sei();
  //set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_mode();

  while(!isTWIReady()) {
    //printf("I'm locked");
    _delay_us(1);}
  if(getTWIErrorCode()==TWI_SUCCESS){
    uint8_t command= TWIReceiveBuffer[0];
    uint8_t payload= TWIReceiveBuffer[1];
    switch(command){
      case SET:
        PORTB = payload;
        //printf("SET command received an PORTB set");
        break;
       
      case SAMPLE:
      
      case APPLY:
        break;
      case GET:
        PORTB = payload;
        uint8_t data[1];
        data[0]= PINA;
        TWISlaveSendData(data,1);
        _delay_ms(10000);
        break;
      //default:
        //printf("Unknown command");
    }
  }
  else{
    //printf("Something went wrong: %x", getTWIErrorCode());
  }
}


