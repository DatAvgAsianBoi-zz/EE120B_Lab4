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
enum States {start, pb_0, wait0_1, pb_1, wait1_0} state;


// void TimerSR() {TimerFlag = 1;}

void tick(){
      switch(state){
            case start:
                  state = pb_0;
                  break;
            case pb_0:
                  state = (PINA & 0x01)? wait0_1 : pb_0;
                  break;
            case wait0_1:
                  state = (PINA & 0x01)? wait0_1 : pb_1;
                  break;
            case pb_1:
                  state = (PINA & 0x01)? wait1_0 : pb_1;
                  break;
            case wait1_0:
                  state = (PINA & 0x01)? wait1_0 : pb_0;
                  break;
            default: state = start;
                  break;
      }
      switch(state){
            case pb_0:        PORTB = 0x01; break;
            case wait0_1:     PORTB = 0x02; break;
            case pb_1:        PORTB = 0x02; break;
            case wait1_0:     PORTB = 0x01; break;
      }
}


int main(void) {
      /* Insert DDR and PORT initializations */
      DDRA = 0x00; PORTA = 0xFF;
      DDRB = 0xFF; PORTB = 0x00;
      /* Insert your solution below */
      state = start;
      while (1) {
          tick();
      }
      return 1;
}
