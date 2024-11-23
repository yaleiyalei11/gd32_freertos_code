/**
 * @Descripttion : freerots资源管理例程，守门人任务实现资源管理,本任务通过资源调度管理解决创建任务例程和队列历程的串口乱码的问题
 * @version      : 2024嵌入式组
 * @Author       : Peng Guangfeng(2304146968@qq.com)
 * @Date         : 2024-11-23
 * @LastEditors  : Peng Guangfeng(2304146968@qq.com)
 * @LastEditTime : 2024-11-23
 * @homework     : 学习之后建议尝试以下功能：
 *								 1.
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
void prvGatekeeperTask(void *pvParameters);

/* 动态创建事件组 */
EventGroupHandle_t xEventGroup;

/* 将要传入的任务参数定义为常量，不在栈中，以确保任务运行时数据保持有效 */
static const char *pcTextForTask1 = "Task 1 is running\r\n";
static const char *pcTextForTask2 = "Task 2 is running\r\n";

QueueHandle_t xQueueprint;


int main(void)
{
    leds_all_init();            //led初始化
    usart_init(USART_0, 115200, 1, 1);//串口初始化

    printf("FreeRTOS Start\r\n");

    int err = 1;    
    
    xQueueprint = xQueueCreate(5, sizeof(char *));
    if(xQueueprint == NULL)
        printf("Queue Create Failed\r\n");

    err  = xTaskCreate( (TaskFunction_t )start_task,
                        (const char*    )"start_task",
                        (uint16_t       )START_STK_SIZE,
                        (void*          )NULL,
                        (UBaseType_t    )START_TASK_PRIO,
                        (TaskHandle_t*  )&StartTask_Handler);
    if(err == pdFALSE)//养成良好的编程习惯，判断任务创建是否成功以更快的判断bug所在
        printf("Task Create Failed\r\n");

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
static void start_task(void *pvParameters)
{
    int err = 1;
    taskENTER_CRITICAL();       //进入临界区

    err = xTaskCreate(  (TaskFunction_t )led_task,      //创建led点亮任务
                        (const char*    )"led_task",
                        (uint16_t       )LED_STK_SIZE,
                        (void*          )pcTextForTask1,
                        (UBaseType_t    )LED_TASK_PRIO,
                        (TaskHandle_t*  )&LEDTask_Handler);
    if(err == pdFALSE)
        printf("Task Create Failed\r\n");

    err = xTaskCreate(  (TaskFunction_t )led2_task,      //创建led熄灭任务
                        (const char*    )"led2_task",
                        (uint16_t       )LED_STK_SIZE,
                        (void*          )pcTextForTask2,
                        (UBaseType_t    )LED2_TASK_PRIO,
                        (TaskHandle_t*  )&LEDTask_Handler2);
    if(err == pdFALSE)
        printf("Task Create Failed\r\n");

    err = xTaskCreate(  (TaskFunction_t )prvGatekeeperTask, 
                        (const char*    )"Gatekeeper", 
                        (uint16_t       )128, 
                                         NULL, 
                                         3, 
                                         NULL);
    if(err == pdFALSE)
        printf("Task Create Failed\r\n");

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
static void led_task(void *pvParameters)
{
    char *pcStringToSend1 = (char* )pvPortMalloc(sizeof("efg"));
    sprintf(pcStringToSend1, "efg");
    for(;;)
    {
        led_on(LED);                                    //点亮核心板led
        xQueueSendToBack(xQueueprint, &pcStringToSend1, 0);          //向守门人任务发送串口输出的请求
        vTaskDelay(200 / portTICK_RATE_MS);             //延时200ms
    }
}

/**
 * @brief        : led_task任务函数，用于控制核心板LED灯的熄灭。
 * @param         {void} *pvParameters:
 * @return        {*}
 * @author       : Peng GuangFeng
**/
static void led2_task(void *pvParameters)
{
    char *pcStringToSend = (char* )pvPortMalloc(sizeof("abc"));
    sprintf(pcStringToSend, "abc");
    for(;;)
    {  
        led_off(LED);                                   //熄灭核心板led
        xQueueSendToBack(xQueueprint, &pcStringToSend, 0);          //向守门人任务发送串口输出的请求
        vTaskDelay(200 / portTICK_RATE_MS);             //延时800ms
    }
}

/**
 * @brief        : 守门人任务，用于输出串口数据。
 * @param        : {void} *pvParameters:
 * @return       : {*}
 * @author       : Peng GuangFeng
**/
static void prvGatekeeperTask(void *pvParameters)
{
    char * pcMessage;
    for(;;)
    {
        xQueueReceive(xQueueprint, &pcMessage, portMAX_DELAY);
        printf("%s\r\n", pcMessage);//可以看见串口发送不再乱码，因为串口资源只有守门人任务可以访问
    }
}
