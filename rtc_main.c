#include <lpc21xx.h>
#include <string.h>
#include "LCD_defines.h"
#include "LCD.h"
#include "adc.h"
#include "adc_defines.h"
#include "gp2xx.h"
#include "kpm.h"
#include "delay.h"

// RTC Macros
#define PREINT_VAL  ((PCLK / 32768) - 1)
#define PREFRAC_VAL (PCLK-((PREINT + 1) * 32768))
#define RTC_ENABLE  (1<<0)
#define RTC_RESET   (1<<1)
#define RTC_EXT_CLOCK (1<<4)

#define SW 0

void SetHOUR(void);
void SetMIN(void);
void SetSEC(void);
void SetDAY(void);
void SetMONTH(void);
void SetYEAR(void);
void SetDATE(void);
void SetEXIT(void);
void RTC_Init(void);

void RTC_SetTimeInfo(unsigned int hour, unsigned int min, unsigned int sec);
void RTC_SetDateInfo(unsigned int date, unsigned int month, unsigned int year);
void RTC_SetHour(unsigned int);
void RTC_SetMin(unsigned int);
void RTC_SetSec(unsigned int);
void RTC_SetDate(unsigned int);
void RTC_SetMonth(unsigned int);
void RTC_SetYear(unsigned int);
void RTC_SetDay(unsigned int);
void EDIT(void);

unsigned int RTC_GetHour(void);
unsigned int RTC_GetMin(void);
unsigned int RTC_GetSec(void);
unsigned int RTC_GetDate(void);
unsigned int RTC_GetMonth(void);
unsigned int RTC_GetYear(void);
unsigned int RTC_GetDay(void);

// Main function
float x;
int cm,ch,y,a;
char* lut[7]={"sun","mon","tue","wed","thu","fri","sat"};
int mlut[30]={7,8,9,0,4,5,6,0,1,2,3,0,0,0,0,0,0,};
char* data[30]={"1.hour","2.min","3.sec","8.exit","4.week","5.mon","6.year","7.del"};
	//char str[20]="1.h 2.m 3.s 8.e";
	//char str1[20]="4.w 5.m 6.y 7.d";

int main()
{
		RTC_Init();
		InitLCD();
    // Set the initial time (hours, minutes, seconds)
		RTC_SetTimeInfo(22,00,00);
	  RTC_SetDay(3);
	  RTC_SetDateInfo(1,9,24);
	  Init_ADC();
	  Init_KPM();
    while (1) 
		{
			a=(HOUR/10)+48;
        // Get and display the current time on LCD
			CmdLCD(0x80);
			CmdLCD(0x80);
			CharLCD(a);
			CharLCD((HOUR%10)+48);
			
			CharLCD(':');
			CharLCD((MIN/10)+48);
			CharLCD((MIN%10)+48);
			CharLCD(':');
			CharLCD((RTC_GetSec()/10)+48);
			CharLCD((RTC_GetSec()%10)+48);
			CmdLCD(GOTO_LINE1_POS0+9);
			StrLCD("DAY:");
			StrLCD(lut[DOW]);
			CmdLCD(GOTO_LINE2_POS0);
			CharLCD((DOM/10)+48);
			CharLCD((DOM%10)+48);
			CharLCD('/');
			CharLCD((MONTH/10)+48);
			CharLCD((MONTH%10)+48);
			CharLCD('/');
			U32LCD(YEAR);
			x=Read_ADC(CH1);
			x=x*100;
			y=x+1;
			cm=ADCValueToDiatance(x);
			CmdLCD(GOTO_LINE2_POS0+10);
			F32LCD(y,1);
			CharLCD(0xdf);
			CharLCD('C');
		  if(((IOPIN0>>SW)&1)==1)
			{
				EDIT();
			}	
			}	
    }	
// RTC Initialization: Configures and enables the RTC
void RTC_Init(void) 
{
    // Reset the RTC
		CCR = RTC_RESET;
    // Set prescaler integer part
		PREINT = PREINT_VAL;
    // Set prescaler fractional part
		PREFRAC = PREFRAC_VAL;
    // Enable the RTC
		CCR = RTC_ENABLE | RTC_EXT_CLOCK;
	
}
// RTC_SetTime: Sets the time and date on the RTC
void RTC_SetTimeInfo(unsigned int hour, unsigned int min, unsigned int sec) 
{

    // Set hours
		HOUR = hour;	  
    // Set minutes
		MIN = min;
	  // Set seconds
		SEC = sec;
	//delay_ms(10);


}
void RTC_SetDateInfo(unsigned int date, unsigned int month, unsigned int year)
{
DOM=date;
MONTH=month;
YEAR=year;
}
void RTC_SetDay(unsigned int day)
{
DOW=day;
}
unsigned int RTC_GetSec(void){
	
return SEC;
}
void EDIT(void){
CmdLCD(CLEAR_LCD);

while(1){
	//char str[20]="1.h 2.m 3.s 8.e";
	//char str1[20]="4.w 5.m 6.y 7.d";
	int i=0;
	char* data[30]={"1.hour","2.min","3.sec","8.exit","4.week","5.mon","6.year","7.del"};
	A1:
	if(i<1){
		i=0;
	}
	if(i>4){
		i=4;
	}
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD(data[i]);
	CmdLCD(GOTO_LINE1_POS0+9);
	StrLCD(data[i+1]);
	CmdLCD(GOTO_LINE2_POS0);
	StrLCD(data[i+2]);
	CmdLCD(GOTO_LINE2_POS0+9);
	StrLCD(data[i+3]);
	ch=keyscan();
	if(ch==16||ch==12){
		if(ch==16){
			i=i+4;
		}
		if(ch==12){
			i=i-4;
		}
		goto A1;
	}
	CmdLCD(CLEAR_LCD);
	switch(ch)
	{
		case 9:
			SetHOUR();
		break;
	  case 10:
			SetMIN();
		break;
		case 11:
			SetSEC();
		break;
		case 5:
			SetDAY();
		break;
		case 6:
			SetMONTH();
		break;
		case 7:
			SetYEAR();
		break;
		/*case 1:
			SetDELETE();
		break;*/
		case 2:
			return;
		case 0:
			break;
			
	}
  }

}


void SetHOUR(void)
{
	int num=0;
	L1:
	CmdLCD(CLEAR_LCD);
	StrLCD("Enter Hour");
	st1:
	while((ch=keyscan())!=15)
	{
		if(ch==4||ch==8||ch==12||ch==16||ch==13){
			delay_ms(10);
			goto st1;
		
		}
		num=num*10+mlut[ch-1];
		delay_ms(10);
		//CmdLCD(CLEAR_LCD);
		CmdLCD(GOTO_LINE2_POS0);
		U32LCD(num);
	}
	CmdLCD(CLEAR_LCD);
	if(num>0&&num<=23){
	HOUR=num;}
	else{
		CmdLCD(CLEAR_LCD);
		StrLCD("Enter valid hour");
		CmdLCD(GOTO_LINE2_POS0);
		num=0;
		goto L1;
		
	}
	
	
}
void SetMIN(void)
{
	int num=0;
	L2:
	
   num=0;
	CmdLCD(CLEAR_LCD);
	StrLCD("Enter MIN");
	st2:
	while((ch=keyscan())!=15)
	{
		if(ch==4||ch==8||ch==12||ch==16||ch==13){
			delay_ms(10);
			goto st2;}
		num=num*10+mlut[ch-1];
		delay_ms(10);
		//CmdLCD(CLEAR_LCD);
		CmdLCD(GOTO_LINE2_POS0);
		U32LCD(num);
	}
	CmdLCD(CLEAR_LCD);
	if(num>0&&num<=59){
	MIN=num;}
	else{
		CmdLCD(CLEAR_LCD);
		StrLCD("ENTER VALID MIN");
		CmdLCD(GOTO_LINE2_POS0);
		goto L2;
	}
	
	
}

void SetSEC(void)
{
	int num=0;
	L3:
	num=0;
	CmdLCD(CLEAR_LCD);
	StrLCD("Enter SEC");
	st3:
	while((ch=keyscan())!=15)
	{
		if(ch==4||ch==8||ch==12||ch==16||ch==13){
			delay_ms(10);
			goto st3;}
		num=num*10+mlut[ch-1];
		delay_ms(10);
		//CmdLCD(CLEAR_LCD);
		CmdLCD(GOTO_LINE2_POS0);
		U32LCD(num);
	}
	CmdLCD(CLEAR_LCD);
	if(num>0&&num<=59){
	SEC=num;}
	else{
		CmdLCD(CLEAR_LCD);
		StrLCD("ENTER VALID SEC");
		CmdLCD(GOTO_LINE2_POS0);
		goto L3;
	}
	
	
}




void SetMONTH(void)
{
	int num=0;
	L5:
	num=0;
	CmdLCD(CLEAR_LCD);
	StrLCD("Enter MONTH");
	st4:
	while((ch=keyscan())!=15)
	{
		if(ch==4||ch==8||ch==12||ch==16||ch==13){
			delay_ms(10);
			goto st4;}
		num=num*10+mlut[ch-1];
		delay_ms(10);
		//CmdLCD(CLEAR_LCD);
		CmdLCD(GOTO_LINE2_POS0);
		U32LCD(num);
	}
	CmdLCD(CLEAR_LCD);
	if(num>=1&&num<=12){
	MONTH=num;}
	else{
		CmdLCD(CLEAR_LCD);
		StrLCD("ENTER VALID MONTH");
		CmdLCD(GOTO_LINE2_POS0);
		goto L5;
	}
}


void SetYEAR(void)
{
	int num=0;
	L6:
	num=0;
	CmdLCD(CLEAR_LCD);
	StrLCD("Enter DATE");
	st5:
	while((ch=keyscan())!=15)
	{
		if(ch==4||ch==8||ch==12||ch==16||ch==13){
			delay_ms(10);
			goto st5;}
		num=num*10+mlut[ch-1];
		delay_ms(10);
		//CmdLCD(CLEAR_LCD);
		CmdLCD(GOTO_LINE2_POS0);
		U32LCD(num);
	}
	CmdLCD(CLEAR_LCD);
	if(num>0){
	YEAR=num;}
	else{
		CmdLCD(CLEAR_LCD);
		StrLCD("ENTER VALID YEAR");
		CmdLCD(GOTO_LINE2_POS0);
		goto L6;
	}
}

void SetDAY(void)
{
	int num=0;
	L7:
	num=0;
	CmdLCD(CLEAR_LCD);
	StrLCD("Enter DAY");
	st6:
	while((ch=keyscan())!=15)
	{
		
		if(ch==4||ch==8||ch==12||ch==16||ch==13){
			delay_ms(10);
			goto st6;}
		num=num*10+mlut[ch-1];
		delay_ms(10);
		//CmdLCD(CLEAR_LCD);
		CmdLCD(GOTO_LINE2_POS0);
		U32LCD(num);
	}
	CmdLCD(CLEAR_LCD);
	if(num>=0&&num<=6){
	DOW=num-1;}
	else{
		CmdLCD(CLEAR_LCD);
		StrLCD("ENTER VALID DAY");
		CmdLCD(GOTO_LINE2_POS0);
		goto L7;
	}
}
