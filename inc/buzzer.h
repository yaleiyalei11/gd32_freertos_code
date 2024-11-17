#ifndef _INC_BUZZER_H
#define _INC_BUZZER_H

#include <stdio.h>
#include "gd32f30x.h"
#include "gd32f30x_gpio.h"

#define BUZZER_PORT GPIOF
#define BUZZER_PIN GPIO_PIN_9
#define BUZZER_CLK RCU_GPIOF

void buzzer_init(void);
void buzzer_on(void);
void buzzer_off(void);

#endif /* _INC_BUZZER */
