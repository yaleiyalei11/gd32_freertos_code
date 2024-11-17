#include "led.h"
#include "systick.h"

/* 静态变量声明，使用静态变量是为了避免污染命名空间 */
static const uint32_t GPIO_PORT[LED_NUM] = {LED1_PORT, LED2_PORT, LED3_PORT, LED4_PORT, LED5_PORT, LED6_PORT, LED7_PORT, LED_PORT};
static const uint32_t GPIO_PIN[LED_NUM] = {LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN, LED5_PIN, LED6_PIN, LED7_PIN, LED_PIN};
static const rcu_periph_enum GPIO_CLK[LED_NUM] = {LED1_CLK, LED2_CLK, LED3_CLK, LED4_CLK, LED5_CLK, LED6_CLK, LED7_CLK, LED_CLK};

/**
 * @brief  所有led初始化
 * @author zbl (1473688511@qq.com)
 */
void leds_all_init(void)
{
    for (int i = 0; i < LED_NUM; i++)
    {
        led_single_init((led_e)i);
    }
}

/**
 * @brief   单独led初始化
 * @param  ledNum:  led序号
 * @author zbl (1473688511@qq.com)
 * @note led_single_init(LED2);
 */
void led_single_init(led_e ledNum)
{
    /* 使能led时钟 */
    rcu_periph_clock_enable(GPIO_CLK[ledNum]);

    /* 初始化led引脚 配置为推挽输出，速度为50MHZ */
    gpio_init(GPIO_PORT[ledNum], GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN[ledNum]);

    /* 设置为低电平 */
    gpio_bit_set(GPIO_PORT[ledNum], GPIO_PIN[ledNum]);
}

/**
 * @brief  led 开
 * @param  ledNum:   led序号
 * @author zbl (1473688511@qq.com)
 * @note 示例：led_on(LED2);
 */
void led_on(led_e ledNum)
{
    /* 设置为高电平 */
    gpio_bit_reset(GPIO_PORT[ledNum], GPIO_PIN[ledNum]);
}

/**
 * @brief  led 关
 * @param  ledNum: led序号
 * @author zbl (1473688511@qq.com)
 */
void led_off(led_e ledNum)
{
    /* 设置为低电平 */
    gpio_bit_set(GPIO_PORT[ledNum], GPIO_PIN[ledNum]);
}

/**
 * @brief  反转led
 * @param  ledNum: led序号
 * @author zbl (1473688511@qq.com)
 */
void led_toggle(led_e ledNum)
{
    FlagStatus ledStatus = gpio_input_bit_get(GPIO_PORT[ledNum], GPIO_PIN[ledNum]); // 读取当前LED高低电平
    gpio_bit_write(GPIO_PORT[ledNum], GPIO_PIN[ledNum], (bit_status)!ledStatus);    // 写入取反后电平
}

/**
 * @brief        : 流水灯(阻塞式)
 * @return        {*}
 * @author       : dengax
**/
void led_flow(void)
{
    for(int i = 0; i < LED_NUM-1; i++)
    {
        gpio_bit_reset(GPIO_PORT[i], GPIO_PIN[i]);
        delay_ms(20);
        gpio_bit_set(GPIO_PORT[i], GPIO_PIN[i]);
    }
}


/**
 * @brief        : 流水灯（非阻塞）
 * @return        {*}
 * @author       : dengax
**/
void led_flow_task(void)
{
    static uint8_t flow_cnt = 0;
    gpio_bit_set(GPIO_PORT[flow_cnt], GPIO_PIN[flow_cnt]);
    flow_cnt++;
    if(flow_cnt >= 7) flow_cnt = 0;
    gpio_bit_reset(GPIO_PORT[flow_cnt], GPIO_PIN[flow_cnt]);
}

