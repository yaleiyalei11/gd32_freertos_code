#include "timer.h"
#include "gd32f30x.h"


/* 静态变量声明，使用静态变量是为了避免污染命名空间 */
static const rcu_periph_enum TIMER_RCU[TIMER_NUM] = {RCU_TIMER0, RCU_TIMER1, RCU_TIMER2, RCU_TIMER3, RCU_TIMER4, RCU_TIMER5, RCU_TIMER6, RCU_TIMER7};
static const uint32_t TIMER_ADDR[TIMER_NUM] = {TIMER0, TIMER1, TIMER2, TIMER3, TIMER4, TIMER5, TIMER6, TIMER7};
static const uint8_t TIMER_IRQ[TIMER_NUM] = {TIMER0_UP_TIMER9_IRQn, TIMER1_IRQn, TIMER2_IRQn, TIMER3_IRQn, TIMER4_IRQn, TIMER5_IRQn, TIMER6_IRQn, TIMER7_UP_TIMER12_IRQn};

/**
 * @brief   定时器初始化函数
 * @param  TIMx:    定时器索引
 * @param  psr:     时钟预分频系数
 * @param  arr:     自动重装载值
 * @param  pre_priority:    中断抢占优先级
 * @param  sub_priority:    中断响应优先级
 * @author zbl (1473688511@qq.com)
 */
void my_timer_init(timer_e TIMx, uint32_t psr, uint32_t arr, uint8_t pre_priority, uint8_t sub_priority)
{
    /* 定义一个定时器初始化结构体 */
    timer_parameter_struct timer_init_struct;

    /* 开启定时器时钟 */
    rcu_periph_clock_enable(TIMER_RCU[TIMx]);

    /* 初始化定时器 */
    timer_deinit(TIMER_ADDR[TIMx]);
    timer_init_struct.prescaler = psr;                     /* 预分频系数 */
    timer_init_struct.period = arr;                        /* 自动重装载值 */
    timer_init_struct.alignedmode = TIMER_COUNTER_EDGE;    /* 计数器对齐模式，边沿对齐 */
    timer_init_struct.counterdirection = TIMER_COUNTER_UP; /* 计数器计数方向，向上 */
    timer_init_struct.clockdivision = TIMER_CKDIV_DIV1;    /* DTS时间分频值 */
    timer_init_struct.repetitioncounter = 0;               /* 重复计数器的值 */
    timer_init(TIMER_ADDR[TIMx], &timer_init_struct);

    /* Timer中断设置，抢占优先级pre_priority，响应优先级sub_priority */
    nvic_irq_enable(TIMER_IRQ[TIMx], pre_priority, sub_priority);
    /* 使能Timer更新中断 */
    timer_interrupt_enable(TIMER_ADDR[TIMx], TIMER_INT_UP);
    /* 使能Timer */
    timer_enable(TIMER_ADDR[TIMx]);
}
