#include  "../header/api.h"    		// private library - API layer
#include  "../header/app.h"    		// private library - APP layer
#include  <stdio.h>


enum FSMstate state;
enum SYSmode lpm_mode;

void main(void){

  state    = state9;       // start in idle state on RESET
  lpm_mode = mode0;        // start in idle state on RESET
  sysConfig();
  lcd_init();
  unsigned int X = 500;     // default => 500ms
  while(1){
	switch(state){

      case state1:      // Blink RGB
          BlinkRGB(X);
		break;
		 
	  case state2:      // Count Up on LCD
          countLCD(X);
		break;
                
      case state3:      // Tones through Buzzer
          BuzzerTone(X);
		break;

      case state4:      // Change X delay time value
          Xval(&X);
        break;

      case state5:      // Measure LDR voltage
          LDRsensor();
        break;

      case state6:      // Clear LCD and reset counter
          ClearLCD();
        break;

      case state7: // Print Menu to PC
           BuzzerPB();
        break;

      case state8:      // Show menu
          ShowMenu();
        break;

      case state9: // Print Menu to PC
          IE2 |= UCA0RXIE;         // Enable USCI_A0 RX interrupt
          enterLPM(lpm_mode);
        break;


	}
  }

}
  
  
  
  
  
  
