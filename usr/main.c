/**
 * @Descripttion : freertos任务通知，本例演示如何在串口传输函数中使用任务通知
 * @version      : 2024嵌入式组
 * @Author       : Peng GuangFeng( 2304146968@qq.com )
 * @Date         : 2024-12-3
 * @LastEditors  : Peng GuangFeng
 * @LastEditTime : 2024-12-3
 * @homework     : 学习之后建议尝试以下功能：
 *								 1.尝试推迟到守护进程中处理，并分析其优劣
 *								 2.尝试用中断接收信号量处理
**/
#include "main.h"
#include "gd32f30x.h"
#include "led.h"
#include "usart.h"
#include "timer.h"

//推迟处理任务优先级
#define UART_TASK_PRIO	    1
//推迟处理任务堆栈大小
#define UART_STK_SIZE      128
//推迟处理任务句柄
TaskHandle_t UartTask_Handler;
//推迟处理任务函数
void Uart_task(void *pvParameters);


int main(void)
{
    usart_init(USART_0, 115200, 1, 1);//串口初始化
    //定时器初始化，时基单元频率为120MHz，初始化为1hz时钟
    my_timer_init(TIM1, 1999, 59999, 1, 1);

    static BaseType_t err = pdFALSE;


    err  = xTaskCreate( (TaskFunction_t )Uart_task,
                        (const char*    )"start_task",
                        (uint16_t       )UART_STK_SIZE,
                        (void*          )NULL,
                        (UBaseType_t    )UART_TASK_PRIO,
                        (TaskHandle_t*  )&UartTask_Handler);
    if(err != pdPASS)
        printf("start_task create failed\r\n");


    /* 开始调度，但软件定时器的启动会在调度后才会启动 */
    vTaskStartScheduler();	//开始调度

	while (1)
	{
        //不会执行到这里
	}
    /*此处有意警告，不必理会*/
	return 0;
}

void Uart_task(void *pvParameters)
{
    const TickType_t xMaxExpectedBlockTime = pdMS_TO_TICKS(510);
    for(;;)
    {
        if(ulTaskNotifyTake(pdFALSE , xMaxExpectedBlockTime) != 0 ){
            printf("uart task notify\r\n");
        }
        else{
            printf("uart task timeout\r\n");
        }
    }
}



