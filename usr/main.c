/**
 * @Descripttion : freerots创建任务例程，实现使用freertos任务创建与删除
 * @version      : 
 * @Author       : Deng Jiaxiong
 * @Date         : 2024-02-16 11:33:46
 * @LastEditors  : Deng Jiaxiong and Peng GuangFeng
 * @LastEditTime : 2024-11-10
 * @homework     : 学习之后建议尝试以下功能：
 *								 1.调用API函数更改任务优先级
 *								 2.尝试不同的任务调度模式运行以及更改时间片
 *								 3.尝试使用空闲任务与空闲钩子任务
 *								 4.尝试线程本地储存
**/
#include "main.h"
#include "gd32f30x.h"
#include "led.h"
#include "usart.h"
#include "string.h"

//启动任务优先级
#define START_TASK_PRIO	    1
//启动任务堆栈大小
#define START_STK_SIZE      128
//启动任务任务句柄
TaskHandle_t StartTask_Handler;
//启动任务任务函数
void start_task(void *pvParameters);

//注意：两者优先级若设置为相同，不同的任务同时操作相同的硬件可能导致debug串口数据错乱
//解决方法：可以设置不同优先级或者采用协同调度的任务调度方式（自行理解）

//led任务任务优先级
#define LED_TASK_PRIO       2
#define LED2_TASK_PRIO      1
//led任务任务堆栈
#define LED_STK_SIZE        128
//led任务任务句柄
TaskHandle_t LEDTask_Handler;
TaskHandle_t LEDTask_Handler2;
//led任务任务函数
void led_task(void *pvParameters);
void led2_task(void *pvParameters);

/* 动态创建事件组 */
EventGroupHandle_t xEventGroup;

/* 将要传入的任务参数定义为常量，不在栈中，以确保任务运行时数据保持有效 */
static const char *pcTextForTask1 = "Task 1 is running\r\n";
static const char *pcTextForTask2 = "Task 2 is running\r\n";

int main(void)
{
    int err = 1;
    
    /* 动态创建事件组 */
	xEventGroup = xEventGroupCreate();

    err  = xTaskCreate( (TaskFunction_t )start_task,
                        (const char*    )"start_task",
                        (uint16_t       )START_STK_SIZE,
                        (void*          )NULL,
                        (UBaseType_t    )START_TASK_PRIO,
                        (TaskHandle_t*  )&StartTask_Handler);
    if(err == pdFALSE)//养成良好的编程习惯，判断任务创建是否成功以更快的判断bug所在
    {
        printf("Task Create Failed\r\n");
    }
    vTaskStartScheduler();	//开始调度

	while (1)
	{
        //不会执行到这里
	}
	return 0;
}

/**
 * @brief        : 创建其他任务：创建led_task、led_1_task、flow_task和eventGroup_task任务。
 * @param         {void} *pvParameters:
 * @return        {*}
 * @author       : Deng Jiaxiong and Peng GuangFeng( 2972534518@qq.com )
**/
void start_task(void *pvParameters)
{
    int err = 1;
    taskENTER_CRITICAL();       //进入临界区
    
    leds_all_init();            //led初始化
    usart_init(USART_0, 115200, 1, 1);//串口初始化
    
    err = xTaskCreate(  (TaskFunction_t )led_task,      //创建led点亮任务
                        (const char*    )"led_task",
                        (uint16_t       )LED_STK_SIZE,
                        (void*          )pcTextForTask1,
                        (UBaseType_t    )LED_TASK_PRIO,
                        (TaskHandle_t*  )&LEDTask_Handler);
    if(err == pdFALSE)
    {
        printf("Task Create Failed\r\n");
    }

    err = xTaskCreate(  (TaskFunction_t )led2_task,      //创建led熄灭任务
                        (const char*    )"led2_task",
                        (uint16_t       )LED_STK_SIZE,
                        (void*          )pcTextForTask2,
                        (UBaseType_t    )LED2_TASK_PRIO,
                        (TaskHandle_t*  )&LEDTask_Handler2);
    if(err == pdFALSE)
    {
        printf("Task Create Failed\r\n");
    }
    taskEXIT_CRITICAL();    //退出临界区
    //注意：该任务删除后其内存会被空闲任务清除，空闲任务在其他任务阻塞时运行
    vTaskDelete(NULL);      //删除本任务，以防任务抢占CPU
}

/**
 * @brief        : led_task任务函数，用于控制核心板LED灯的闪烁。
 * @param         {void} *pvParameters:
 * @return        {*}
 * @author       : Deng Jiaxiong and Peng GuangFeng( 2972534518@qq.com )
**/
void led_task(void *pvParameters)
{
    char* str = (char *)pvParameters;
    for(;;)
    {
        if(xEventGroup != NULL){                            //如果事件组创建成功
            led_on(LED);                                    //点亮核心板led
            printf("%s\r\n", str);                    //debug发送      
            vTaskDelay(200 / portTICK_RATE_MS);             //延时200ms
        }
        else
            vTaskDelay(10 / portTICK_RATE_MS);              //延时10ms，避免事件组不存在导致任务占据cpu资源
    }
}

/**
 * @brief        : led_task任务函数，用于控制核心板LED灯的熄灭。
 * @param         {void} *pvParameters:
 * @return        {*}
 * @author       : Peng GuangFeng
**/
void led2_task(void *pvParameters)
{
    char* str = (char *)pvParameters;
    for(;;)
    {
        if(xEventGroup != NULL){
            led_off(LED);                                   //熄灭核心板led
            printf("%s\r\n", str);                           //debug发送
            vTaskDelay(200 / portTICK_RATE_MS);             //延时800ms
        }
        else
            vTaskDelay(10 / portTICK_RATE_MS);              //延时10ms，避免事件组不存在导致任务占据cpu资源
    }
}
