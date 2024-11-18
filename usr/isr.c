#include <stdio.h>
#include "gd32f30x.h"
#include "main.h"

/* function declarations */
extern SemaphoreHandle_t xSemaphore;

/* 定时器中断 */
void TIMER0_UP_IRQHandler(void);
void TIMER1_IRQHandler(void);
void TIMER2_IRQHandler(void);
void TIMER3_IRQHandler(void);
void TIMER4_IRQHandler(void);
void TIMER5_IRQHandler(void);
void TIMER6_IRQHandler(void);
void TIMER7_UP_IRQHandler(void);

/* 串口中断 */
void USART0_IRQHandler(void);
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
/* this function handles NMI exception */
void NMI_Handler(void);
/* this function handles HardFault exception */
void HardFault_Handler(void);
/* this function handles MemManage exception */
void MemManage_Handler(void);
/* this function handles BusFault exception */
void BusFault_Handler(void);
/* this function handles UsageFault exception */
void UsageFault_Handler(void);
/* this function handles SVC exception */
void SVC_Handler(void);
/* this function handles DebugMon exception */
void DebugMon_Handler(void);
/* this function handles PendSV exception */
void PendSV_Handler(void);

/**
 * @brief  TIMER0 中断服务函数
 * @author zbl (1473688511@qq.com)
 */
void TIMER0_UP_IRQHandler(void)
{
    if (timer_interrupt_flag_get(TIMER0, TIMER_INT_FLAG_UP))
    {

        /* 清除TIMER 中断标志位 */
        timer_interrupt_flag_clear(TIMER0, TIMER_INT_FLAG_UP);
    }
}

/**
 * @brief  TIMER1 中断服务函数
 * @author Peng GuangFeng (2304146968@qq.com)
 */
void TIMER1_IRQHandler(void)
{
    if (timer_interrupt_flag_get(TIMER1, TIMER_INT_FLAG_UP))
    {
        /*用于上下文切换，初始值最好设置为pdFALSE*/
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        /*多次释放信号量，将中断获得的数据推迟到任务集中处理，以免中断阻塞*/
        xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);
        xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);
        xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);

        /*因为中断结束后必然是start_task执行，因此此处要申请上下文切换*/
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        /* 清除TIMER 中断标志位 */
        timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);
    }
}

/**
 * @brief  TIMER2 中断服务函数
 * @author zbl (1473688511@qq.com)
 */
void TIMER2_IRQHandler(void)
{
    if (timer_interrupt_flag_get(TIMER2, TIMER_INT_FLAG_UP))
    {

        /* 清除TIMER2 中断标志位 */
        timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_UP);
    }
}

/**
 * @brief  TIMER3 中断服务函数
 * @author zbl (1473688511@qq.com)
 */
void TIMER3_IRQHandler(void)
{
    if (timer_interrupt_flag_get(TIMER3, TIMER_INT_FLAG_UP))
    {

        /* 清除TIMER 中断标志位 */
        timer_interrupt_flag_clear(TIMER3, TIMER_INT_FLAG_UP);
    }
}

/**
 * @brief  TIMER4 中断服务函数
 * @author zbl (1473688511@qq.com)
 */
void TIMER4_IRQHandler(void)
{
    if (timer_interrupt_flag_get(TIMER4, TIMER_INT_FLAG_UP))
    {

        /* 清除TIMER5 中断标志位 */
        timer_interrupt_flag_clear(TIMER4, TIMER_INT_FLAG_UP);
    }
}


/**
 * @brief  TIMER5 中断服务函数
 * @author zbl (1473688511@qq.com)
 */
void TIMER5_IRQHandler(void)
{
    if (timer_interrupt_flag_get(TIMER5, TIMER_INT_FLAG_UP))
    {


        /* 清除TIMER5 中断标志位 */
        timer_interrupt_flag_clear(TIMER5, TIMER_INT_FLAG_UP);
    }
}

/**
 * @brief  TIMER6 中断服务函数
 * @author zbl (1473688511@qq.com)
 */
void TIMER6_IRQHandler(void)
{
    if (timer_interrupt_flag_get(TIMER6, TIMER_INT_FLAG_UP))
    {

        /* 清除TIMER 中断标志位 */
        timer_interrupt_flag_clear(TIMER6, TIMER_INT_FLAG_UP);
    }
}

/**
 * @brief  TIMER7 中断服务函数
 * @author zbl (1473688511@qq.com)
 */
void TIMER7_UP_IRQHandler(void)
{
    if (timer_interrupt_flag_get(TIMER7, TIMER_INT_FLAG_UP))
    {

        /* 清除TIMER 中断标志位 */
        timer_interrupt_flag_clear(TIMER7, TIMER_INT_FLAG_UP);
    }
}

/**
 * @brief  USART0 中断服务函数
 * @author zbl (1473688511@qq.com)
 */
void USART0_IRQHandler(void)
{
    if (RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE))
    {

        usart_interrupt_flag_clear(USART0, USART_INT_FLAG_RBNE);
    }
}

/**
 * @brief  USART1 中断服务函数
 * @author zbl (1473688511@qq.com)
 */
void USART1_IRQHandler(void)
{
    if (RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_RBNE))
    {

        usart_interrupt_flag_clear(USART1, USART_INT_FLAG_RBNE);
    }
}

/**
 * @brief  USART2 中断服务函数
 * @author zbl (1473688511@qq.com)
 */
void USART2_IRQHandler(void)
{
    if (RESET != usart_interrupt_flag_get(USART2, USART_INT_FLAG_RBNE))
    {

        usart_interrupt_flag_clear(USART2, USART_INT_FLAG_RBNE);
    }
}

/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while (1)
    {
    }
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while (1)
    {
    }
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while (1)
    {
    }
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while (1)
    {
    }
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}
