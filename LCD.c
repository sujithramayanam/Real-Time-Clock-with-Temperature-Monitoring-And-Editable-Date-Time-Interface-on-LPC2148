#include <lpc21xx.h>
#include "LCD_defines.h"
#include "delay.h"
#include "types.h"
void WriteLCD(unsigned char dat)
{
	//clear the r/w 
	IOCLR0=1<<LCD_RW;
	//write the data on lcd data pins
	IOPIN0=(IOPIN0&~(255<<LCD_DATA))|(dat<<LCD_DATA);
	//to latch the data into lcd make en high to low
	IOSET0=1<<LCD_EN;//HIGH
	delay_us(1);
	IOCLR0=1<<LCD_EN;//LOW
	//maintain dealy for internal process
	delay_ms(2);
}
void CmdLCD(unsigned char cmd)
{
	//select the cmd/inst reg
	IOCLR0=1<<LCD_RS;
	//write the command into command reg
	WriteLCD(cmd);
}

void CharLCD(unsigned char asciival)
{
	//select the data reg
	IOSET0=1<<LCD_RS;
	//write the asciival to lcd as ddram/cgram
	WriteLCD(asciival);
}
void InitLCD(void)
{
	IODIR0|=0XFF<<LCD_DATA|7<<LCD_RS;
	delay_ms(15);
	CmdLCD(0x30);
	delay_ms(5);
	CmdLCD(0x30);
	delay_us(100);
	CmdLCD(0x30);
	CmdLCD(MODE_8BIT_2LINE);
	CmdLCD(DSP_ON_CUR_OFF);
	CmdLCD(CLEAR_LCD);
	CmdLCD(SHIFT_CUR_RIGHT);
}
void StrLCD(char *s)
{
	while(*s)
	CharLCD(*s++);
}
void U32LCD(unsigned int n)
{
	int i=0;
	unsigned char a[10];
	if(n==0)
	{
		CharLCD('0');
	}
	else
	{
		while(n)
		{
			a[i++]=(n%10)+48;
			n/=10;
		}
		for(--i; i>=0; i--)
		CharLCD(a[i]);
	}
	
}
void S32LCD(int n)
{
	if(n<0)
	{
		CharLCD('-');
		n=-n;
	}
	U32LCD(n);
}

void F32LCD(f32 f,u8 nDP)
{
	u32 n,i;
	if(f<0.0)
	{
		CharLCD('-');
		f=-f;
	}
	n=f;
	U32LCD(n);
	CharLCD('.');
	for(i=0;i<nDP;i++)
	{
		f=(f-n)*10;
		n=f;
		CharLCD(n+48);
	}
}

void BinLCD(u32 n,u8 nBD)
{
  s32 i;
	for(i=nBD;i>=0;i--)
	{
		CharLCD(((n>>i)&1)+48);
	}
}	

void HexLCD(u32 n)
{
   s32 i=0;   u8 a[8],t;
	 if(n==0)
		 CharLCD('0');
	 else
	 {
		 while(n>0)
		 {
			 ((t=(n%16))<10)? (t=t+48): (t=(t-10)+'A');
			 a[i++]=t;
			 n/=16;
		 }
		 for(--i;i>=0;i--)
		   CharLCD(a[i]);
	 }
 }
void BuildCGRAM(u8 *p,u8 nBytes)
{
	u32 i;
	//point to cgram start
	CmdLCD(GOTO_CGRAM);
	for(i=0;i<nBytes;i++)
	{
		//write to cgram via data reg
		CharLCD(p[i]);
	}
	//return back to ddram
	CmdLCD(GOTO_LINE1_POS0);
}


	



