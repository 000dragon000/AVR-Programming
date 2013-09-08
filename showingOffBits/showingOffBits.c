            /* Showing off some patterns to practice our bit-twiddling */

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>

#define DELAYTIME 85                                   /* milliseconds */
#define LED_PORT                PORTB
#define LED_DDR                 DDRB

int main(void) {
	clock_prescale_set(clock_div_8);
	
	uint8_t i;
  uint8_t repetitions;
  uint8_t whichLED;
  // -------- Inits --------- //
  LED_DDR = 0xff;                    /* all LEDs configured for output */
  // ------ Event loop ------ //
  while (1) {
                                                            /* Go Left */
    for (i = 0; i < 8; i++) {
      LED_PORT |= (1 << i);                    /* turn on the i'th pin */
      _delay_ms(DELAYTIME);                                    /* wait */
    }
    for (i = 0; i < 8; i++) {
      LED_PORT &= ~(1 << i);                  /* turn off the i'th pin */
      _delay_ms(DELAYTIME);                                    /* wait */
    }
    _delay_ms(5 * DELAYTIME);                                 /* pause */

                                                           /* Go Right */
    for (i = 7; i < 255; i--) {
      LED_PORT |= (1 << i);              /* turn on the i'th pin */
      _delay_ms(DELAYTIME);                                    /* wait */
    }
    for (i = 7; i < 255; i--) {
      LED_PORT &= ~(1 << i);            /* turn off the i'th pin */
      _delay_ms(DELAYTIME);                                    /* wait */
    }
    _delay_ms(5 * DELAYTIME);                                 /* pause */

                           /* Toggle in a "random" pattern for a while */
    for (repetitions = 0; repetitions < 75; repetitions++) {
          /* Do some funny math, and then zero all but the last 3 bits */
      whichLED = (repetitions * 11) & 0b00000111;
      LED_PORT ^= (1 << whichLED);                   /* toggle our LED */
      _delay_ms(DELAYTIME);
    }
    LED_PORT = 0;                                      /* all LEDs off */
    _delay_ms(5 * DELAYTIME);                                 /* pause */

  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
