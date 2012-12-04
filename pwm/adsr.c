/* 
   Direct-digital synthesis
   ADSR Demo
   
*/

// ------- Preamble -------- //
#include <avr/io.h>		/* Defines pins, ports, etc */
#include <util/delay.h>		/* Functions to waste time */
#include <avr/interrupt.h>	
#include "pinDefines.h"
#include "macros.h"
#include "scale.h"

#define SPEED           32   /* powers of two are good values. */

static inline void initTimer0(void){
  set_bit(TCCR0A, COM0A1);	/* PWM output on OCR0A */
  set_bit(SPEAKER_DDR, SPEAKER); /* enable output on pin */

  set_bit(TCCR0A, WGM00);	/* Fast PWM mode */
  set_bit(TCCR0A, WGM01);	/* Fast PWM mode, pt.2 */
  
  set_bit(TCCR0B, CS00);	/* Clock with /1 prescaler */
}

static inline void initLEDs(void){
  uint8_t i;
  LED_DDR = 0xff;	/* All LEDs for diagnostics */
  for (i=0; i<8; i++){
    set_bit(LED_PORT, i);
    _delay_ms(100);
    clear_bit(LED_PORT, i);
  }
}

// Note: signed integer here: -128 to 127
static inline int8_t triangle(uint8_t waveStep){
  uint8_t triangleValue;
  if (waveStep < 64){		/* 0..63 -> 1..127 */
    triangleValue = (2*waveStep)+1;
  }
  else if (waveStep < 192){ 	/* 64..191 -> 126..-128 */
    triangleValue =  126 - 2*(waveStep - 64);
  }
  else {			/* 192..255 -> -127..-1 */
    triangleValue =  -127 + 2*(waveStep - 192);
  }
  return(triangleValue);
}

int main(void){

  uint16_t accumulators[4] = {0,0,0,0};  
  uint16_t tuningWords[4];
  uint8_t  volumes[4] = {31,31,31,31};
  /* signed output makes math easier */
  int16_t mixer;

  uint8_t waveStep;
  uint8_t i;
  uint16_t cycleCount=0;
  uint8_t longCount=0;

  // -------- Inits --------- //
  
  initLEDs();
  initTimer0();
  
  set_bit(BUTTON_PORT, BUTTON);	/* pullup on button */
  set_bit(SPEAKER_DDR, SPEAKER); /* speaker output */
  
  // Notes are a C Maj chord
  tuningWords[0] = C2;
  tuningWords[1] = E2;
  tuningWords[2] = G2;
  tuningWords[3] = C3;
  
  accumulators[0] = 0;
  accumulators[1] = 0;
  accumulators[2] = 0;
  accumulators[3] = 0;
  
  // ------ Event loop ------ //
  while(1){		       
    
    set_bit(LED_PORT, LED0);		/* debugging -- begins wait time */
    loop_until_bit_is_set(TIFR0, TOV0); /* wait for timer0 overflow */
    clear_bit(LED_PORT, LED0);		/* debugging -- ends wait time */
    set_bit(TIFR0, TOV0);		/* reset the overflow bit */
    OCR0A = 128 + mixer;			/* update the value */

    /* Update all accumulators, mix together */
    
    mixer = 0;
    for (i=0; i < 4; i++){
      accumulators[i] += tuningWords[i]; /* accumulator update */
      waveStep = (uint8_t) (accumulators[i] >> 8); 
      // Triangle wave in code:
      mixer += triangle(waveStep) * volumes[i];
    }
    mixer = mixer >> 5;		/* 5-bit volume */
    mixer = mixer >> 2;		/* quick divide by 4 voices */
    

  } /* End event loop */
  return(0);		      /* This line is never reached  */
}

