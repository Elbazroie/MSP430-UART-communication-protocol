#ifndef _bsp_H_
#define _bsp_H_

//#include  <msp430g2553.h>          // MSP430x2xx
#include  <msp430FG4619.h>           // MSP430x4xx


#define   debounceVal      10000
#define   debounceVal2     15000

//--------------------- start of LCD abstraction ----------------//

#ifdef CHECKBUSY
	#define	LCD_WAIT lcd_check_busy()
#else
	#define LCD_WAIT DelayMs(5)
#endif

#define LCD_EN(a)	(!a ? (P2OUT&=~0X20) : (P2OUT|=0X20)) // P2.5 is lcd enable pin
#define LCD_EN_DIR(a)	(!a ? (P2DIR&=~0X20) : (P2DIR|=0X20)) // P2.5 pin direction 

#define LCD_RS(a)	(!a ? (P2OUT&=~0X40) : (P2OUT|=0X40)) // P2.6 is lcd RS pin
#define LCD_RS_DIR(a)	(!a ? (P2DIR&=~0X40) : (P2DIR|=0X40)) // P1.6 pin direction  
  
#define LCD_RW(a)	(!a ? (P2OUT&=~0X80) : (P2OUT|=0X80)) // P2.7 is lcd RW pin
#define LCD_RW_DIR(a)	(!a ? (P2DIR&=~0X80) : (P2DIR|=0X80)) // P2.7 pin direction

#define LCD_DATA_OFFSET 0x04 //data pin selection offset for 4 bit mode, variable range is 0-4, default 0 - Px.0-3, no offset
   
#define LCD_DATA_WRITE	P1OUT
#define LCD_DATA_DIR	P1DIR
#define LCD_DATA_READ	P1IN
#define LCD_DATA_SEL	P1SEL
#define LCD_CTRL_SEL    P2SEL

#define FOURBIT_MODE	0x0
#define EIGHTBIT_MODE	0x1
#define LCD_MODE        FOURBIT_MODE
   
#define OUTPUT_PIN      1	
#define INPUT_PIN       0	
#define OUTPUT_DATA     (LCD_MODE ? 0xFF : (0x0F << LCD_DATA_OFFSET))
#define INPUT_DATA      0x00	

#define LCD_STROBE_READ(value)	LCD_EN(1), \
				asm("Nop"), asm("nop"), \
				value=LCD_DATA_READ, \
				LCD_EN(0) 

#define	lcd_cursor(x)		lcd_cmd(((x)&0x7F)|0x80)
#define lcd_clear()		    lcd_cmd(0x01)
#define lcd_putchar(x)		lcd_data(x)
#define lcd_goto(x)		    lcd_cmd(0x80+(x))
#define lcd_cursor_right()	lcd_cmd(0x14)
#define lcd_cursor_left()	lcd_cmd(0x10)
#define lcd_display_shift()	lcd_cmd(0x1C)
#define lcd_home()		    lcd_cmd(0x02)
#define cursor_off          lcd_cmd(0x0C)
#define cursor_on           lcd_cmd(0x0F)
#define lcd_function_set    lcd_cmd(0x3C) // 8bit,two lines,5x10 dots
#define lcd_new_line        lcd_cmd(0xC0)

//--------------------- end of LCD abstraction ----------------//

// PushButtons abstraction
#define PBsArrPort	       P1IN
#define PBsArrIntPend	   P1IFG 
#define PBsArrIntEn	       P1IE
#define PBsArrIntEdgeSel   P1IES
#define PBsArrPortSel      P1SEL 
#define PBsArrPortDir      P1DIR 
#define PB0                0x01    // to P1.0
#define PB1                0x02    // to P1.1
#define PB2                0x04    // to P1.2
#define PB3                0x08    // to P1.3

// LEDs abstraction
#define LEDsArrPort        P9OUT
#define LEDsArrPortDir     P9DIR
#define LEDsArrPortSel     P9SEL

// RGB abstraction
#define RGBDir    P2DIR
#define RGBSel    P2SEL
#define RGBOut    P2OUT

// LDR abstraction
#define LDRDir    P1DIR
#define LDRSel    P1SEL

// LDR abstraction
#define BuzzerDir    P2DIR
#define BuzzerSel    P2SEL
#define BuzzerOut    P2OUT

// ADC10 abstraction          // modify for LAB
#define ADC_CTL0     ADC12CTL0
#define ADC_CTL1     ADC12CTL1
#define ADC_AE0      ADC12AE0
#define ADC_REG      ADC12MEM
#define ADC10SC      ADC12SC
#define ADC10_VECTOR ADC12_VECTOR

// Timer A0 abstraction
#define TActrl       TA0CTL
#define TAcc_ctrl0   TA0CCTL0
#define TAcc_reg0    TA0CCR0

// Timer B0 abstraction       // modify for LAB
#define TA1ctrl      TB0CTL
#define TA1cc_ctrl0  TB0CCTL0
#define TA1cc_ctrl1  TB0CCTL1
#define TA1cc_reg0   TB0CCR0
#define TA1cc_reg1   TB0CCR1
#define TIMER1_A1_VECTOR  TIMER1_B0_VECTOR
#define TA1IV_TACCR1      TB0IV_TBCCR1
#define TA1IV_TACCR2      TB0IV_TBCCR2
#define TA1IV_TAIFG       TB0IV_TBIFG

// UART abstraction       // modify for LAB
#define  UARTSEL  P4SEL
#define  UARTSEL2 P4SEL2
#define  UARTOUT  P4OUT
#define  RXD      BIT1
#define  TXD      BIT0

extern void GPIOconfig(void);
extern void TIMERconfig(void);
extern void ADCconfig(void);
extern void UARTconfig(void);

#endif



