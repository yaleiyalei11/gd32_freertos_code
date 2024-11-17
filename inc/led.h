#ifndef _INC_LED_H
#define _INC_LED_H

#include <stdio.h>
#include "gd32f30x.h"
#include "gd32f30x_gpio.h"

#define LED_NUM 8

#define LED_PORT GPIOB
#define LED_PIN GPIO_PIN_13
#define LED_CLK RCU_GPIOB

#define LED1_PORT GPIOG
#define LED1_PIN GPIO_PIN_2
#define LED1_CLK RCU_GPIOG

#define LED2_PORT GPIOG
#define LED2_PIN GPIO_PIN_3
#define LED2_CLK RCU_GPIOG

#define LED3_PORT GPIOG
#define LED3_PIN GPIO_PIN_4
#define LED3_CLK RCU_GPIOG

#define LED4_PORT GPIOG
#define LED4_PIN GPIO_PIN_5
#define LED4_CLK RCU_GPIOG

#define LED5_PORT GPIOG
#define LED5_PIN GPIO_PIN_6
#define LED5_CLK RCU_GPIOG

#define LED6_PORT GPIOG
#define LED6_PIN GPIO_PIN_7
#define LED6_CLK RCU_GPIOG

#define LED7_PORT GPIOG
#define LED7_PIN GPIO_PIN_8
#define LED7_CLK RCU_GPIOG

typedef enum
{
    LED1 = 0,
    LED2,
    LED3,
    LED4,
    LED5,
    LED6,
    LED7,
    LED
} led_e;

void leds_all_init(void);
void led_single_init(led_e ledNum);
void led_on(led_e ledNum);
void led_off(led_e ledNum);
void led_toggle(led_e ledNum);
void led_flow(void);
void led_flow_task(void);

#endif /* _INC_LED */
