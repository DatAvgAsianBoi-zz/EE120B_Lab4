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
enum States {start, locked, wait_locked, wait_unlocked, unlocked} state;
unsigned char comb[4];
unsigned char button_press = 0;
unsigned char prev_button = 0x00;


// void TimerSR() {TimerFlag = 1;}
void resetComb(){
      comb[0] = 3;
      comb[1] = 3;
      comb[2] = 3;
      comb[3] = 3;
      button_press = 0;
}

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
      if(button_press != 4)
            return 0;
      if(comb[0] != 2)
            return 0;
      if(comb[1] != 0)
            return 0;
      if(comb[2] != 1)
            return 0;
      if(comb[3] != 0)
            return 0;
      return 1;
}

void tick(){
      switch(state){
            case start:
                  resetComb();
                  PORTB = 0x00;
                  state = locked;
                  prev_button = 0x00;
                  break;
            case locked:
                  prev_button = 0x00;
                  if(buttons_Pressed() == 1){
                        if(PINA & 0x80){
                              state = wait_locked;
                              prev_button = 0x80;
                        }
                        else{
                              if(PINA & 0x01){
                                    comb[button_press++] = 0;
                                    prev_button = 0x01;
                              }
                              else if(PINA & 0x02){
                                    comb[button_press++] = 1;
                                    prev_button = 0x02;
                              }
                              else if(PINA & 0x04){
                                    comb[button_press++] = 2;
                                    prev_button = 0x04;
                              }

                              if(checkComb()){
                                    state = wait_unlocked;
                                    resetComb();
                              }
                              else{
                                    state = wait_locked;
                                    if(button_press >= 4)
                                          resetComb();
                              }
                        }
                  }
                  else
                        state = locked;
                  break;

            case wait_locked:
                  if(buttons_Pressed() > 0){
                        if(buttons_Pressed() == 1){
                              if(PINA & 0x80){
                                    state = wait_locked;
                                    prev_button = 0x80;
                              }
                              else{
                                    unsigned char temp = prev_button;
                                    if((PINA & 0x01) && (0x01 != prev_button)){
                                          comb[button_press++] = 0;
                                          prev_button = 0x01;
                                    }
                                    if((PINA & 0x02) && (0x02 != prev_button)){
                                          comb[button_press++] = 1;
                                          prev_button = 0x02;
                                    }
                                    if((PINA & 0x04) && (0x04 != prev_button)){
                                          comb[button_press++] = 2;
                                          prev_button = 0x04;
                                    }

                                    if(temp != prev_button)
                                          if(checkComb()){
                                                state = wait_unlocked;
                                                resetComb();
                                          }
                                          else{
                                                state = wait_locked;
                                                if(button_press >= 4)
                                                      resetComb();
                                          }
                              }
                        }
                        else
                              state = wait_locked;
                  }
                  else
                        state = locked;
                  break;

            case wait_unlocked:
                  if(buttons_Pressed() > 0){
                        if(buttons_Pressed() == 1){
                              if(PINA & 0x80){
                                    state = wait_locked;
                                    prev_button = 0x80;
                              }
                              else{
                                    unsigned char temp = prev_button;
                                    if((PINA & 0x01) && (0x01 != prev_button)){
                                          comb[button_press++] = 0;
                                          prev_button = 0x01;
                                    }
                                    if((PINA & 0x02) && (0x02 != prev_button)){
                                          comb[button_press++] = 1;
                                          prev_button = 0x02;
                                    }
                                    if((PINA & 0x04) && (0x04 != prev_button)){
                                          comb[button_press++] = 2;
                                          prev_button = 0x04;
                                    }

                                    if(temp != prev_button)
                                          if(checkComb()){
                                                state = wait_locked;
                                                resetComb();
                                          }
                                          else{
                                                state = wait_unlocked;
                                                if(button_press >= 4)
                                                      resetComb();
                                          }
                              }
                        }
                        else
                              state = wait_unlocked;
                  }
                  else
                        state = unlocked;
                  break;

            case unlocked:
                  prev_button = 0x00;
                  if(buttons_Pressed() == 1){
                        if(PINA & 0x80){
                              state = wait_locked;
                              prev_button = 0x80;
                        }
                        else{
                              if(PINA & 0x01){
                                    comb[button_press++] = 0;
                                    prev_button = 0x01;
                              }
                              else if(PINA & 0x02){
                                    comb[button_press++] = 1;
                                    prev_button = 0x02;
                              }
                              else if(PINA & 0x04){
                                    comb[button_press++] = 2;
                                    prev_button = 0x04;
                              }

                              if(checkComb()){
                                    state = wait_locked;
                                    resetComb();
                              }
                              else{
                                    state = wait_unlocked;
                                    if(button_press >= 4)
                                          resetComb();
                              }
                        }
                  }
                  else
                        state = locked;
                  break;
            default: state = start;
                  break;
      }
      switch(state){
            case start:             PORTB = 0x00;     break;
            case locked:            PORTB = 0x00;     break;
            case wait_locked:       PORTB = 0x00;     break;
            case wait_unlocked:     PORTB = 0x01;     break;
            case unlocked:          PORTB = 0x01;     break;
      }
}


int main(void) {
      /* Insert DDR and PORT initializations */
      DDRA = 0x00; PORTA = 0xFF;
      DDRB = 0xFF; PORTB = 0x00;
      // DDRC = 0xFF; PORTC = 7;
      /* Insert your solution below */
      state = start;
      resetComb();
      PORTB = 0x00;
      while (1) {
          tick();
      }
      return 1;
}
