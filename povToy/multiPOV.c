/* 
   Multi-POV.  
   Allows you to switch between many POV routines 
     at the push of a button.
   Also a debouncing demo.
*/

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include "pinDefines.h"		/* defines used for our kit */
#include "povRoutines.h"	/* display routines here */
#include "povData.h"		/* POV image data here */

#define DEBOUNCETIME   5


// ------- Functions -------- //
uint8_t debounce(void){
  if (bit_is_clear(BUTTON_IN, BUTTON)){      /* button pressed */
    _delay_ms(DEBOUNCETIME);
    if (bit_is_clear(BUTTON_IN, BUTTON)){    /* still  pressed */
      return(1);
    }
  }
  return(0);
}


int main(void){

  // -------- Inits --------- //
  uint8_t whichPOV=0;
  uint8_t buttonWasPressed=0;

  LED_DDR = 0xff;                  /* all LED for output */
  BUTTON_PORT |= (1 << BUTTON);	/* pullup on button */

  // ------ Event loop ------ //
  while(1){			

    /* Here is the button-pressing logic */
    if (debounce()){		/* button pressed */
      if (!buttonWasPressed){
	whichPOV++;		/* go to next mode */
	buttonWasPressed = 1;	/* and record that was pressed */
      }
    }
    else {		       
      buttonWasPressed = 0;
    }
    
    /* Switch among the animations 
       depending on the value of whichPOV */
    switch(whichPOV){
    case 0: 
      cylonPOV();
      break;
    case 1:
      POVString("Hello");
      break;
    case 2:
      POVString("World");
      break;
    case 3:
      POVString("Multi");
      break;
    case 4:
      POVString("POV");
      break;
    case 5:
      POVDisplay(invaderData1, sizeof(invaderData1)/sizeof(uint8_t));
      /* The bit with sizeof() passes the length of the byte array
       to the POVDisplay() function.  See the text for details. */
      break;
    case 6:
      POVDisplay(invaderData2, sizeof(invaderData2)/sizeof(uint8_t));
      break;
    case 7:
      POVDisplay(Star1UP, sizeof(Star1UP)/sizeof(uint8_t));
      break;

      // Add your functions here

    default:
      whichPOV = 0;		/* cycle back around */
      break;
    }

  } /* End event loop */
  return(0);
}
