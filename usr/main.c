/**
 * @Descripttion : freerots事件组例程，利用事件组实现三个任务的同步
 * @version      : 2024嵌入式组
 * @Author       : Peng Guangfeng and Deng Jiaxiong
 * @Date         : 2024-11-23 
 * @LastEditors  : Peng Guangfeng
 * @LastEditTime : 2024-11-23
**/
#include "main.h"
#include "gd32f30x.h"
#include "led.h"

//启动任务优先级
#define START_TASK_PRIO	    1
//启动任务堆栈大小
#define START_STK_SIZE      128
//启动任务任务句柄
TaskHandle_t StartTask_Handler;
//启动任务任务函数
void start_task(void *pvParameters);

//led任务任务优先级
#define LED_TASK_PRIO       1
//led任务任务堆栈
#define LED_STK_SIZE        128
//led任务任务句柄
TaskHandle_t LEDTask_Handler;
//led任务任务函数
void led_task(void *pvParameters);

//led1任务任务优先级
#define LED_1_TASK_PRIO       1
//led1任务任务堆栈
#define LED_1_STK_SIZE        128
//led1任务任务句柄
TaskHandle_t LED_1Task_Handler;
//led1任务任务函数
void led_1_task(void *pvParameters);

//流水灯任务任务优先级
#define FLOW_TASK_PRIO       1
//流水灯任务任务堆栈
#define FLOW_STK_SIZE        128
//流水灯任务任务句柄
TaskHandle_t FLOWTask_Handler;
//流水灯任务任务函数
void flow_task(void *pvParameters);

//事件组任务任务优先级
#define EVENT_GROUP_TASK_PRIO       2
//事件组任务任务堆栈
#define EVENT_GROUP_STK_SIZE        128
//事件组任务任务句柄
TaskHandle_t EventGroupTask_Handler;
//事件组任务任务函数
void eventGroup_task(void *pvParameters);

/* 定义事件位的意义 */
#define LED_TASK_BIT    ( 1UL << 0UL )
#define LED_1_TASK_BIT  ( 1UL << 1UL )
#define FLOW_TASK_BIT   ( 1UL << 2UL )

/* 动态创建事件组 */
EventGroupHandle_t xEventGroup;

int main(void)
{
    /* 动态创建事件组 */
	xEventGroup = xEventGroupCreate();

    xTaskCreate((TaskFunction_t )start_task,
                (const char*    )"start_task",
                (uint16_t       )START_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )START_TASK_PRIO,
                (TaskHandle_t*  )&StartTask_Handler);
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
 * @author       : Deng Jiaxiong( 2972534518@qq.com )
**/
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();       //进入临界区
    
    leds_all_init();            //led初始化
    
    xTaskCreate((TaskFunction_t )led_task,
                (const char*    )"led_task",
                (uint16_t       )LED_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_TASK_PRIO,
                (TaskHandle_t*  )&LEDTask_Handler);
    xTaskCreate((TaskFunction_t )led_1_task,
                (const char*    )"led_1_task",
                (uint16_t       )LED_1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_1_TASK_PRIO,
                (TaskHandle_t*  )&LED_1Task_Handler);
    xTaskCreate((TaskFunction_t )flow_task,
                (const char*    )"flow_task",
                (uint16_t       )FLOW_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )FLOW_TASK_PRIO,
                (TaskHandle_t*  )&FLOWTask_Handler);
    xTaskCreate((TaskFunction_t )eventGroup_task,
                (const char*    )"enentGroup_task",
                (uint16_t       )EVENT_GROUP_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )EVENT_GROUP_TASK_PRIO,
                (TaskHandle_t*  )&EventGroupTask_Handler);
                
    taskEXIT_CRITICAL();    //退出临界区
    vTaskDelete(NULL);      //删除本任务
}

/**
 * @brief        : led_task任务函数，用于控制核心板LED灯的闪烁。
 * @param         {void} *pvParameters:
 * @return        {*}
 * @author       : Deng Jiaxiong( 2972534518@qq.com )
**/
void led_task(void *pvParameters)
{
    for(;;)
    {
        if(xEventGroup != NULL){                            //如果事件组创建成功
            led_on(LED);                                    //点亮核心板led
            xEventGroupSetBits(xEventGroup,LED_TASK_BIT);   //事件组LED_TASK_BIT位置一
            vTaskDelay(200 / portTICK_RATE_MS);             //延时200ms
            led_off(LED);                                   //熄灭核心板led
            vTaskDelay(800 / portTICK_RATE_MS);             //延时800ms
        }
        else
            vTaskDelay(10 / portTICK_RATE_MS);              //延时10ms，避免事件组不存在导致任务占据cpu资源
    }
}

/**
 * @brief        : led_1_task任务的实现，用于控制LED1的闪烁。
 * @param         {void} *pvParameters:
 * @return        {*}
 * @author       : Deng Jiaxiong( 2972534518@qq.com )
**/
void led_1_task(void *pvParameters)
{
    for(;;)
    {
        if(xEventGroup != NULL){                                //如果事件组创建成功
            led_on(LED1);                                       //点亮led1
            xEventGroupSetBits(xEventGroup, LED_1_TASK_BIT);    //事件组LED_1_TASK_BIT位置一
            vTaskDelay(100 / portTICK_RATE_MS);                 //延时100ms
            led_off(LED1);                                      //熄灭led1
            vTaskDelay(1300 / portTICK_RATE_MS);                //延时1300ms
        }
        else
            vTaskDelay(10 / portTICK_RATE_MS);                  //延时10ms，避免事件组不存在导致任务占据cpu资源
    }
}

/**
 * @brief        : flow_task任务的实现，用于控制LED灯的流水效果。
 * @param         {void} *pvParameters:
 * @return        {*}
 * @author       : Deng Jiaxiong( 2972534518@qq.com )
**/
void flow_task(void *pvParameters)
{
    for(uint8_t cnt = 0; cnt < 7; cnt++)
    {
        led_flow_task(); 
        vTaskDelay(100 / portTICK_RATE_MS);
    }
    xEventGroupSetBits(xEventGroup, FLOW_TASK_BIT);
    vTaskDelete(NULL);  //执行一次后删除任务
}

/**
 * @brief        : eventGroup_task任务的实现，用于监听其他任务的事件标志位，并根据事件标志位控制LED2灯的闪烁。
 * @param         {void} *pvParameters:
 * @return        {*}
 * @author       : Deng Jiaxiong( 2972534518@qq.com )
**/
void eventGroup_task(void *pvParameters)
{
    for(;;)
    {
        if(xEventGroup != NULL){
            /*等待事件标志*/
            xEventGroupWaitBits((EventGroupHandle_t	)xEventGroup,
                               (EventBits_t			)LED_TASK_BIT | LED_1_TASK_BIT | FLOW_TASK_BIT,//等待三个事件标志位
                               (BaseType_t			)pdTRUE,    //执行后清除标志位
                               (BaseType_t			)pdFALSE,   //或条件执行
                               (TickType_t			)portMAX_DELAY);
            /*led2点亮10ms后熄灭*/
            led_on(LED2);
            vTaskDelay(10 / portTICK_RATE_MS);
            led_off(LED2);
        }
        else
            vTaskDelay( 10 / portTICK_RATE_MS); //如果事件标志创建不成功释放资源
    }
}
