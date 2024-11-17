/**
 * @Descripttion : freerots创建队列例程，实现使用freertos队列通信
 * @version      : 2024嵌入式组版
 * @Author       : Peng GuangFeng（2304146968@qq.com)
 * @Date         : 2024-11-11
 * @LastEditors  : Peng GuangFeng（2304146968@qq.com)
 * @LastEditTime : 2024-11-11  
**/

//注意：队列集对比单一队列的设计更加繁琐且低效，无必要情况不需要使用队列集，队列集写法请自行探索

#include "main.h"
#include "gd32f30x.h"
#include "led.h"
#include "usart.h"
#include "string.h"

//启动任务优先级
#define START_TASK_PRIO	    1
//启动任务堆栈大小
#define START_STK_SIZE      128
//启动任务句柄
TaskHandle_t StartTask_Handler;
//启动任务函数
void start_task(void *pvParameters);

//注意：两者优先级若设置为相同，不同的任务同时操作相同的硬件可能导致debug串口数据错乱
//解决方法：可以设置不同优先级或者采用协同调度的任务调度方式（自行理解）

//任务优先级，本例接收优先级高于发送优先级，因此发送无需阻塞
#define LED_TASK_PRIO       1
#define LED2_TASK_PRIO      1
#define Rev_TASK_PRIO       2
//任务堆栈
#define LED_STK_SIZE        128
//任务句柄
TaskHandle_t LEDTask_Handler;
TaskHandle_t LEDTask_Handler2;
TaskHandle_t RevTask_Handler3;
//任务函数,同时用于向队列传输数据和接收数据
void led_task(void *pvParameters);
void led2_task(void *pvParameters);
void rev_task(void *pvParameters);

/* 队列句柄，本例演示两种队列的传输方式 */
QueueHandle_t xQueue1;//复制实现队列
QueueHandle_t xQueue2;//引用实现队列

//注意：正常情况下建议使用复制实现队列，原因自行搜索

/* 动态创建事件组 */
EventGroupHandle_t xEventGroup;

/* 将要传入的任务参数定义为常量，不在栈中，以确保任务运行时数据保持有效 */
static const char *pcTextForTask1 = "Task 1 is running\r\n";
static const char *pcTextForTask2 = "Task 2 is running\r\n";
static const char *pcTextForTask3 = "Task 3 is running\r\n";

/* 定义一个枚举变量，用于识别数据来源 */
typedef enum 
{
    eSender1,
    eSender2
}DataSource_t;

/* 定义队列将要传输的结构体类型 */
typedef struct
{
    DataSource_t eDataSource;
    int iValue;
}Data_t;

/* 声明两个Data_t结构体实例，用于演示在队列中的传输 */
static const Data_t xStructsToSend[2] =
{
    {eSender1, 1},
    {eSender2, 2}
};


int main(void)
{
    usart_init(USART_0, 115200, 1, 1);//串口初始化

    int err = 1;

    /* 创建队列 */
    xQueue1 = xQueueCreate(5, sizeof(Data_t));
    if(xQueue1 == NULL)
        printf("Queue Create Failed\r\n");

    xQueue2 = xQueueCreate(5, sizeof(Data_t*));
    if(xQueue2 == NULL)
        printf("Queue Create Failed\r\n");

    /* 动态创建事件组 */
	xEventGroup = xEventGroupCreate();

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
    //此处会有一个警告，无需理会，因为不会运行到此处
	return 0;
}

/**
 * @brief        : 创建其他任务：创建led_task、led_1_task、flow_task和eventGroup_task任务。
 * @param         {void} *pvParameters:
 * @return        {*}
 * @author       : Peng GuangFeng
**/
void start_task(void *pvParameters)
{
    int err = 1;
    taskENTER_CRITICAL();       //进入临界区
    
    leds_all_init();            //led初始化
    
    
    err = xTaskCreate(  (TaskFunction_t )led_task,      //创建复制队列发送任务
                        (const char*    )"led_task",
                        (uint16_t       )LED_STK_SIZE,
                        (void*          )pcTextForTask1,
                        (UBaseType_t    )LED_TASK_PRIO,
                        (TaskHandle_t*  )&LEDTask_Handler);
    if(err == pdFALSE)
        printf("Task Create Failed\r\n");

    err = xTaskCreate(  (TaskFunction_t )led2_task,      //创建指针队列发送任务
                        (const char*    )"led2_task",
                        (uint16_t       )LED_STK_SIZE,
                        (void*          )pcTextForTask2,
                        (UBaseType_t    )LED2_TASK_PRIO,
                        (TaskHandle_t*  )&LEDTask_Handler2);
    if(err == pdFALSE)
        printf("Task Create Failed\r\n");

    err = xTaskCreate(  (TaskFunction_t )rev_task,      //创建rev_task任务
                        (const char*    )"rev_task",
                        (uint16_t       )LED_STK_SIZE,
                        (void*          )pcTextForTask3,
                        (UBaseType_t    )LED2_TASK_PRIO,
                        (TaskHandle_t*  )&RevTask_Handler3);
    if(err == pdFALSE)
        printf("Task Create Failed\r\n");

    taskEXIT_CRITICAL();    //退出临界区
    //注意：该任务删除后其内存会被空闲任务清除，空闲任务在其他任务阻塞时运行
    vTaskDelete(NULL);      //删除本任务，以防任务抢占CPU
}

/**
 * @brief        : 队列1发送任务函数，采用复制队列的发送方式。
 * @param         {void} *pvParameters:
 * @return        {*}
 * @author       : Peng GuangFeng
**/
void led_task(void *pvParameters)
{
    int err;
    char* str = (char *)pvParameters;
    for(;;)
    {
        if(xEventGroup != NULL){                            //如果事件组创建成功
            led_on(LED);                                    //点亮核心板led
            printf("%s\r\n", str);                          //debug发送

            err = xQueueSendToBack(xQueue1, &xStructsToSend[0], 0);     //将第一个数据以复制队列的形式发送到队列,采用不阻塞的模式
            if(err == pdPASS)
                printf("Queue1 Send \r\n");

            //vTaskDelay(200 / portTICK_RATE_MS);             //无需延时
        }
        else
            vTaskDelay(10 / portTICK_RATE_MS);              //延时10ms，避免事件组不存在导致任务占据cpu资源
    }
}

/**
 * @brief        : 队列2发送任务函数，采用指针队列发送方式。
 * @param         {void} *pvParameters:
 * @return        {*}
 * @author       : Peng GuangFeng
**/
void led2_task(void *pvParameters)
{
    int err;
    //采用指针发送方式需要先开辟一块内存用于存储发送的数据，以防指针发送出现错误
    const Data_t *pcStringToSend = (Data_t *)pvPortMalloc(sizeof(Data_t));
    pcStringToSend = &xStructsToSend[1]; 

    char* str = (char *)pvParameters;
    for(;;)
    {
        if(xEventGroup != NULL){
            led_off(LED);                                   //熄灭核心板led
            printf("%s\r\n", str);                           //debug发送

            err = xQueueSendToBack(xQueue2, &pcStringToSend, 0);
            if(err == pdPASS)
                printf("Queue2 Send \r\n");
            //vTaskDelay(200 / portTICK_RATE_MS);             //无需延时
        }
        else
            vTaskDelay(10 / portTICK_RATE_MS);              //延时10ms，避免事件组不存在导致任务占据cpu资源
    }
}

/**
 * @brief        : 接收任务函数。
 * @param         {void} *pvParameters:
 * @return        {*}
 * @author       : Peng GuangFeng
**/
void rev_task(void *pvParameters)
{
    int err;
    Data_t  xReceivedData1;
    Data_t *xReceivedData2;
    
    for(;;)
    {   
        //串口输出有重叠为正常现象，因为多个任务同时操作硬件导致的资源占用，不影响真正的数据接收，将队列传输判断后的串口输出注释掉即为正常
        //或者将判断成功改为判断失败，请读者自行尝试
        if(xQueue1 != NULL){
            err = xQueueReceive(xQueue1, &xReceivedData1,(200 / portTICK_RATE_MS)); //阻塞接收
            if(err == pdPASS)
                printf("Queue1 Receive\r\n Data: %d \r\n", xReceivedData1.iValue);
        }
        if(xQueue2 != NULL){
            err = xQueueReceive(xQueue2, &xReceivedData2,(200 / portTICK_RATE_MS)); //阻塞接收
            if(err == pdPASS)
                printf("Queue2 Receive\r\n Data: %d \r\n" , xReceivedData2->iValue);
            vPortFree(xReceivedData2);//用完之后释放
        }

    }
}
