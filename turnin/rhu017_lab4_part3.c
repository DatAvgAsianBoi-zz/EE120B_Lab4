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
#include <string.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
// volatile unsigned char TimerFlag = 0;
enum States {start, locked, wait, unlocked} state;
unsigned char comb[2];
unsigned char button_press = 0;


// void TimerSR() {TimerFlag = 1;}
unsigned char buttons_Pressed(){
      unsigned char ret = 0;
      if(PINA & 0x01)
            ret++;
      if(PINA & 0x02)
            ret++;
      if(PINA & 0x04)
            ret++;
      if(PINA & 0x80)
            ret++;
      return ret;
}

unsigned char checkComb(){
      if(button_press != 2)
            return 0;
      if(comb[0] != 2)
            return 0;
      if(comb[1] != 1)
            return 0;
      return 1;
}

void tick(){
      switch(state){
            case start:
                  state = locked;
                  PORTB = 0x00;
                  break;
            case locked:
                  if(buttons_Pressed() == 1){
                        state = wait;
                        if(PINA & 0x01)
                              comb[button_press++] = 0;
                        else if(PINA & 0x02)
                              comb[button_press++] = 1;
                        else if(PINA & 0x04)
                              comb[button_press++] = 2;
                        else if(PINA & 0x80){
                              button_press = 0;
                              memset(comb, 0, 2);
                        }
                  }
                  else
                        state = locked;
                  break;
            case wait:
                  if(buttons_Pressed() > 0)
                        state = wait;
                  else
                        if(checkComb()){
                              state = unlocked;
                              button_press = 0;
                              memset(comb, 0, 2);
                        }
                        else
                              state = locked;
                  break;
            case unlocked:
                  if(buttons_Pressed() == 1){
                        state = wait;
                        if(PINA & 0x01)
                              comb[button_press++] = 0;
                        else if(PINA & 0x02)
                              comb[button_press++] = 1;
                        else if(PINA & 0x04)
                              comb[button_press++] = 2;
                        else if(PINA & 0x80){
                              button_press = 0;
                              memset(comb, 0, 2);
                        }
                  }
                  else
                        state = unlocked;
                  break;
            default: state = start;
                  break;
      }
      switch(state){
            case locked:      PORTB = 0x00;                                   break;
            case wait:        PORTB = checkComb() ? unlocked : locked;        break;
            case unlocked:    PORTB = 0x01;                                   break;
      }
}


int main(void) {
      /* Insert DDR and PORT initializations */
      DDRA = 0x00; PORTA = 0xFF;
      DDRB = 0xFF; PORTB = 0x00;
      // DDRC = 0xFF; PORTC = 7;
      /* Insert your solution below */
      state = start;
      button_press = 0;
      memset(comb, 0, 2);
      while (1) {
          tick();
      }
      return 1;
}
