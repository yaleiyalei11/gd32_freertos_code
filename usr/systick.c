#include "systick.h"
#include "main.h"

#define USE_HAL_LEGACY

#define Timebase_Source_is_SysTick 1 // 当Timebase Source为SysTick时改为1
// #define Timebase_Source_is_SysTick 0 // 当使用FreeRTOS，Timebase Source为其他定时器时改为0

#if (!Timebase_Source_is_SysTick)
extern TIM_HandleTypeDef htim7; // 当使用FreeRTOS，Timebase Source为其他定时器时，修改为对应的定时器
#define Timebase_htim htim7

#define Delay_GetCounter() __HAL_TIM_GetCounter(&Timebase_htim)
#define Delay_GetAutoreload() __HAL_TIM_GetAutoreload(&Timebase_htim)
#else
#define Delay_GetCounter() (SysTick->VAL)
#define Delay_GetAutoreload() (SysTick->LOAD)
#endif

static uint16_t fac_us = 0;
static uint32_t fac_ms = 0;
static uint32_t g_systick = 0; // g_systick时钟变量，每触发一次systick，该变量增加1

extern void xPortSysTickHandler( void );
//extern BaseType_t xTaskGetSchedulerState( void );

void systick_config(void)
{
    /* setup systick timer for 1000Hz interrupts */
    if (SysTick_Config(SystemCoreClock / 1000U))
    {
        /* capture error */
        while (1)
        {
        }
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
}

/**
 * @brief  SysTick 中断函数
 * @author zbl (1473688511@qq.com)
 * @author dengax
 */
void SysTick_Handler(void)
{
    g_systick++;
//    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行
//    {
        xPortSysTickHandler();
//    }
}

/**
 * @brief  获取系统当前的systick
 * @return uint32_t: 当前的systick
 * @author zbl (1473688511@qq.com)
 */
uint32_t systick_get(void)
{
    return g_systick;
}

/**
 * @brief  获取上次系统的systick到当前系统的systick 距离多久
 * @param  last_tick:   上次系统的systick
 * @return uint32_t:
 * @author zbl (1473688511@qq.com)
 */
uint32_t systick_distance_get(uint32_t last_tick)
{
    return (g_systick - last_tick);
}

/**
 * @brief  延时初始化函数
 * @author zbl (1473688511@qq.com)
 */
void delay_init(void)
{
#if (!Timebase_Source_is_SysTick)
    fac_ms = 1000000; // 作为时基的计数器时钟频率在HAL_InitTick()中被设为了1MHz
    fac_us = fac_ms / 1000;
#else
    fac_ms = SystemCoreClock / 1000;
    fac_us = fac_ms / 1000;
#endif
}

/**
 * @brief  微秒级延时
 * @param  nus:
 * @author zbl (1473688511@qq.com)
 */
void delay_us(uint32_t nus)
{
    uint32_t ticks = 0;
    uint32_t told = 0;
    uint32_t tnow = 0;
    uint32_t tcnt = 0;
    uint32_t reload = 0;

    reload = Delay_GetAutoreload();

    ticks = nus * fac_us;

    told = Delay_GetCounter();

    while (1)
    {
        tnow = Delay_GetCounter();

        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;
            }
        }
    }
}

/**
 * @brief  毫秒级延时
 * @param  nms:
 * @author zbl (1473688511@qq.com)
 */
void delay_ms(uint32_t nms)
{
    uint32_t ticks = 0;
    uint32_t told = 0;
    uint32_t tnow = 0;
    uint32_t tcnt = 0;
    uint32_t reload = 0;

    reload = Delay_GetAutoreload();

    ticks = nms * fac_ms;

    told = Delay_GetCounter();

    while (1)
    {
        tnow = Delay_GetCounter();

        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;
            }
        }
    }
}

// /*重写HAL_Delay*/
// void HAL_Delay(uint32_t Delay)
// {
//     uint32_t tickstart = HAL_GetTick();
//     uint32_t wait = Delay;

//     /*不太明白官方源码为啥这么写，会多延时1ms，注释掉后更准*/
//     //  /* Add a freq to guarantee minimum wait */
//     //  if (wait < HAL_MAX_DELAY)
//     //  {
//     //    wait += (uint32_t)(uwTickFreq);
//     //  }

//     while ((HAL_GetTick() - tickstart) < wait)
//     {
//     }
// }
