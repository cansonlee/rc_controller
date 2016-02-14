#ifndef __DLY_TIMER_H
#define __DLY_TIMER_H

#include "Stm32f2xx.h"
#include "Misc.h"
#include "Stm32f2xx_rcc.h"
#include "Stm32f2xx_tim.h"


#include <stdint.h>

void delay_init(void);
void delay_01us(uint16_t nb);
void delay_ms(uint32_t ms);

#endif

