#ifndef _INC_TIMER_H
#define _INC_TIMER_H

#include <stdio.h>
#include "gd32f30x.h"
#include "gd32f30x_timer.h"

#define TIMER_NUM 8 // 定时器数

typedef enum
{
    TIM0,
    TIM1,
    TIM2,
    TIM3,
    TIM4,
    TIM5,
    TIM6,
    TIM7,
} timer_e;

void my_timer_init(timer_e TIMx, uint32_t psr, uint32_t arr, uint8_t pre_priority, uint8_t sub_priority);

#endif // _INC_TIMER_H
