#ifdef __MSP430FG4619__
#include  "../header/bsp_msp430x4xx.h"    // private library - BSP layer
#define LabKit

#else  // #define __MSP430G2x55
#include "../header/bsp_msp430x2xx.h"
#define PersonalEvaluationKit

#endif

//-----------------------------------------------------------------------------  
//           GPIO configuration
//-----------------------------------------------------------------------------

void GPIOconfig(void){
  volatile unsigned int i; // in case of while loop usage
  
  WDTCTL = WDTHOLD | WDTPW;		// Stop WDT


  // PushButtons Setup - for P1.0
    PBsArrPortSel    &= ~0x01;         // set as I\O
    PBsArrPortDir    &= ~0x01;         // set as input
    PBsArrIntEdgeSel |=  0x01;         // pull-up mode
    PBsArrIntEn      |=  0x01;         // enable interrupts
    PBsArrIntPend    &= ~0x01;         // clear pending interrupts


  // RGB configuration
  RGBDir |=  0x07;   // output
  RGBSel &= ~0x07;   // I\O
  RGBOut &= ~0x07;

  // LDR configuration
  LDRDir &= ~BIT3;   // input
  LDRSel &= ~BIT3;   // I\O
  
  // Buzzer configuration
  BuzzerSel &= ~BIT4; // I\O
  BuzzerDir |=  BIT4; // output
  
  
  // LCD setup  
  LCD_DATA_WRITE &= ~0xFF;   // Clear data
  LCD_DATA_DIR   |=  0xF0;   // P1.4-P1.7 To Output
  LCD_DATA_SEL   &= ~0xF0;   // I\O P1.4-P1.7 DATA bits
  LCD_CTRL_SEL   &= ~0xE0;   // I\O P2.5-P2.7 CTRL bits
  
  _BIS_SR(GIE);                     // enable interrupts globally
}                     

//-----------------------------------------------------------------------------
//            Timers configuration
//-----------------------------------------------------------------------------

void TIMERconfig(void){  
  //TIMER A1 setup - Generate Frequency
  TA1ctrl    |= TACLR;                                 // CLR Bit
  TA1ctrl     = TASSEL_2 + ID_3 + MC_0 + TAIE;         // SMCLK, Stop, Div by 8, EN interrupt
  TA1cc_ctrl1 = OUTMOD_4 + CCIE;


  //Timer A0 setup - Delay
  TActrl    |= TACLR;                                 // CLR Bit
  TActrl     = TASSEL_2 + ID_3 + MC_0 + TAIE;         // SMCLK, Stop, Div by 8, EN interrupt
}

//-----------------------------------------------------------------------------
//            ADC configuration
//-----------------------------------------------------------------------------

void ADCconfig(void){
    //ADC10 setup
    ADC_CTL0 = ADC10SHT_2 + ADC10ON + SREF_0 + ADC10IE ;  // 16*ADCLK + Turn on, set ref to Vcc and Gnd, and Enable Interrupt
    ADC_CTL1 = INCH_3 + ADC10SSEL_3;                      // Input A3 and SMCLK
    ADC_AE0 |= BIT3;                                      // P1.3  ADC Enable
}

//-----------------------------------------------------------------------------
//            DMA configuration
//-----------------------------------------------------------------------------

void UARTconfig(void){

    if (CALBC1_1MHZ==0xFF)                    // If calibration constant erased
      {
        while(1);                             // do not load, trap CPU!!
      }
    DCOCTL = 0;                               // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
    DCOCTL = CALDCO_1MHZ;

    UARTSEL  |=  RXD + TXD ;                  // P1.1 = RXD, P1.2=TXD
    UARTSEL2 |=  RXD + TXD ;                  // P1.1 = RXD, P1.2=TXD
    UARTOUT  &= ~0xFF;

    UCA0CTL1 |=  UCSSEL_2;                    // CLK = SMCLK
    UCA0BR0   =  104;                         //
    UCA0BR1   =  0x00;                        //
    UCA0MCTL  =  UCBRS0;                      //
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**

    IE2 |= UCA0RXIE;
}

