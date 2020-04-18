/*	Author: huryan18
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
// volatile unsigned char TimerFlag = 0;
enum States {start, inactive, wait_inc, wait_dec, wait_reset} state;


// void TimerSR() {TimerFlag = 1;}

void tick(){
      switch(state){
            case start:
                  state = inactive;
                  break;
            case inactive:
                  if((PINA & 0x01) && (PINA & 0x02)){
                        PORTC = 0;
                        state = wait_reset;
                  }
                  else if(PINA & 0x01){
                        PORTC++;
                        state = wait_inc;
                  }
                  else if(PINA & 0x02){
                        PORTC--;
                        state = wait_dec;
                  }
                  else
                        state = inactive;

                  break;
            case wait_inc:
                  state = (PINA & 0x01)? wait_inc : inactive;
                  break;
            case wait_dec:
                  state = (PINA & 0x02)? wait_dec : inactive;
                  break;
            case wait_reset:
                  state = ((PINA & 0x01) || (PINA & 0x02))? wait_reset : inactive;
                  break;
            default: state = start;
                  break;
      }
}


int main(void) {
      /* Insert DDR and PORT initializations */
      DDRA = 0x00; PORTA = 0xFF;
      // DDRB = 0xFF; PORTB = 0x00;
      DDRC = 0xFF; PORTC = 7;
      /* Insert your solution below */
      state = start;
      while (1) {
          tick();
      }
      return 1;
}
