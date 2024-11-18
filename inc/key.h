#ifndef _INC_KEY_H
#define _INC_KEY_H

#include <stdio.h>
#include "gd32f30x.h"
#include "gd32f30x_gpio.h"

#define KEY_NUM 4

#define KEY1_PORT GPIOA
#define KEY1_PIN GPIO_PIN_0
#define KEY1_CLK RCU_GPIOA

#define KEY2_PORT GPIOC
#define KEY2_PIN GPIO_PIN_13
#define KEY2_CLK RCU_GPIOC

#define KEY3_PORT GPIOF
#define KEY3_PIN GPIO_PIN_5
#define KEY3_CLK RCU_GPIOF

#define KEY4_PORT GPIOF
#define KEY4_PIN GPIO_PIN_4
#define KEY4_CLK RCU_GPIOF

void keys_init(void);
void keys_proc(void);

#endif // _INC_KEY_H
