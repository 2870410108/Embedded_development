
#ifndef _Timer_H_
#define _Timer_H_

#include "stm32f10x.h"

void init_timer_3(void);
uint64_t get_ms(void);
void set_ms(uint8_t temp);
#endif
