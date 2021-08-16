#include <util/delay.h>
#include <stdio.h>
#include <avr/sleep.h>
#include "./avr_common/uart.h" // this includes the printf and initializes it
#include "TWIlib.h"
#define ADDR 1
#define SAMPLE 1
#define GET 2
#define SET 3
#define APPLY 4

int main(void){
  // this initializes the printf/uart thingies
  printf_init(); 
  DDRB |= 0xFF; //Port B used as output
  PORTB =  0;
  TWIInitSlave(ADDR);

  TWISlaveReadData(2);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_mode();
  
  while(!isTWIReady()) {_delay_us(1);}
  if(getTWIErrorCode()==TWI_SUCCESS){
    uint8_t command= TWIReceiveBuffer[0];
    uint8_t payload= TWIReceiveBuffer[1];
    switch(command){
      case SET:
        PORTB = payload;
        printf("SET command received an PORTB set");
        break;
       
      case SAMPLE:

      case APPLY:

      case GET:
        break;
      default:
        printf("Unknown command");
    }
  }
  
}


