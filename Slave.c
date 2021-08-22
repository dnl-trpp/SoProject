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

  DDRB |= 0xFF;  //Port B used as output
  DDRA &= 0x00;  //Port A used as input
  PORTB =  0;    //Set Port B to Low
  uint8_t sampled = 0;
  uint8_t toSet   = 0;

  TWIInitSlave(ADDR);
  sei();

  while(1){

      TWISlaveReadData(3);
      //set_sleep_mode(SLEEP_MODE_PWR_DOWN);
      sleep_mode();

      while(!isTWIReady()) {_delay_us(1);}

      if(getTWIErrorCode()==TWI_SUCCESS){

        uint8_t command= TWIReceiveBuffer[0];
        uint8_t payload= TWIReceiveBuffer[1];
        uint8_t checksum= TWIReceiveBuffer[2];
        
        if(((command+payload)&0xff)==checksum){
          switch(command){

            case SET:
              toSet = payload;
              break;
            
            case SAMPLE:
              sampled = PINA;
              break;
            
            case APPLY:
              PORTB = toSet;
              break;

            case GET: ;
              uint8_t data[1];
              data[0]= sampled;
              TWISlaveSendData(data,1);
              break;
      
          }
        }
    }
    _delay_ms(1);
  
  }
}


