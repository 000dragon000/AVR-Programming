/* 

Cylon eyes actually turns out to be a decent POV routine.  
The only change here is in DELAYTIME.  

 */


#include <avr/io.h>		/* Defines pins, ports, etc */
#define F_CPU 1000000UL	        /* Sets up the chip speed for delay.h */
#include <util/delay.h>		/* Functions to waste time */

#define DELAYTIME 2		/* milliseconds */

void main(void){

  DDRB = 0b11111111;		      /* Data Direction Register B:
					 all set up for output */

  while(1){

    while(i <= 7){
      PORTB = (1 << i);		/* illuminate only i'th pin */
      _delay_ms(DELAYTIME);	/* wait */
      i = i + 1;		/* move to the next LED */
    }

    while(i > 0){
      PORTB = (1 << i);		/* illuminate only i'th pin */
      _delay_ms(DELAYTIME);	/* wait */
      i = i - 1;		/* move to the previous LED */
    }

  } 
}

