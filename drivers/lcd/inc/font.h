#ifndef __FONT_H
#define __FONT_H 	   

#include "Stm32f2xx.h"


typedef struct
{								// ×Ö·û¿â½á¹¹
    uint8_t Index[1];
    uint8_t Mask[16];
}LittleCharStruct;


extern LittleCharStruct LittleCharLib[];
extern const uint8_t ascii_8x16[];
extern const uint8_t ascii_0816[];
extern const uint8_t ascii_bmp[];
extern const uint8_t skyborne_bmp[];

#endif
