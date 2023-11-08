#include  "../header/api.h"    		// private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer

#define SIZE        32
#define NUM         10

// Global Variables

unsigned int cnt=0,l=0, adc,volt,res,temp, tol=70;
unsigned int toneArray[7] = {1000,1250,1500,1750,2000,2250,2500};
unsigned int toneArray2[4] = {1750,2000,2250,2500};
unsigned int RCV = 0;
char RXdata[5];
unsigned int  RXsize=0;
const char Menu[] = {"----Menu---\n 1. Blink RGB LED, color by color with delay of X[ms]\n \
2. Count up onto LCD screen with delay of X[ms]\n \
3. Circular tone series via Buzzer with delay of X[ms]\n \
4. Get delay time X[ms]\n \
5. LDR 3-digit value [v] onto LCD\n \
6. Clear LCD screen\n \
7. On each PB1 pressed, activate the Buzzer at f(i)\n \
8. Show menu\n \
9. Sleep\n |"};
unsigned int menuSize = sizeof(Menu) / sizeof(Menu[0]);
//--------------------------------------------------------------------
//            Blink RGB on LCD      -  state 1
//--------------------------------------------------------------------
void BlinkRGB(unsigned int X){
        lcd_clear();
        RGBOut = 0x01;
    while (state==state1) {
        TimerDelay(X);
        RGBOut += RGBOut;
        RGBOut %= 8;
        if (RGBOut==0x00) RGBOut = 0x01;
    }
    RGBOut = 0x00;
}

//--------------------------------------------------------------------
//            LCD count up      -  state 2
//--------------------------------------------------------------------
void countLCD(unsigned int X){
    lcd_clear();
    while (state==state2) {
        intconv(cnt);
        cnt += 1;
        if (cnt==0) lcd_putchar(cnt+48);
        TimerDelay(X);
    }
    lcd_clear();
    TIMERconfig();
}

//--------------------------------------------------------------------
//            Circular tone on buzzer    -  state 3
//--------------------------------------------------------------------
void BuzzerTone(unsigned int X){
    lcd_clear();
    BuzzerDir |=  BIT4;
    unsigned int i=0;
    while (state==state3) {
        setFreq(toneArray[i]);
        i++;
        TimerDelay(X);
        i %= 7;
    }
    BuzzerDir &= ~BIT4;
    TIMERconfig();
}

//--------------------------------------------------------------------
//            Receive X value    -  state 4
//--------------------------------------------------------------------
void Xval(unsigned int* X){
    unsigned int i;
    while (state == state4){
        __bis_SR_register(LPM0_bits + GIE);
    }
    // Set new X value and clear Variables
    RCV = 0;
    RXsize = 0;
    *X = atoi(RXdata);
    for(i = 0; i < 5; i++){      // 2^4 samples
        RXdata[i] = 0;
    }
}


//--------------------------------------------------------------------
//            LDR sensor    -      state 5
//--------------------------------------------------------------------
void LDRsensor(void){
    unsigned int i,sum=0;
    lcd_clear();
    lcd_puts("V = .    [v]");
    while (state==state5) {
        sum=0;
        for(i = 0; i < 16; i++){      // 2^4 samples
            ADC_CTL0 |= ENC + ADC10SC;
            __bis_SR_register(LPM0_bits + GIE);
            sum += volt;
        }
        res = sum >> 4;
        if (abs(temp - res) > tol){ // tolerance for LCD update: 50mV
            intconvADC(res);
            temp = res;
        }
    }
    lcd_clear();
}

//--------------------------------------------------------------------
//            LCD clear      -  state 6
//--------------------------------------------------------------------            
void ClearLCD(void){
    lcd_clear();
    cnt = 0;
    state=state9;
}

//--------------------------------------------------------------------
//           Show menu on PC      -  state 8
//--------------------------------------------------------------------
void ShowMenu(){
    while (state==state8);
}

//--------------------------------------------------------------------
//            Circular tone on buzzer with PB    -  state 7
//--------------------------------------------------------------------
void BuzzerPB(void){
    lcd_clear();
    BuzzerDir |=  BIT4;
    while (state==state7) {
        setFreq(toneArray2[l]);
        __bis_SR_register(LPM0_bits + GIE);
    }
    BuzzerDir &= ~BIT4;
}
