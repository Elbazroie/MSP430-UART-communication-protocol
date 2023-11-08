#ifndef _halGPIO_H_
#define _halGPIO_H_
  
#ifdef LabKit
#include  "../header/bsp_msp430x4xx.h"
#else  // #define PersonalEvaluationKit
#include "../header/bsp_msp430x2xx.h"

#endif

#include  "../header/app.h"    		// private library - APP layer

extern enum FSMstate state;   // global variable
extern enum SYSmode lpm_mode; // global variable
extern unsigned int RCV,adc,volt;
extern char RXdata[5];
extern unsigned int RXsize;
extern const char Menu[];
extern unsigned int menuSize;
extern unsigned int l;


extern void sysConfig(void);
extern void delay(unsigned int);
extern void enterLPM(unsigned char);
extern void enable_interrupts();
extern void disable_interrupts();

extern void lcd_cmd(unsigned char);
extern void lcd_data(unsigned char);
extern void lcd_puts(const char * s);
extern void lcd_init();
extern void lcd_strobe();
extern void DelayMs(unsigned int);
extern void DelayUs(unsigned int);

extern void LEDclear(void);
extern void stringOnLCD(unsigned char* string , unsigned int size);
extern void intconv(unsigned int num);
extern void intconvADC(unsigned int num);
extern void TimerDelay(unsigned int X);
extern void setFreq(unsigned int freq);

extern __interrupt void TimerBinterrupt(void);
extern __interrupt void countClicks(void);
extern __interrupt void setPWM(void);
#endif







