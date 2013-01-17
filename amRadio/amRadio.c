/*  

Plays a simple tune, broadcasts it in the AM radio band.
Basically just showing off with timers.

*/

// ------- Preamble -------- //
#include <avr/io.h>		/* Defines pins, ports, etc */
#include <util/delay.h>		/* Functions to waste time */
#include "pinDefines.h"
#include "macros.h"
#include "scale16.h"

#define COUNTER_VALUE   5  	/* determines carrier frequency */
// From f = f_cpu / ( 2* N* (1 + OCRnx) )
// Good values for the AM band from 2 to 8: pick one that's clear
// (1+OCRnx) in denominator b/c counts up from 0 -- 0, 1, 2, 3

// Divide by two b/c we're toggling on or off each loop; 
//  a full cycle of the carrier takes two loops.

// These are theoretical values, my chip apparently runs faster
//  and they measure a little higher
// 8Mhz / (2 * 1 * (1+3)) = 1000 kHz
// 8Mhz / (2 * 1 * (1+4)) = 800 kHz
// 8Mhz / (2 * 1 * (1+5)) = 670 kHz
// 8Mhz / (2 * 1 * (1+7)) = 500 kHz

#define NOTE_LENGTH  9000

static inline void initTimer(void){

  set_bit(TCCR0A, WGM01);	/* CTC mode */
  set_bit(TCCR0A, COM0B0);	/* Toggles pin each time through */
  set_bit(TCCR0B, CS00);	/* Clock at CPU frequency, ~8MHz */

  OCR0A = COUNTER_VALUE;	/* carrier frequency */
}

static inline void transmitBeep(uint16_t pitch){
  uint16_t elapsed;
  uint16_t i;
  for(elapsed = 0; elapsed < NOTE_LENGTH; elapsed += pitch){
    for (i = 0; i < pitch; i++){ /* delay for pitch cycles */
      asm volatile("nop"::);	
      asm volatile("nop"::); /* eight no-operations takes 1us at 8MHz */
      asm volatile("nop"::);	
      asm volatile("nop"::);	
      asm volatile("nop"::);	
      asm volatile("nop"::);	
      asm volatile("nop"::);	
      asm volatile("nop"::);	
    }    
    toggle_bit(ANTENNA_DDR, ANTENNA);
  }
  set_bit(ANTENNA_DDR, ANTENNA);  /* back on full */
}

int main(void){
  
  uint8_t i;

  // -------- Inits --------- //
  initTimer();
  
  // ------ Event loop ------ //
  while(1){	
    
    transmitBeep(E2);
    _delay_ms(2);
    transmitBeep(E2);
    _delay_ms(200);
    transmitBeep(E2);
    _delay_ms(200);
    transmitBeep(C2);
    transmitBeep(E2);
    _delay_ms(200);
    transmitBeep(G2);
    transmitBeep(G2);
    _delay_ms(500);
    transmitBeep(G1);
    transmitBeep(G1);
    
    _delay_ms(300);
    _delay_ms(300);
    _delay_ms(300);
    _delay_ms(300);

  } /* End event loop */
  return(0);		      /* This line is never reached  */
}




