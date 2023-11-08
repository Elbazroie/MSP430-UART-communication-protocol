#ifndef _api_H_
#define _api_H_

#include  "../header/halGPIO.h"     // private library - HAL layer


extern void ClearLCD(void);
extern void BlinkRGB(unsigned int X);
extern void countLCD(unsigned int X);
extern void BuzzerTone(unsigned int X);
extern void Xval(unsigned int* X);
extern void LDRsensor(void);
extern void ShowMenu();
extern void BuzzerPB(void);
#endif







