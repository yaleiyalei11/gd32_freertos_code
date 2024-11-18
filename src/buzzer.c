/**
 * @Descripttion : ·äÃùÆ÷Çý¶¯
 * @version      : 
 * @Author       : dengax
 * @Date         : 2023-11-09 13:12:46
 * @LastEditors  : dengax
 * @LastEditTime : 2023-11-11 21:40:50
**/
#include "buzzer.h"

/**
 * @brief        : ·äÃùÆ÷³õÊ¼»¯
 * @return        {*}
 * @author       : dengax
**/
void buzzer_init(void)
{
    rcu_periph_clock_enable(BUZZER_CLK);
    gpio_init(BUZZER_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, BUZZER_PIN);
    gpio_bit_set(BUZZER_PORT, BUZZER_PIN);
}

/**
 * @brief        : ¿ªÆô·äÃùÆ÷
 * @return        {*}
 * @author       : dengax
**/
void buzzer_on(void)
{
    gpio_bit_reset(BUZZER_PORT, BUZZER_PIN);
}

/**
 * @brief        : ¹Ø±Õ·äÃùÆ÷
 * @return        {*}
 * @author       : dengax
**/
void buzzer_off(void)
{
    gpio_bit_set(BUZZER_PORT, BUZZER_PIN);
}
