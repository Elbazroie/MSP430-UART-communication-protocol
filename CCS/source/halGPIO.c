#include  "../header/halGPIO.h"     // private library - HAL layer

// Global Variables
unsigned char TMP;
unsigned int i=0;
const int max=3000;
//--------------------------------------------------------------------
//             System Configuration  
//--------------------------------------------------------------------
void sysConfig(void){ 
	GPIOconfig();
	TIMERconfig();
    ADCconfig();
    UARTconfig();
	lcd_init();
}

//---------------------------------------------------------------------
//            Polling based Delay function
//---------------------------------------------------------------------
void delay(unsigned int t){  // t[usec]
	volatile unsigned int i;
	  for(i=t; i>0; i--); {
        }
}

//---------------------------------------------------------------------
//            Enter from LPM0 mode
//---------------------------------------------------------------------
void enterLPM(unsigned char LPM_level){
	if (LPM_level == 0x00) 
	  _BIS_SR(LPM0_bits);     /* Enter Low Power Mode 0 */
        else if(LPM_level == 0x01) 
	  _BIS_SR(LPM1_bits);     /* Enter Low Power Mode 1 */
        else if(LPM_level == 0x02) 
	  _BIS_SR(LPM2_bits);     /* Enter Low Power Mode 2 */
	else if(LPM_level == 0x03) 
	  _BIS_SR(LPM3_bits);     /* Enter Low Power Mode 3 */
        else if(LPM_level == 0x04) 
	  _BIS_SR(LPM4_bits);     /* Enter Low Power Mode 4 */
}
//---------------------------------------------------------------------
//            Enable interrupts
//---------------------------------------------------------------------
void enable_interrupts(){
  _BIS_SR(GIE);
}
//---------------------------------------------------------------------
//            Disable interrupts
//---------------------------------------------------------------------
void disable_interrupts(){
  _BIC_SR(GIE);
}

//*********************************************************************
//            Port1 Interrupt Service Rotine
//*********************************************************************
#pragma vector=PORT1_VECTOR
  __interrupt void PBs_handler(void){

    delay(debounceVal);
    //-----------------------------------------------------------------
    //            selector of transition between states
    //-----------------------------------------------------------------

      if(PBsArrIntPend & PB1){
          if (state=state7){
              PBsArrIntPend &= ~PB1;
              l++;
              l %= 4;
          }
      // state9 configuration

        }

    //-----------------------------------------------------------------
    //            Exit from a given LPM
    //-----------------------------------------------------------------
        switch(lpm_mode){
        case mode0:
         LPM0_EXIT; // must be called from ISR only
         break;

        case mode1:
         LPM1_EXIT; // must be called from ISR only
         break;

        case mode2:
         LPM2_EXIT; // must be called from ISR only
         break;

                case mode3:
         LPM3_EXIT; // must be called from ISR only
         break;

                case mode4:
         LPM4_EXIT; // must be called from ISR only
         break;
    }

}

//*********************************************************************
//                           UART ISR - TX
//*********************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0TX_VECTOR))) USCI0TX_ISR (void)
#else
#error Compiler not supported!
#endif
{
    if (state == state4) {
        UCA0TXBUF = '4';        // get X value
        IE2 &= ~UCA0TXIE;       // Disable TX ISR
    }
    else if (state == state8) {                  // Print Menu on PC
        if (RCV==0) {
            RCV = 2;
            UCA0TXBUF = '8';
        }
        if (RCV==2) {
            if (i != menuSize) UCA0TXBUF = Menu[i++];
            else {
                i=0;
                RCV = 0;
                state=state9;
                IE2 &= ~UCA0TXIE;       // Disable TX ISR
            }
        }
    }
    else {
        UCA0TXBUF = '9';        // Sleep Mode
        IE2 &= ~UCA0TXIE;       // Disable TX ISR
    }
}


//*********************************************************************
//                           UART ISR - RX
//*********************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCI0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{                               // Read RX buffer
        if(UCA0RXBUF == '0'){
            if (RCV==1) RXdata[RXsize++] = UCA0RXBUF;
        }

        else if(UCA0RXBUF == '1'){
            if (RCV==1) RXdata[RXsize++] = UCA0RXBUF;
            else {
            state = state1;
            IE2 |= UCA0TXIE;    // Enable TX ISR
            }
        }
        else if(UCA0RXBUF == '2'){
            if (RCV==1) RXdata[RXsize++] = UCA0RXBUF;
            else {
            state = state2;
            IE2 |= UCA0TXIE;
            }
        }
        else if(UCA0RXBUF == '3'){
            if (RCV==1) RXdata[RXsize++] = UCA0RXBUF;
            else {
            state = state3;
            IE2 |= UCA0TXIE;
            }
        }

        else if(UCA0RXBUF == '4' && RCV==0){
            RCV = 1;
            state = state4;
            IE2 |= UCA0TXIE;
        }
        else if(UCA0RXBUF == '4' && RCV==1){
            RXdata[RXsize++] = UCA0RXBUF;
        }
            // End of X transmitted data
        else if (UCA0RXBUF == '\n'  && RCV==1) {
            RCV = 0;
            state = state9;
            IE2 |= UCA0TXIE;
        }

        else if(UCA0RXBUF == '5'){
            if (RCV==1) RXdata[RXsize++] = UCA0RXBUF;
            else {
            state = state5;
            IE2 |= UCA0TXIE;
            }
        }
        else if(UCA0RXBUF == '6'){
            if (RCV==1) RXdata[RXsize++] = UCA0RXBUF;
            else {
            state = state6;
            IE2 |= UCA0TXIE;
            }
        }
        else if(UCA0RXBUF == '8'){
            if (RCV==1) RXdata[RXsize++] = UCA0RXBUF;
            else {
            state = state8;
            IE2 |= UCA0TXIE;
            }
        }
        else if(UCA0RXBUF == '9'){
            if (RCV==1) RXdata[RXsize++] = UCA0RXBUF;
            else {
                state = state9;
                IE2 |= UCA0TXIE;
            }
        }
        else if(UCA0RXBUF == '7'){
            if (RCV==1) RXdata[RXsize++] = UCA0RXBUF;
            else {
                state = state7;
                IE2 |= UCA0TXIE;
            }
        }

    switch(lpm_mode){
    case mode0:
        LPM0_EXIT; // must be called from ISR only
        break;
    case mode1:
        LPM1_EXIT; // must be called from ISR only
        break;
    case mode2:
        LPM2_EXIT; // must be called from ISR only
        break;
    case mode3:
        LPM3_EXIT; // must be called from ISR only
        break;
    case mode4:
        LPM4_EXIT; // must be called from ISR only
        break;
    }
}

//*********************************************************************
//            Timer 0 A1 interrupt
//*********************************************************************
#pragma vector=TIMER0_A1_VECTOR
  __interrupt void countClicks(void){
    
    switch( TA0IV )  {
    
    case  TA0IV_TACCR2 :  break;            // CCR2 not used  
    case  TA0IV_TACCR1 :  break;            // CCR1 not used
    case  TA0IV_TAIFG  :                    // overflow
      TActrl &= ~TAIFG;
      __bic_SR_register_on_exit(LPM0_bits);       // Exit LPMx, interrupts enabled
    }
 }

  //*********************************************************************
  //            Timer 1 A1 interrupt
  //*********************************************************************
  #pragma vector=TIMER1_A1_VECTOR
    __interrupt void setPWM(void){

      switch( TA1IV )  {

      case  TA1IV_TACCR2 :  break;            // CCR2 not used
      case  TA1IV_TACCR1 :                    // CCR1
          TA1cc_ctrl1 &= ~TAIFG;
          BuzzerOut   ^= BIT4;
      case  TA1IV_TAIFG  :                    // overflow
          TA1ctrl   &= ~TAIFG;
          BuzzerOut ^= BIT4;
      }
   }

//*********************************************************************
//            ADC10 Vector Interrupt Service Routine
//*********************************************************************
#pragma vector = ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
    ADC_CTL0 &= ~(ENC + ADC10SC);
    adc = ADC_REG;
    volt = max - ((adc << 1) + adc ) ; // ~= 3000/1024 * adc
    __bic_SR_register_on_exit(LPM0_bits);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~$$$$$$$$$$$$$$$~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~$$$$$$$$$$$$$$$~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//--------------------------------------------------------------------
//            Print string on LCD
//--------------------------------------------------------------------

void TimerDelay(unsigned int X){
  unsigned int quarter, remainder,temp=X;
  quarter = 32768;  // 250ms / (1/2^17) = 32k
  remainder = (temp%250) << 7; // ~= 131
  TActrl |= MC_1 ;
  while (temp != 0) {
      if (temp >= 250) {
          TAcc_reg0 = quarter;
          temp-=250;
      }
      else {
          TAcc_reg0 = remainder;
          temp=0;
      }
      __bis_SR_register(LPM0_bits + GIE);       // Exit LPMx, interrupts enabled
  }
  //TIMERconfig();
}

//--------------------------------------------------------------------
//            set Buzzer frequency using TimerA1
//--------------------------------------------------------------------

void setFreq(unsigned int freq){
    unsigned int reg = 0xFFFF/freq;
    TA1cc_reg0 = reg ;
    TA1cc_reg1 = reg >> 1 ;
    TA1ctrl |= MC_1 ; // start Timer A1: UP mode

}
//--------------------------------------------------------------------
//            Convert int to str and Print on LCD
//--------------------------------------------------------------------

void intconv(unsigned int num){
    unsigned char str[10];
    int j= 0,i=0,temp=num;
    while (temp!=0){
        str[i] = temp % 10 + 48;
        temp /= 10;
        i++;
    }
    lcd_home();
    for (j = i-1; j >= 0; j--) {
        lcd_putchar(str[j]);
    }
}

void intconvADC(unsigned int num){
    unsigned char str[4];
    int j= 0,i=0,temp=num;
    while (temp!=0){
        str[i] = temp % 10 + 48;
        temp /= 10;
        i++;
    }
    if (num<1000 & num>=100) {
        str[3] = 48;
    }
    else if (num<100 & num>=10) {
        str[3] = 48;
        str[2] = 48;
    }
    else if (num<10) {
        str[3] = 48;
        str[2] = 48;
        str[1] = 48;
    }
    lcd_home();
    lcd_cursor_right();
    lcd_cursor_right();
    lcd_cursor_right();
    for (j = 3; j >= 0; j--) {
        if (j==2){
            lcd_cursor_right();
            lcd_putchar(str[j]);
        }
        else lcd_putchar(str[j]);
    }
}
//--------------------------------------------------------------------
//            Print string on LCD
//--------------------------------------------------------------------

void stringOnLCD(unsigned char* string , unsigned int size){
    unsigned int i,half;
    half = size/2;
    for (i = 0; i < half; i++) {
        lcd_putchar(string[i]);
    }
    lcd_new_line;
    for (i = half; i < size; i++) {
        lcd_putchar(string[i]);
    }
    lcd_home();
}

//--------------------------------------------------------------------
//             send a command to the LCD
//--------------------------------------------------------------------
void lcd_cmd(unsigned char c){
  
	LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h

	if (LCD_MODE == FOURBIT_MODE)
	{
		LCD_DATA_WRITE &= ~OUTPUT_DATA;// clear bits before new write
                LCD_DATA_WRITE |= ((c >> 4) & 0x0F) << LCD_DATA_OFFSET;
		lcd_strobe();
                LCD_DATA_WRITE &= ~OUTPUT_DATA;
    		LCD_DATA_WRITE |= (c & (0x0F)) << LCD_DATA_OFFSET;
		lcd_strobe();
	}
	else
	{
		LCD_DATA_WRITE = c;
		lcd_strobe();
	}
}

//--------------------------------------------------------------------
//             send data to the LCD
//--------------------------------------------------------------------
void lcd_data(unsigned char c){
        
	LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h

	LCD_DATA_WRITE &= ~OUTPUT_DATA;       
	LCD_RS(1);
	if (LCD_MODE == FOURBIT_MODE)
	{
    		LCD_DATA_WRITE &= ~OUTPUT_DATA;
                LCD_DATA_WRITE |= ((c >> 4) & 0x0F) << LCD_DATA_OFFSET;  
		lcd_strobe();		
                LCD_DATA_WRITE &= (0xF0 << LCD_DATA_OFFSET) | (0xF0 >> 8 - LCD_DATA_OFFSET);
                LCD_DATA_WRITE &= ~OUTPUT_DATA;
		LCD_DATA_WRITE |= (c & 0x0F) << LCD_DATA_OFFSET; 
		lcd_strobe();
	}
	else
	{
		LCD_DATA_WRITE = c;
		lcd_strobe();
	}       
	LCD_RS(0);   
}

//--------------------------------------------------------------------
//            write a string of chars to the LCD
//--------------------------------------------------------------------
void lcd_puts(const char * s){
  
	while(*s)
		lcd_data(*s++);
}

//--------------------------------------------------------------------
//            initialize the LCD
//--------------------------------------------------------------------
void lcd_init(){
  
	char init_value;

	if (LCD_MODE == FOURBIT_MODE) init_value = 0x3 << LCD_DATA_OFFSET;
        else init_value = 0x3F;
	
	LCD_RS_DIR(OUTPUT_PIN);
	LCD_EN_DIR(OUTPUT_PIN);
	LCD_RW_DIR(OUTPUT_PIN);
        LCD_DATA_DIR |= OUTPUT_DATA;
        LCD_RS(0);
	LCD_EN(0);
	LCD_RW(0);
        
	DelayMs(15);
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
	LCD_DATA_WRITE |= init_value;
	lcd_strobe();
	DelayMs(5);
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
	LCD_DATA_WRITE |= init_value;
	lcd_strobe();
	DelayUs(200);
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
	LCD_DATA_WRITE |= init_value;
	lcd_strobe();
	
	if (LCD_MODE == FOURBIT_MODE){
		LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h
                LCD_DATA_WRITE &= ~OUTPUT_DATA;
		LCD_DATA_WRITE |= 0x2 << LCD_DATA_OFFSET; // Set 4-bit mode
		lcd_strobe();
		lcd_cmd(0x28); // Function Set
	}
        else lcd_cmd(0x3C); // 8bit,two lines,5x10 dots 
	
	lcd_cmd(0xF); //Display On, Cursor On, Cursor Blink
	lcd_cmd(0x1); //Display Clear
	lcd_cmd(0x6); //Entry Mode
	lcd_cmd(0x80); //Initialize DDRAM address to zero
}

//--------------------------------------------------------------------
//              LCD Delay usec functions
//--------------------------------------------------------------------
void DelayUs(unsigned int cnt){
  
	unsigned char i;
        for(i=cnt ; i>0 ; i--) asm("nOp"); // tha command asm("nop") takes raphly 1usec
	
}

//--------------------------------------------------------------------
//              LCD Delay msec functions
//--------------------------------------------------------------------
void DelayMs(unsigned int cnt){
  
	unsigned char i;
        for(i=cnt ; i>0 ; i--) DelayUs(1000); // tha command asm("nop") takes raphly 1usec
	
}

//--------------------------------------------------------------------
//              LCD strobe functions
//--------------------------------------------------------------------
void lcd_strobe(){
  LCD_EN(1);
  asm("noP");
  asm("NOP");
  LCD_EN(0);
}
