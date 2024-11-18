#ifndef __SYSTICK_H
#define __SYSTICK_H

#include <stdint.h>
#include "gd32f30x.h"

void systick_config(void);
void SysTick_Handler(void);
uint32_t systick_get(void);
uint32_t systick_distance_get(uint32_t last_tick);
extern void delay_init(void);
extern void delay_us(uint32_t nus);
extern void delay_ms(uint32_t nms);

#endif // __SYSTICK_H
