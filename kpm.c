#include <LPC21xx.h>
#include "types.h"
#include "defines.h"
#include "KPM_defines.h"
u8 KPMLUT[4][4]={
	               {1,2,3,4},
								 {5,6,7,8},
								 {9,10,11,12},
								 {13,14,15,16},
                };
u32 colscan(void){
	return (READNIBBLE(IOPIN1,COL0)<15)?0:1;
}
u32 rowcheck(void){
u32 rno;
	//ground 0th row 
	SCLRBIT(IOCLR1,ROW0);
	SSETBIT(IOSET1,ROW1);
	SSETBIT(IOSET1,ROW2);
	SSETBIT(IOSET1,ROW3);
	if(colscan()==0){
		rno=0;
		goto extrowcheck;
	
	}
	//ground 1st r0w
	SCLRBIT(IOCLR1,ROW1);
	SSETBIT(IOSET1,ROW0);
	SSETBIT(IOSET1,ROW2);
	SSETBIT(IOSET1,ROW3);
	if(colscan()==0){
		rno=1;
		goto extrowcheck;
	}
	//gnd 2nd r0w
	SCLRBIT(IOCLR1,ROW2);
	SSETBIT(IOSET1,ROW0);
	SSETBIT(IOSET1,ROW1);
	SSETBIT(IOSET1,ROW3);
	if(colscan()==0){
		rno=2;
		goto extrowcheck;
	}
	//gnd 3rd row
	SCLRBIT(IOCLR1,ROW3);
	SSETBIT(IOSET1,ROW0);
	SSETBIT(IOSET1,ROW2);
	SSETBIT(IOSET1,ROW1);
	if(colscan()==0){
		rno=3;
		goto extrowcheck;
	}
	extrowcheck:
	//reintialize all
	SCLRBIT(IOCLR1,ROW0);
	SCLRBIT(IOCLR1,ROW1);
	SCLRBIT(IOCLR1,ROW2);
	SCLRBIT(IOCLR1,ROW3);
	return rno;
}

u32 colcheck(void){
u32 cno;
	for(cno=0;cno<=3;cno++){
	 if(READBIT(IOPIN1,COL0+cno)==0)
		 break;
	}
	return cno;
}


u32 keyscan(void){
u32 rno,cno,keyv;
	//wait to detect key press
	while(colscan());
	//identify row
	rno=rowcheck();
	//identify col
	cno=colcheck();
	//extract key values
	keyv=KPMLUT[rno][cno];
	return keyv;
}

void Init_KPM(void){
//cfg row lines as gpio output
	WRITENIBBLE(IODIR1,ROW0,15);
	//hence all rows are grounded
	//default all col lines
}
