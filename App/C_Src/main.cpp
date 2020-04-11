extern "C"
{
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include <iar_dlmalloc.h>
}
#include "CNetIncludes.h"

//debug info output
#if DBGUART
extern "C"
{
#define DBG_USE 1

#if DBG_USE
    //是否打开该文件内的调试LOG
    static const char EN_LOG = BLUE;
    //LOG输出文件标记
    static const char TAG[] = "AppMain: ";
#else
#ifdef DBG_Printf
#undef DBG_Printf
#define DBG_Printf(...)
#else
#endif
#endif
}

#endif

//任务优先级
#define START_TASK_PRIO 1
//任务堆栈大小
#define START_STK_SIZE 128
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//任务优先级
#define LED0_TASK_PRIO 2
//任务堆栈大小
#define LED0_STK_SIZE 50
//任务句柄
TaskHandle_t LED0Task_Handler;
//任务函数
void led0_task(void *pvParameters);

//任务优先级
#define LED1_TASK_PRIO 3
//任务堆栈大小
#define LED1_STK_SIZE 50
//任务句柄
TaskHandle_t LED1Task_Handler;
//任务函数
void led1_task(void *pvParameters);

//任务优先级
#define FLOAT_TASK_PRIO 4
//任务堆栈大小
#define FLOAT_STK_SIZE (512 * 3)
//任务句柄
TaskHandle_t FLOATTask_Handler;
//任务函数
void float_task(void *pvParameters);

//任务优先级
#define CONNECT_TASK_PRIO 5
//任务堆栈大小
#define CONNECT_STK_SIZE (512 * 3)
//任务句柄
TaskHandle_t CONNECTTask_Handler;
//任务函数
void connect_task(void *pvParameters);

void main(void)
{
    Driver_MCU_Init();
    delay_init(168);
    uart_init(921600);
    Dprintf(EN_LOG,TAG,"app start--\r\n");
    LED_Init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //设置系统中断优先级分组4
    //创建开始任务

    xTaskCreate((TaskFunction_t)start_task,          //任务函数
                (const char *)"start_task",          //任务名称
                (uint16_t)START_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)START_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&StartTask_Handler); //任务句柄
    vTaskStartScheduler();                           //开启任务调度
    while (1)
        ;
    // while(1)
    // {
    //     delay_ms(1000);
    //     LED0 ^= 1;
    // }
}
//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL(); //进入临界区
    //创建LED0任务
    xTaskCreate((TaskFunction_t)led0_task,
                (const char *)"led0_task",
                (uint16_t)LED0_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)LED0_TASK_PRIO,
                (TaskHandle_t *)&LED0Task_Handler);
    //创建LED1任务
    xTaskCreate((TaskFunction_t)led1_task,
                (const char *)"led1_task",
                (uint16_t)LED1_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)LED1_TASK_PRIO,
                (TaskHandle_t *)&LED1Task_Handler);
    //浮点测试任务
    xTaskCreate((TaskFunction_t)float_task,
                (const char *)"float_task",
                (uint16_t)FLOAT_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)FLOAT_TASK_PRIO,
                (TaskHandle_t *)&FLOATTask_Handler);
    //初始化网络任务
    xTaskCreate((TaskFunction_t)connect_task,
                (const char *)"connect_task",
                (uint16_t)CONNECT_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)CONNECT_TASK_PRIO,
                (TaskHandle_t *)&CONNECTTask_Handler);
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

//LED0任务函数
void led0_task(void *pvParameters)
{
    Dprintf(EN_LOG,TAG,"led0_task start--\r\n");
    while (1)
    {
        LED0 = ~LED0;
        vTaskDelay(500);
    }
}

//LED1任务函数
void led1_task(void *pvParameters)
{
    Dprintf(EN_LOG,TAG,"led1_task start--\r\n");
    while (1)
    {
        // LED1=0;
        vTaskDelay(200);
        // LED1=1;
        vTaskDelay(800);
    }
}
static uint8_t uart_app_rxbuf[512];
static uint8_t uart_app_txbuf[512];
static uint8_t uart_drv_rxbuf[512];
static uint8_t uart_drv_txbuf[512];

static NetDrvUartStruct_t uart_com =
    {
        0x01,  //普通串口
        0x02,  //串口2
        921600, //波特率
        uart_drv_rxbuf,
        512,
        uart_drv_txbuf,
        512
    };
NetParaType_t para =
    {
        NET_UART,
        APP_UART,
        {
            512, //rx max buffer
            uart_app_rxbuf,
            512, //tx max buffer
            uart_app_txbuf,
        },
        {
            (void *)&uart_com,
        }
    };
//浮点测试任务
void float_task(void *pvParameters)
{
    static float float_num = 0.00;
    Dprintf(EN_LOG,TAG,"float_task start--\r\n");
    while (1)
    {
        float_num += 0.01f;
        //printf("float_num的值为: %.4f\r\n",float_num);
        vTaskDelay(1000);
        //CUartProtocol::get_this((void*)&para)->HeartBeatSend();
        //__iar_dlmalloc_stats();
    }
}

//连接通信任务
void connect_task(void *pvParameters)
{
    Dprintf(EN_LOG,TAG,"connect_task start--\r\n");
    CNetManagement *g_Uart_To_Udp;
    g_Uart_To_Udp = new CNetManagement(&para, new CUartProtocol(), new IUarts());
    while (1)
    {
        g_Uart_To_Udp->Agreement();
        vTaskDelay(10);
    }
}
