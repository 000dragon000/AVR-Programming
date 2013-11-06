

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "pinDefines.h"

#include "USART.h"

#define TEMP_OFFSET  0
#define TEMP_SCALE   1

// -------- Global Variables --------- //
volatile uint8_t milliseconds = 0;
volatile uint8_t hundredMilliseconds = 0;

// -------- Functions --------- //
static inline void initTimerTicks(void) {
  TCCR0A |= (1 << WGM01);                                  /* CTC mode */
  TCCR0B |= (1 << CS00);
  TCCR0B |= (1 << CS02);                               /* 8 MHz / 1024 */
  TIMSK0 |= (1 << OCIE0A);          /* output compare interrupt enable */
  OCR0A = 7;                            /* 8 Mhz / 1024 / 8 = 1.024 ms */
  sei();                          /* set (global) enable interrupt bit */
}

ISR(TIMER0_COMPA_vect) {
  milliseconds++;
  if (milliseconds == 99) {
    milliseconds = 0;
    hundredMilliseconds++;
  }

}


static inline void initTemperature(void) {
  ADMUX |= 8;                                           /* set to ADC8 */
                                     /* set to 1.1v internal reference */
  ADMUX |= (1 << REFS0) | (1 << REFS1);
  ADCSRA |= (1 << ADPS1) | (1 << ADPS2);    /* ADC clock prescaler /64 */
                                  /* enable and start dummy conversion */
  ADCSRA |= (1 << ADEN) | (1 << ADSC);
  loop_until_bit_is_clear(ADCSRA, ADSC);            /* wait until done */
}

int main(void) {
  uint8_t transmitTime;
  uint16_t thisTemp;
  uint16_t temperature;
  volatile uint8_t i;

  // -------- Inits --------- //

  initUSART();
  // printString("\r\nInternal ADC / Temperature Sensor Demo\r\n");
  initTimerTicks();
  LED_DDR |= (1 << LED0);
  initTemperature();
  temperature = ADC;
  // ------ Event loop ------ //
  while (1) {

                      /* Report temperature reading every second or so */
    if (hundredMilliseconds == transmitTime) {
      ADCSRA |= (1 << ADSC);                              /* start ADC */
      loop_until_bit_is_clear(ADCSRA, ADSC);        /* wait until done */
      //      adcValue = ADC;                            /* 10-bit ADC */
      // temperatureTimesTen += (ADC - TEMP_OFFSET) / TEMP_SCALE;
      thisTemp = ADC;
      temperature = ((temperature * 15) + thisTemp) >> 4;

      transmitTime += 10;
      transmitByte((uint8_t) (temperature >> 8));         /* high byte */
      transmitByte((uint8_t) temperature);                 /* low byte */
    }
    i++;
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_mode();

  }                                                  /* End event loop */
  return (0);                            /* This line is never reached */
}
