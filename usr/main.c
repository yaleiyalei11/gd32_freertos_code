/**
 * @Descripttion : freertos硬件中断的使用，与计数信号量联动
 * @version      : 2024嵌入式组
 * @Author       : Peng GuangFeng( 2304146968@qq.com )
 * @Date         : 2024-11-18
 * @LastEditors  : Peng GuangFeng
 * @LastEditTime : 2024-11-18
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
#define START_TASK_PRIO	    1
//推迟处理任务堆栈大小
#define START_STK_SIZE      128
//推迟处理任务句柄
TaskHandle_t StartTask_Handler;
//推迟处理任务函数
void start_task(void *pvParameters);

/*信号量的句柄*/
SemaphoreHandle_t xSemaphore;


int main(void)
{
    usart_init(USART_0, 115200, 1, 1);//串口初始化
    //定时器初始化，时基单元频率为120MHz，初始化为1hz时钟
    my_timer_init(TIM1, 1999, 59999, 1, 1);

    static BaseType_t err = pdFALSE;

    /*注意：使用计数信号量时必须将FreeRTOSConfig.h中定义的configUSE_COUNTING_SEMAPHORE宏设置为1,前面的例程未改，此例已更改*/

    /* 创建一个计数信号量，最大值为10，初始值为0 */
    xSemaphore = xSemaphoreCreateCounting(10,0);
    if(xSemaphore == NULL)
    {
        printf("xSemaphoreCreateCounting failed\r\n");
    }

    err  = xTaskCreate( (TaskFunction_t )start_task,
                        (const char*    )"start_task",
                        (uint16_t       )START_STK_SIZE,
                        (void*          )NULL,
                        (UBaseType_t    )START_TASK_PRIO,
                        (TaskHandle_t*  )&StartTask_Handler);
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

void start_task(void *pvParameters)
{
    for(;;)
    {
        if(xSemaphoreTake(xSemaphore,portMAX_DELAY) == pdTRUE)
        {
            /*此处为信号处理处*/
            printf("xSemaphoreTake success\r\n");
        }
    }
}



