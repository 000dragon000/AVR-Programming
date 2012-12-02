/* Direct-digital synthesis */

// ------- Preamble -------- //
#include <avr/io.h>		/* Defines pins, ports, etc */
#include <util/delay.h>		/* Functions to waste time */	

#include "pinDefines.h"
#include "macros.h"
#include "fullWaves.h"
#include "USART.h"
#include "scale.h"

static inline void initTimer0(void){
  set_bit(TCCR0A, COM0A1);	/* PWM output on OCR0A */
  set_bit(SPEAKER_DDR, SPEAKER); /* enable output on pin */

  set_bit(TCCR0A, WGM00);	/* Fast PWM mode */
  set_bit(TCCR0A, WGM01);	/* Fast PWM mode, pt.2 */
  
  set_bit(TCCR0B, CS00);	/* Clock with /1 prescaler */
}

static inline void pollButton(void){
  if (bit_is_clear(BUTTON_IN, BUTTON)) {
    set_bit(SPEAKER_DDR, SPEAKER); /* enable output on pin */
    clear_bit(LED_PORT, LED0);
  }
  else {
    clear_bit(SPEAKER_DDR, SPEAKER); /* disable output on pin */
    set_bit(LED_PORT, LED0);
  }
}

static inline void initLED(void){
  set_bit(LED_DDR, LED0);	/* LED on for diagnostics */
  set_bit(LED_PORT, LED0);
  _delay_ms(100);
  clear_bit(LED_PORT, LED0);
}

static inline void playNote(uint16_t tuningWord, uint16_t duration){
  uint16_t accumulator;  
  uint8_t  whichByte;
  uint8_t  pwmValue;
  uint16_t sampleCount;
  uint8_t volume;
  int16_t signedPWM;
  
  set_bit(SPEAKER_DDR, SPEAKER);
  for(sampleCount=0; sampleCount < duration; sampleCount++){

    loop_until_bit_is_set(TIFR0, TOV0); /* wait for overflow bit */
    OCR0A = pwmValue;	
    set_bit(TIFR0, TOV0);	        /* reset timer overflow bit */
    accumulator += tuningWord;	        /* take tuningWord steps */
    whichByte = (uint8_t) (accumulator >> 8);
    
    //pwmValue = fullSine[whichByte];
    //pwmValue = fullTriangle[whichByte];
    pwmValue = whichByte;	/* sawtooth */

    // 6-bit volume, simple function of elapsed time
    // Not very realistic, but quick to code up.
    volume = (1 << 6) - (uint8_t) (sampleCount >> 9);
    signedPWM = pwmValue - 128;	      /* center around 0 */
    signedPWM = (signedPWM * volume) >> 6; /* scale by volume */
    pwmValue =  128 + signedPWM;  /* re-center 0..255 */
  }
  clear_bit(SPEAKER_DDR, SPEAKER);
}

int main(void){


  // -------- Inits --------- //
  
  initLED();
  initTimer0();
  initUSART();

  set_bit(BUTTON_PORT, BUTTON);	/* pullup on button */
  set_bit(SPEAKER_DDR, SPEAKER); /* speaker output */
  	
  
  // ------ Event loop ------ //
  while(1){		       

    playNote(C0, 256*20);
    _delay_ms(20);
    playNote(D0, 256*20);
    _delay_ms(20);
    playNote(E0, 256*20);
    _delay_ms(20);
    playNote(F0, 256*20);
    _delay_ms(20);
    playNote(G0, 256*20);
    _delay_ms(20);
    playNote(A0, 256*20);
    _delay_ms(20);
    playNote(B0, 256*20);
    _delay_ms(20);
    playNote(C1, 256*80);

    _delay_ms(100);
    _delay_ms(100);
    _delay_ms(100);
    _delay_ms(100);
    _delay_ms(100);
    _delay_ms(100);
    

  } /* End event loop */
  return(0);		      /* This line is never reached  */
}



