/**
 * @Descripttion : freerots创建软件定时器，实现软件定时器的控制
 * @version      : 2024嵌入式组
 * @Author       : Peng GuangFeng( 2304146968@qq.com )
 * @Date         : 2024-11-13 
 * @LastEditors  : Peng GuangFeng
 * @LastEditTime : 2024-11-13
 * @homework     : 学习之后建议尝试以下功能：
 *								 1.尝试不同定时器调用不同回调函数
 *								 2.尝试更改使用API函数更改定时器的定时周期
 *								 3.尝试调用定时器ID
 *								 4.理解守护进程与定时器队列的工作关系
**/
#include "main.h"
#include "gd32f30x.h"
#include "led.h"
#include "usart.h"

//本节采用该方式计算真实时间
#define Timer_Period pdMS_TO_TICKS(1000)
#define Timer_Period_2 pdMS_TO_TICKS(333)

#define Timer_ID (void *)1
#define Timer_ID_2 (void *)2

/*定时器句柄*/
TimerHandle_t xTimer1; 
TimerHandle_t xTimer2;

/* 定时器回调函数，两个定时都回调此函数 */
static void prvTimer_Callback(TimerHandle_t xTimer)
{
    /* 获取当前滴答数，验证软件定时器的正确 */
    TickType_t xTimeNow = xTaskGetTickCount();

    /*判断来自哪一个定时器*/
    if(xTimer == xTimer1)
    {
        printf("Timer1 Callback :%d \n",xTimeNow);
    }
    
    if(xTimer == xTimer2)
    {
        printf("Timer2 Callback :%d \n",xTimeNow);
        xTimerReset(xTimer2,0);//重新唤醒该定时器
    }

}

/*回调函数的函数指针*/
void (*pxCallbackFunction)(TimerHandle_t) = prvTimer_Callback ;


int main(void)
{
    usart_init(USART_0, 115200, 1, 1);//串口初始化

    static BaseType_t err = pdPASS;
    
    /* 创建两个软件定时器，一个为周期性定时器，另一个为一次性定时器 */
    xTimer1 = xTimerCreate("Timer1", Timer_Period, pdTRUE, Timer_ID, pxCallbackFunction);
    xTimer2 = xTimerCreate("Timer2", Timer_Period_2, pdFALSE, Timer_ID_2, pxCallbackFunction);

    /* 创建定时器成功 */
    if (xTimer1 != NULL && xTimer2 != NULL)
    {
        /* 启动定时器,向定时器队列发送，守护队列接收 */
        err = xTimerStart(xTimer1, 0);
        if(err != pdPASS)
            printf("Timer start Failed\n");
        err = xTimerStart(xTimer2, 0);
        if(err != pdPASS)
            printf("Timer start Failed\n");

    }
    else
    {
        printf("Timer Create Failed\n");
    }

    /* 开始调度，但软件定时器的启动会在调度后才会启动 */
    vTaskStartScheduler();	//开始调度

	while (1)
	{
        //不会执行到这里
	}
    /*此处有意警告，不必理会*/
	return 0;
}



