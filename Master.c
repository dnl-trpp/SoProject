#include <util/delay.h>
#include <stdio.h>
#include "./avr_common/uart.h" // this includes the printf and initializes it
#include "TWIlib.h"

int main(void){
  // this initializes the printf/uart thingies
  printf_init(); 
  
  int k=0;
  while(1){
    printf("hello %d\n", F_CPU);
    _delay_ms(1000); // from delay.h, wait 1 sec
  }
  
}
