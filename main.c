#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <string.h>
#include <math.h>

#define  CS_DDR      DDRB
#define  CS_PORT     PORTB
#define  CS          2
#define  disable()   CS_PORT|=(1<<CS);
#define  enable()    CS_PORT&=~(1<<CS);



#define  BRIGHTNESS   1
#define  ANIM_DELAY   20


#define  RTC_reg_init()     ASSR=0x20;TCCR2B=0x05;TIMSK2=0x01;        \
                            TIFR2=0x00;sei();                         \
						    while(ASSR & ((1<<TCN2UB)|(TCR2BUB)));
						  
#define  power_save_sleep() ADCSRA =0x00;ACSR|=(1<<ACD);              \
                            cli();set_sleep_mode(SLEEP_MODE_PWR_SAVE);\
					        sleep_enable();sleep_bod_disable();       \
					        sei();sleep_cpu();sleep_disable();

volatile uint32_t RTC_TR=0,RTC_DR=0;
uint8_t months[13]={29,31,28,31,30,31,30,31,31,30,31,30,31};

void RTC_Start(uint32_t time, uint32_t date){
uint32_t sec=time%100;
uint32_t min=(time/100)%100;
uint32_t hr=time/10000;
RTC_TR=(hr*3600)+(min*60)+sec;
uint32_t day=(date/10000);
uint32_t mnth=(date/100)%100;
uint32_t year=(date%100);
uint32_t total_days=0;
for(uint8_t i=1;i<mnth;i++){total_days+=months[i];}
if(((year/4)==0)&&(mnth>2)){total_days+=1;}
RTC_DR=total_days+day;
RTC_reg_init();
}

ISR(TIMER2_OVF_vect){
RTC_TR++;
if(RTC_TR>86399){RTC_TR=0;RTC_DR++;}
}

void RTC_sleep(uint32_t sec){
uint32_t  now=RTC_TR+sec;
if(now>86399){while(RTC_TR!=0){power_save_sleep();}now=now-86400;}
while(RTC_TR<now){power_save_sleep();}
}



uint8_t raw[32];
uint8_t mnth[13][3] PROGMEM={
                 {' ',' ',' '},
                 {'J','A','N'},
				 {'F','E','B'},
				 {'M','A','R'},
				 {'A','P','R'},
				 {'M','A','Y'},
				 {'J','U','N'},
				 {'J','U','L'},
				 {'A','U','G'},
				 {'S','E','P'},
				 {'O','C','T'},
				 {'N','O','V'},
				 {'D','E','C'}
				 };
uint8_t chars[102][8] PROGMEM={
                 
				 {
				 0b00000,  //      //
				 0b00000,
				 0b00000,
				 0b00000,
				 0b00000,
				 0b00000,
				 0b00000,
				 0b00000
				 },
				 
				 {
				 0b00100,  // ! //
				 0b00100,
				 0b00100,
				 0b00100,
				 0b00100,
				 0b00000,
				 0b00100,
				 0b00000
				 },
				 
				 {
				 0b10010,  // " //
				 0b10010,
				 0b10010,
				 0b00000,
				 0b00000,
				 0b00000,
				 0b00000,
				 0b00000
				 },
				 
				 {
				 0b00000,  // # //
				 0b00101,
				 0b11111,
				 0b01010,
				 0b11111,
				 0b10100,
				 0b0000,
				 0b00000
				 },
				 
				 {
				 0b01110,  // $ //
				 0b10101,
				 0b10100,
				 0b01110,
				 0b00101,
				 0b10101,
				 0b01110,
				 0b00000
				 },
				 
				 {
				 0b11000,  // % //
				 0b11001,
				 0b00010,
				 0b00100,
				 0b01000,
				 0b10011,
				 0b00011,
				 0b00000
				 },
				 
				 {
				 0b01110,   // & //
				 0b10010,
				 0b10100,
				 0b01000,
				 0b10101,
				 0b10010,
				 0b01101,
				 0b00000
				 },
				 
				 {
				 0b00100,  // ' //
				 0b00100,
				 0b00100,
				 0b00000,
				 0b00000,
				 0b00000,
				 0b00000,
				 0b00000
				 },
				 
				 {
				 0b00010,  // ( //
				 0b00100,
				 0b01000,
				 0b01000,
				 0b01000,
				 0b00100,
				 0b00010,
				 0b00000
				 },
				 
				 {
				 0b01000,  // ) //
				 0b00100,
				 0b00010,
				 0b00010,
				 0b00010,
				 0b00100,
				 0b01000,
				 0b00000
				 },
				 
				 {
				 0b00100,  // * //
				 0b00101,
				 0b10110,
				 0b01100,
				 0b01010,
				 0b10001,
				 0b00000,
				 0b00000
				 },
				 
				 {
				 0b00000,  // + //
				 0b00100,
				 0b00100,
				 0b11111,
				 0b00100,
				 0b00100,
				 0b00000,
				 0b00000
				 },
				 
				 {
				 0b00000,  // , //
				 0b00000,
				 0b00000,
				 0b01100,
				 0b01100,
				 0b00100,
				 0b01000,
				 0b00000
				 },
				 
				 {
				 0b00000,  // - //
				 0b00000,
				 0b00000,
				 0b11111,
				 0b00000,
				 0b00000,
				 0b00000,
				 0b00000
				 },
				 
				 {
				 0b00000, // . //
				 0b00000,
				 0b00000,
				 0b00000,
				 0b00000,
				 0b01100,
				 0b01100,
				 0b00000
				 },
				 
				 {
				 0b00000,  // / //
				 0b00001,
				 0b00010,
				 0b00100,
				 0b01000,
				 0b10000,
				 0b00000,
				 0b00000
				 },
				 
				 {
				 0b01110, // 0 (+16) //
				 0b10001,
				 0b10011,
				 0b10101,
				 0b11001,
				 0b10001,
				 0b01110,
				 0b00000
				 },
				 
				 {
				 0b00100, // 1 //
				 0b01100,
				 0b00100,
				 0b00100,
				 0b00100,
				 0b00100,
				 0b01110,
				 0b00000
				 },
				 
				 {
				 0b01110, // 2 //
				 0b10001,
				 0b00001,
				 0b00010,
				 0b00100,
				 0b01000,
				 0b11111,
				 0b00000
				 },
				 
				 {
				 0b01110, // 3 //
				 0b10001,
				 0b00001,
				 0b00010,
				 0b00001,
				 0b10001,
				 0b01110,
				 0b00000
				 },
				 
				 {
				 0b00011, // 4 //
				 0b00101,
				 0b01001,
				 0b10001,
				 0b11111,
				 0b00001,
				 0b00001,
				 0b00000
				 },
				 
				 {
				 0b11111, // 5 //
				 0b10000,
				 0b11110,
				 0b00001,
				 0b00001,
				 0b10001,
				 0b01110,
				 0b00000
				 },
				 
				 {
				 0b01110,  // 6 //
				 0b10001,
				 0b10000,
				 0b11110,
				 0b10001,
				 0b10001,
				 0b01110,
				 0b00000
				 },
				 
				 {
				 0b11111,  // 7 //
				 0b10001,
				 0b00001,
				 0b00010,
				 0b00100,
				 0b01000,
				 0b01000,
				 0b00000
				 },
				 
				 { 
				 0b01110,  // 8 //
				 0b10001,
				 0b10001,
				 0b01110,
				 0b10001,
				 0b10001,
				 0b01110,
				 0b00000
				 },
				 
				 {
				 0b01110,  // 9 //
				 0b10001,
				 0b10001,
				 0b01111,
				 0b00001,
				 0b10001,
				 0b01110,
				 0b00000
				 },
				 
				 {
				 0b00000,  // : //
				 0b01100,
				 0b01100,
				 0b00000,
				 0b01100,
				 0b01100,
				 0b00000,
				 0b00000
				 },
				 
				 {
				 0b01100,  // ; //
				 0b01100,
				 0b00000,
				 0b01100,
				 0b01100,
				 0b00100,
				 0b01000,
				 0b00000
				 },
				 
				 {
				 0b00001,  // < //
				 0b00010,
				 0b00100,
				 0b01000,
				 0b00100,
				 0b00010,
				 0b00001,
				 0b00000
				 },
				 
				 {
				 0b00000,  // = //
				 0b00000,
				 0b11111,
				 0b00000,
				 0b11111,
				 0b00000,
				 0b00000,
				 0b00000
				 },
				 
				 {
				 0b10000,
				 0b01000, // > //
				 0b00100,
				 0b00010,
				 0b00100,
				 0b01000,
				 0b10000,
				 0b00000
				 },
				 
				 {
				 0b01110,  // ? //
				 0b10001,
				 0b10001,
				 0b00010,
				 0b00100,
				 0b00000,
				 0b00100,
				 0b00000
				 },
				 
				 {
				 0b01110,   // @ //
				 0b10001,
				 0b10111,
				 0b10110,
				 0b10000,
				 0b10000,
				 0b01111,
				 0b00000
				 },
				 
                 {
				 0b01110,
				 0b10001,
				 0b10001,
				 0b10001,
				 0b11111,
				 0b10001,
				 0b10001,
				 0b00000
				 },
				 
				 {
				 0b11110,
				 0b10001,
				 0b10001,
				 0b11110,
				 0b10001,
				 0b10001,
				 0b11110,
				 0b00000
				 },
				 
				 {
				 0b01110,
				 0b10001,
				 0b10000,
				 0b10000,
				 0b10000,
				 0b10001,
				 0b01110,
				 0b00000
				 },
				 
				 {
				 0b11110,
				 0b10001,
				 0b10001,
				 0b10001,
				 0b10001,
				 0b10001,
				 0b11110,
				 0b00000
				 },
				 
				 {
				 0b11111,
				 0b10000,
				 0b10000,
				 0b11110,
				 0b10000,
				 0b10000,
				 0b11111,
				 0b00000
				 },
				 
				 {
				 0b11111,
				 0b10000,
				 0b10000,
				 0b11110,
				 0b10000,
				 0b10000,
				 0b10000,
				 0b00000
				 },
				 
				 {
				 0b01110,
				 0b10001,
				 0b10000,
				 0b10000,
				 0b10111,
				 0b10001,
				 0b01110,
				 0b00000
				 },
				 
				 {
				 0b10001,
				 0b10001,
				 0b10001,
				 0b11111,
				 0b10001,
				 0b10001,
				 0b10001,
				 0b00000
				 },
				 
				 {
				 0b01110,
				 0b00100,
				 0b00100,
				 0b00100,
				 0b00100,
				 0b00100,
				 0b01110,
				 0b00000
				 },
				 
				 {
				 0b11111,
				 0b00001,
				 0b00001,
				 0b00001,
				 0b00001,
				 0b10001,
				 0b01110,
				 0b00000
				 },
				 
				 {
				 0b10001,
				 0b10010,
				 0b10100,
				 0b11000,
				 0b10100,
				 0b10010,
				 0b10001,
				 0b00000
				 },
				 
				 {
				 0b10000,
				 0b10000,
				 0b10000,
				 0b10000,
				 0b10000,
				 0b10001,
				 0b11111,
				 0b00000
				 },
				 
				 {
				 0b10001,
				 0b11011,
				 0b10101,
				 0b10001,
				 0b10001,
				 0b10001,
				 0b10001,
				 0b00000
				 },
				 
				 {
				 0b10001,
				 0b10001,
				 0b11001,
				 0b10101,
				 0b10011,
				 0b10001,
				 0b10001,
				 0b00000
				 },
				 
				 {
				 0b01110,
				 0b10001,
				 0b10001,
				 0b10001,
				 0b10001,
				 0b10001,
				 0b01110,
				 0b00000
				 },
				 
				 {
				 0b11110,
				 0b10001,
				 0b10001,
				 0b11110,
				 0b10000,
				 0b10000,
				 0b10000,
				 0b00000
				 },
				 
				 {
				 0b01100,
				 0b10010,
				 0b10010,
				 0b11010,
				 0b10110,
				 0b10110,
				 0b01101,
				 0b00000
				 },
				 
				 {
				 0b11110,
				 0b10001,
				 0b10001,
				 0b11110,
				 0b10100,
				 0b10010,
				 0b10001,
				 0b00000
				 },
				 
				 {
				 0b01110,
				 0b10001,
				 0b10000,
				 0b01110,
				 0b00001,
				 0b10001,
				 0b01110,
				 0b00000
				 },
				 
				 {
				 0b11111,
				 0b10101,
				 0b00100,
				 0b00100,
				 0b00100,
				 0b00100,
				 0b00100,
				 0b00000
				 },
				 
				 {
				 0b10001,
				 0b10001,
				 0b10001,
				 0b10001,
				 0b10001,
				 0b10001,
				 0b01110,
				 0b00000
				 },
				 
				 {
				 0b10001,
				 0b10001,
				 0b10001,
				 0b10001,
				 0b10001,
				 0b01010,
				 0b00100,
				 0b00000
				 },
				 
				 {
				 0b10001,
				 0b10001,
				 0b10001,
				 0b10101,
				 0b10101,
				 0b10101,
				 0b01010,
				 0b00000
				 },
				 
				 {
				 0b10001,
				 0b10001,
				 0b01010,
				 0b00100,
				 0b01010,
				 0b10001,
				 0b10001,
				 0b00000
				 },
				 
				 {
				 0b10001,
				 0b10001,
				 0b01010,
				 0b00100,
				 0b00100,
				 0b00100,
				 0b00100,
				 0b00000
				 },
				 
				 {
				 0b11111,
				 0b00001,
				 0b00010,
				 0b00100,
				 0b01000,
				 0b10000,
				 0b11111,
				 0b00000
				 },
				 
				 {             
				 0b01100,
				 0b01000,
				 0b01000,
				 0b01000,
				 0b01000,
				 0b01000,
				 0b01100,
				 0b00000
				 },
				 
				 {             
				 0b00000,
				 0b10000,
				 0b01000,
				 0b00100,
				 0b00010,
				 0b00001,
				 0b00000,
				 0b00000
				 },
				 
				 {             
				 0b01100,
				 0b00100,
				 0b00100,
				 0b00100,
				 0b00100,
				 0b00100,
				 0b01100,
				 0b00000
				 },
				 
				 {             
				 0b00100,
				 0b01010,
				 0b10001,
				 0b00000,
				 0b00000,
				 0b00000,
				 0b00000,
				 0b00000
				 },
				 
				 {             
				 0b00000,
				 0b00000,
				 0b00000,
				 0b00000,
				 0b00000,
				 0b00000,
				 0b11111,
				 0b00000
				 },
				 
				 {             
				 0b01000,
				 0b00100,
				 0b00010,
				 0b00000,
				 0b00000,
				 0b00000,
				 0b00000,
				 0b00000
				 },
				 
				 {             // a  //
				 0b00000,
				 0b00000,
				 0b01101,
				 0b10011,
				 0b10001,
				 0b10011,
				 0b01101,
				 0b00000
				 },
				 
				 {             
				 0b10000,
				 0b10000,
				 0b10110,
				 0b11001,
				 0b10001,
				 0b11001,
				 0b10110,
				 0b00000
				 },
				 
				 {             
				 0b00000,
				 0b00000,
				 0b01110,
				 0b10001,
				 0b10000,
				 0b10001,
				 0b01110,
				 0b00000
				 },
				 
				 {            
				 0b00001,
				 0b00001,
				 0b01101,
				 0b10011,
				 0b10001,
				 0b10011,
				 0b01101,
				 0b00000
				 },
				 
				 {            
				 0b00000,
				 0b00000,
				 0b01110,
				 0b10001,
				 0b11111,
				 0b10000,
				 0b01111,
				 0b00000
				 },
				 
				 {          
				 0b00110,
				 0b01001,
				 0b01000,
				 0b11100,
				 0b01000,
				 0b01000,
				 0b01000,
				 0b00000
				 },
				 
				 {           
				 0b00000,
				 0b00000,
				 0b01111,
				 0b10001,
				 0b01111,
				 0b00001,
				 0b11110,
				 0b00000
				 },
				 
				 {          
				 0b10000,
				 0b10000,
				 0b10000,
				 0b11100,
				 0b10010,
				 0b10010,
				 0b10010,
				 0b00000
				 },
				 
				 {           
				 0b00100,
				 0b00000,
				 0b00100,
				 0b00100,
				 0b00100,
				 0b00100,
				 0b00100,
				 0b00000
				 },
				 
				 {           
				 0b00010,
				 0b00000,
				 0b00010,
				 0b00010,
				 0b00010,
				 0b10010,
				 0b01100,
				 0b00000
				 },
				 
				 {           
				 0b10000,
				 0b10000,
				 0b10010,
				 0b10100,
				 0b11000,
				 0b10100,
				 0b10010,
				 0b00000
				 },
				 
				 {             
				 0b00100,
				 0b00100,
				 0b00100,
				 0b00100,
				 0b00100,
				 0b00100,
				 0b00100,
				 0b00000
				 },
				 
				 {           
				 0b00000,
				 0b00000,
				 0b11110,
				 0b10101,
				 0b10101,
				 0b10101,
				 0b10101,
				 0b00000
				 },
				 
				 {           
				 0b00000,
				 0b00000,
				 0b11100,
				 0b10010,
				 0b10010,
				 0b10010,
				 0b10010,
				 0b00000
				 },
				 
				 {           
				 0b00000,
				 0b00000,
				 0b01110,
				 0b10001,
				 0b10001,
				 0b10001,
				 0b01110,
				 0b00000
				 },
				 
				 {          
				 0b00000,
				 0b00000,
				 0b11110,
				 0b10001,
				 0b11110,
				 0b10000,
				 0b10000,
				 0b00000
				 },
				 
				 {           
				 0b00000,
				 0b00000,
				 0b01111,
				 0b10001,
				 0b01111,
				 0b00001,
				 0b00001,
				 0b00000
				 },
				 
				 {           
				 0b00000,
				 0b00000,
				 0b10110,
				 0b11001,
				 0b10000,
				 0b10000,
				 0b10000,
				 0b00000
				 },
				 
				 {          
				 0b00000,
				 0b00000,
				 0b01111,
				 0b10000,
				 0b01110,
				 0b00001,
				 0b11110,
				 0b00000
				 },
				 
				 {             
				 0b01000,
				 0b01000,
				 0b11100,
				 0b01000,
				 0b01000,
				 0b01001,
				 0b00110,
				 0b00000
				 },
				 
				 {          
				 0b00000,
				 0b00000,
				 0b10001,
				 0b10001,
				 0b10001,
				 0b10001,
				 0b01111,
				 0b00000
				 },
				 
				 {           
				 0b00000,
				 0b00000,
				 0b10001,
				 0b10001,
				 0b10001,
				 0b01010,
				 0b00100,
				 0b00000
				 },
				 
				 {             
				 0b00000,
				 0b00000,
				 0b10001,
				 0b10101,
				 0b10101,
				 0b10101,
				 0b01010,
				 0b00000
				 },
				 
				 {           
				 0b00000,
				 0b00000,
				 0b10001,
				 0b01010,
				 0b00100,
				 0b01010,
				 0b10001,
				 0b00000
				 },
				 
				 {             
				 0b00000,
				 0b00000,
				 0b10001,
				 0b01001,
				 0b00110,
				 0b00100,
				 0b11000,
				 0b00000
				 },
				 
				 {           
				 0b00000,
				 0b00000,
				 0b11111,
				 0b00010,
				 0b00100,
				 0b01000,
				 0b11111,
				 0b00000
				 },
				 
				 {
				 0b01100,
				 0b10010,
				 0b10010,
				 0b01100,
				 0b00000,
				 0b00000,
				 0b00000,
				 0b00000
				 }
				 ,
				 
				 {
				 0b011110, // 0 (+92) //
				 0b110011,
				 0b110011,
				 0b110011,
				 0b110011,
				 0b110011,
				 0b110011,
				 0b011110
				 },
				 
				 {
				 0b001100, 
				 0b011100,
				 0b001100,
				 0b001100,
				 0b001100,
				 0b001100,
				 0b001100,
				 0b011110
				 },
				 
				 {
				 0b011110, 
				 0b110011,
				 0b000011,
				 0b000110,
				 0b001100,
				 0b011000,
				 0b110001,
				 0b111111
				 },
				 
				 {
				 0b011110, 
				 0b110011,
				 0b000011,
				 0b000110,
				 0b000011,
				 0b000011,
				 0b110011,
				 0b011110
				 },
				 
				 {
				 0b000011, 
				 0b000111,
				 0b001111,
				 0b011011,
				 0b110011,
				 0b111111,
				 0b000011,
				 0b000011
				 },
				 
				 {
				 0b111111, 
				 0b110000,
				 0b110000,
				 0b111110,
				 0b000011,
				 0b000011,
				 0b110011,
				 0b011110
				 },
				 
				 {
				 0b011110, 
				 0b110011,
				 0b110000,
				 0b111110,
				 0b110011,
				 0b110011,
				 0b110011,
				 0b011110
				 },
				 
				 {
				 0b111111, 
				 0b110011,
				 0b000011,
				 0b000110,
				 0b001100,
				 0b011000,
				 0b011000,
				 0b011000
				 },
				 
				 {
				 0b011110, 
				 0b110011,
				 0b110011,
				 0b011110,
				 0b110011,
				 0b110011,
				 0b110011,
				 0b011110
				 },
				 
				 {
				 0b011110, 
				 0b110011,
				 0b110011,
				 0b110011,
				 0b011111,
				 0b000011,
				 0b110011,
				 0b011110
				 },
      };


void spi_init(void){
DDRB |=(1<<2)|(1<<3)|(1<<5);
PORTB|=(1<<3);
disable();
SPCR=(1 << SPE) | (1 << MSTR) | (1 << SPR1);
}

void spi_tx(uint8_t data){
SPDR = data;
while((SPSR & (1 << SPIF))==0);
}

void spi_write(uint8_t addr, uint8_t data){
spi_tx(addr);
spi_tx(data);
}

void whole(uint8_t addr, uint8_t data){
enable();
spi_write(addr,data);
spi_write(addr,data);
spi_write(addr,data);
spi_write(addr,data);
disable();
}

void send(uint8_t *data){
for(uint8_t i=0;i<8;i++){
  enable();
  for(uint8_t k=0;k<4;k++){spi_write(i+1,data[k*8+i]);}
  disable();
  }
}

void clear_display(void){
whole(0x01,0x00);
whole(0x02,0x00);
whole(0x03,0x00);
whole(0x04,0x00);
whole(0x05,0x00);
whole(0x06,0x00);
whole(0x07,0x00);
whole(0x08,0x00);


whole(0x01,0x00);
whole(0x02,0x00);
whole(0x03,0x00);
whole(0x04,0x00);
whole(0x05,0x00);
whole(0x06,0x00);
whole(0x07,0x00);
whole(0x08,0x00);

whole(0x01,0x00);
whole(0x02,0x00);
whole(0x03,0x00);
whole(0x04,0x00);
whole(0x05,0x00);
whole(0x06,0x00);
whole(0x07,0x00);
whole(0x08,0x00);
}

void display_init(void){
_delay_ms(100);
whole(0x0F,0x01);
_delay_ms(100);
whole(0x0F,0x00);
whole(0x0A,BRIGHTNESS);
whole(0x0B,0x07);
clear_display();
whole(0x0C,0x01);
_delay_ms(2000);
}

void empty_coloumn(void){
for(uint8_t i=0;i<32;i++){raw[i]=0;}
}

void fill_buffer(uint8_t character,uint8_t max_col, int posx, int posy){
for(int col=(max_col-1);col>-1;col--){
  for(int row=0;row<8;row++){
    raw[((max_col-1)-col)+posx]+=((pgm_read_byte(&(chars[character-32][row]))>>col)&1)<<row;
	}
  }
for(uint8_t i=posx;i<(posx+max_col);i++){
  int tmp=posy;
  if(tmp>0){raw[i]>>=tmp;}
  else if(tmp<0){tmp=-tmp;raw[i]<<=tmp;}
  }
}
/*
void print_num(uint16_t num){
uint8_t n[4];
n[0]=(num/1000)+48;
n[1]=((num/100)%10)+48;
n[2]=((num/10)%10)+48;
n[3]=(num%10)+48;
empty_coloumn();

fill_buffer(n[0],5,0,0);
fill_buffer(n[1],5,6,0);
fill_buffer(n[2],5,12,0);
fill_buffer(n[3],5,18,0);
send(raw);
}*/

void char_slide_in(uint8_t max_col, uint8_t c1, uint8_t pos1, uint8_t c2, uint8_t pos2, uint8_t c3,uint8_t pos3, uint8_t c4,uint8_t pos4, uint8_t c5, uint8_t pos5){
for(int i=10;i>-1;i--){
  empty_coloumn();
  fill_buffer(c1,max_col,pos1,i);
  fill_buffer(c2,max_col,pos2,i);
  fill_buffer(c3,max_col,pos3,i);
  fill_buffer(c4,max_col,pos4,i);
  fill_buffer(c5,max_col,pos5,i);
  send(raw);
  _delay_ms(ANIM_DELAY);
  }
}

void char_slide_in_fall(uint8_t max_col, uint8_t c1, uint8_t pos1, uint8_t c2, uint8_t pos2, uint8_t c3,uint8_t pos3, uint8_t c4,uint8_t pos4, uint8_t c5, uint8_t pos5){
for(int i=13;i>-13;i--){
  int i1=i;if(i1<0){i1=0;}
  int i2=i+3;if(i2<0){i2=0;}
  int i3=i+6;if(i3<0){i3=0;}
  int i4=i+9;if(i4<0){i4=0;}
  int i5=i+12;if(i5<0){i5=0;}
  empty_coloumn();
  fill_buffer(c1,max_col,pos1,i5);
  fill_buffer(c2,max_col,pos2,i4);
  fill_buffer(c3,max_col,pos3,i3);
  fill_buffer(c4,max_col,pos4,i2);
  fill_buffer(c5,max_col,pos5,i1);
  send(raw);
  _delay_ms(ANIM_DELAY);
  }
}

void char_slide_out(uint8_t max_col,uint8_t c1, uint8_t pos1, uint8_t c2, uint8_t pos2, uint8_t c3,uint8_t pos3, uint8_t c4,uint8_t pos4, uint8_t c5, uint8_t pos5){
for(int i=0;i>-10;i--){
  empty_coloumn();
  fill_buffer(c1,max_col,pos1,i);
  fill_buffer(c2,max_col,pos2,i);
  fill_buffer(c3,max_col,pos3,i);
  fill_buffer(c4,max_col,pos4,i);
  fill_buffer(c5,max_col,pos5,i);
  send(raw);
  _delay_ms(ANIM_DELAY);
  }
}

void char_slide_out_fall(uint8_t max_col,uint8_t c1, uint8_t pos1, uint8_t c2, uint8_t pos2, uint8_t c3,uint8_t pos3, uint8_t c4,uint8_t pos4, uint8_t c5, uint8_t pos5){
for(int i=0;i>-26;i--){
  int i1=i;if(i1>0){i1=0;}
  int i2=i+3;if(i2>0){i2=0;}
  int i3=i+6;if(i3>0){i3=0;}
  int i4=i+9;if(i4>0){i4=0;}
  int i5=i+12;if(i5>0){i5=0;}
  empty_coloumn();
  fill_buffer(c1,max_col,pos1,i5);
  fill_buffer(c2,max_col,pos2,i4);
  fill_buffer(c3,max_col,pos3,i3);
  fill_buffer(c4,max_col,pos4,i2);
  fill_buffer(c5,max_col,pos5,i1);
  send(raw);
  _delay_ms(ANIM_DELAY);
  }
}


void time_slide(uint8_t hh, uint8_t mm, uint8_t big_font){
uint8_t h1=(hh/10)+48;uint8_t h2=(hh%10)+48;
uint8_t m1=(mm/10)+48;uint8_t m2=(mm%10)+48;
uint8_t max_col=5,pos1=0,pos2=7,pos3=20,pos4=27,colon=14;
if(big_font){h1+=76;h2+=76;m1+=76;m2+=76;max_col+=1;pos1=0;pos2=7;pos3=19;pos4=26;colon=13;}
if((h1==48)||(h1==124)){h1=32;}
char_slide_in_fall(max_col,h1,pos1,h2,pos2,':',colon,m1,pos3,m2,pos4);
_delay_ms(5000);
char_slide_out_fall(max_col,h1,pos1,h2,pos2,':',colon,m1,pos3,m2,pos4);
}

void date_slide(uint8_t dd, uint8_t mm){
uint8_t d1=(dd/10)+48;uint8_t d2=(dd%10)+48;
char_slide_in_fall(5,d1,0,d2,6,pgm_read_byte(&(mnth[mm][0])),15,pgm_read_byte(&(mnth[mm][1])),21,pgm_read_byte(&(mnth[mm][2])),27);
_delay_ms(700);
char_slide_out_fall(5,d1,0,d2,6,pgm_read_byte(&(mnth[mm][0])),15,pgm_read_byte(&(mnth[mm][1])),21,pgm_read_byte(&(mnth[mm][2])),27);
}

void temp_slide(uint8_t temp, uint8_t offset){
uint8_t t1=(temp/10)+48;uint8_t t2=(temp%10)+48;
char_slide_in_fall(5,t1,offset,t2,6+offset,123,12+offset,'C',17+offset,' ',23+offset);
_delay_ms(700);
char_slide_out_fall(5,t1,offset,t2,6+offset,123,12+offset,'C',17+offset,' ',23+offset);
}

int main(void){

spi_init();
display_init();


RTC_Start(93900,170920);

_delay_ms(100);

while(1){
          int hh=(RTC_TR/3600);
		  int mm=(RTC_TR/60)%60;
		  time_slide(hh,mm,0);
		  date_slide(19,9);
		  temp_slide(31,5);
		   
		  
		 }
}
