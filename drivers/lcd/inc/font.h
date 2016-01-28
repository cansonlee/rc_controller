#ifndef __FONT_H
#define __FONT_H 	   

#include "Stm32f2xx.h"


typedef struct
{								// ×Ö·û¿â½á¹¹
    uint8_t Index[1];
    uint8_t Mask[14];
}LittleCharStruct;



void dispCharacter(uint8_t x,uint8_t y,uint8_t dispByte);
void dispCharStr(unsigned char x,unsigned char y,unsigned char *pCharStr);



#endif
