#include "types.h"
//primery LCD drivers
void WriteLCD(unsigned char dat);
void CmdLCD(unsigned char cmd);
void CharLCD(unsigned char asciival);
void InitLCD(void);

//secondary LCD drivers
void StrLCD(char *s);
void U32LCD(unsigned int n);
void S32LCD(int n);
void F32LCD(f32 f,u8 nDP);
void BinLCD(u32 n,u8 nBD);
void HexLCD(u32 n);
void BuildCGRAM(u8 *p,u8 nBytes);



