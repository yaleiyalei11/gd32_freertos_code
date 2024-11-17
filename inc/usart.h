#ifndef _INC_USART_H
#define _INC_USART_H

#include <stdio.h>
#include "gd32f30x.h"

#define USART_NUM 2
#define USART_RX_BUF 256

#define DEBUG_USART USART0

#define USART0_CLK RCU_USART0
#define USART0_TX_PIN GPIO_PIN_9
#define USART0_RX_PIN GPIO_PIN_10
#define USART0_GPIO_PORT GPIOA
#define USART0_GPIO_CLK RCU_GPIOA

#define USART1_CLK RCU_USART1
#define USART1_TX_PIN GPIO_PIN_2
#define USART1_RX_PIN GPIO_PIN_3
#define USART1_GPIO_PORT GPIOA
#define USART1_GPIO_CLK RCU_GPIOA

#define USART2_CLK RCU_USART2
#define USART2_TX_PIN GPIO_PIN_8
#define USART2_RX_PIN GPIO_PIN_9
#define USART2_GPIO_PORT GPIOD
#define USART2_GPIO_CLK RCU_GPIOD

typedef enum
{
    USART_0 = 0,
    USART_1,
		USART_2,
	
} usart_e;

typedef enum
{
    PIN_TX = 0,
    PIN_RX,
} usart_pin_e;

void usart_init(usart_e usartx, uint32_t bound, uint8_t pre_priority, uint8_t sub_priority);

extern uint8_t USART_RX_Buffer[USART_RX_BUF];
extern uint8_t usart_buf_index;

#endif
