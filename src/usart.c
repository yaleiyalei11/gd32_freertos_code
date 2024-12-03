#include "usart.h"

/* 静态变量声明，使用静态变量是为了避免污染命名空间 */
static const uint32_t USART[USART_NUM] = {USART0, USART1};
static const uint32_t GPIO_PORT[USART_NUM] = {USART0_GPIO_PORT, USART1_GPIO_PORT};
static const uint32_t GPIO_PIN[USART_NUM][2] = {{USART0_TX_PIN, USART0_RX_PIN}, {USART1_TX_PIN, USART1_RX_PIN}};
static const rcu_periph_enum GPIO_CLK[USART_NUM] = {USART0_GPIO_CLK, USART1_GPIO_CLK};
static const rcu_periph_enum USART_CLK[USART_NUM] = {USART0_CLK, USART1_CLK};
static const uint8_t USART_IRQ[USART_NUM] = {USART0_IRQn, USART1_IRQn};

uint8_t USART_RX_Buffer[USART_RX_BUF] = {0};
uint8_t usart_buf_index = 0;

/**
 * @brief  串口输出重定向
 * @param  ch:
 * @param  f:
 * @return int:
 * @author zbl (1473688511@qq.com)
 */
int fputc(int ch, FILE *f)
{
    while (RESET == usart_flag_get(DEBUG_USART, USART_FLAG_TBE));
    usart_data_transmit(DEBUG_USART, (uint8_t)ch);
    return ch;
}

/**
 * @brief  串口初始化
 * @param  usartx: 串口索引
 * @param  bound: 波特率
 * @param  pre_priority:    抢占优先级
 * @param  sub_priority:    响应优先级
 * @author zbl (1473688511@qq.com)
 */
void usart_init(usart_e usartx, uint32_t bound, uint8_t pre_priority, uint8_t sub_priority)
{
    /* 使能 GPIO 时钟 */
    rcu_periph_clock_enable(GPIO_CLK[usartx]);

    /* 使能 USART 时钟 */
    rcu_periph_clock_enable(USART_CLK[usartx]);

    /* 复用为 USART0_Tx */
    gpio_init(GPIO_PORT[usartx], GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN[usartx][PIN_TX]);

    /* 复用为 USARTx_Rx */
    gpio_init(GPIO_PORT[usartx], GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN[usartx][PIN_RX]);

    /* USART 初始化配置 */
    usart_deinit(USART[usartx]);
    usart_baudrate_set(USART[usartx], bound);                         // 设置波特率
    usart_word_length_set(USART[usartx], USART_WL_8BIT);              // 数据为设置
    usart_stop_bit_set(USART[usartx], USART_STB_1BIT);                // 停止位设置
    usart_parity_config(USART[usartx], USART_PM_NONE);                // 校验位设置
    usart_hardware_flow_rts_config(USART[usartx], USART_RTS_DISABLE); // 硬件流控制RTS
    usart_hardware_flow_cts_config(USART[usartx], USART_CTS_DISABLE); // 硬件流控制CTS
    usart_receive_config(USART[usartx], USART_RECEIVE_ENABLE);        // 使能接收
    usart_transmit_config(USART[usartx], USART_TRANSMIT_ENABLE);      // 使能发送
    usart_enable(USART[usartx]);                                      // 使能串口

    /* Usart中断设置，抢占优先级pre_priority，响应优先级sub_priority */
    nvic_irq_enable(USART_IRQ[usartx], pre_priority, sub_priority);
    usart_interrupt_enable(USART[usartx], USART_INT_RBNE); // 接收中断
}
