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
    //�Ƿ�򿪸��ļ��ڵĵ���LOG
    static const char EN_LOG = BLUE;
    //LOG����ļ����
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

//�������ȼ�
#define START_TASK_PRIO 1
//�����ջ��С
#define START_STK_SIZE 128
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);

//�������ȼ�
#define LED0_TASK_PRIO 2
//�����ջ��С
#define LED0_STK_SIZE 50
//������
TaskHandle_t LED0Task_Handler;
//������
void led0_task(void *pvParameters);

//�������ȼ�
#define LED1_TASK_PRIO 3
//�����ջ��С
#define LED1_STK_SIZE 50
//������
TaskHandle_t LED1Task_Handler;
//������
void led1_task(void *pvParameters);

//�������ȼ�
#define FLOAT_TASK_PRIO 4
//�����ջ��С
#define FLOAT_STK_SIZE (512 * 3)
//������
TaskHandle_t FLOATTask_Handler;
//������
void float_task(void *pvParameters);

//�������ȼ�
#define CONNECT_TASK_PRIO 5
//�����ջ��С
#define CONNECT_STK_SIZE (512 * 3)
//������
TaskHandle_t CONNECTTask_Handler;
//������
void connect_task(void *pvParameters);

void main(void)
{
    Driver_MCU_Init();
    delay_init(168);
    uart_init(921600);
    Dprintf(EN_LOG,TAG,"app start--\r\n");
    LED_Init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //����ϵͳ�ж����ȼ�����4
    //������ʼ����

    xTaskCreate((TaskFunction_t)start_task,          //������
                (const char *)"start_task",          //��������
                (uint16_t)START_STK_SIZE,            //�����ջ��С
                (void *)NULL,                        //���ݸ��������Ĳ���
                (UBaseType_t)START_TASK_PRIO,        //�������ȼ�
                (TaskHandle_t *)&StartTask_Handler); //������
    vTaskStartScheduler();                           //�����������
    while (1)
        ;
    // while(1)
    // {
    //     delay_ms(1000);
    //     LED0 ^= 1;
    // }
}
//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL(); //�����ٽ���
    //����LED0����
    xTaskCreate((TaskFunction_t)led0_task,
                (const char *)"led0_task",
                (uint16_t)LED0_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)LED0_TASK_PRIO,
                (TaskHandle_t *)&LED0Task_Handler);
    //����LED1����
    xTaskCreate((TaskFunction_t)led1_task,
                (const char *)"led1_task",
                (uint16_t)LED1_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)LED1_TASK_PRIO,
                (TaskHandle_t *)&LED1Task_Handler);
    //�����������
    xTaskCreate((TaskFunction_t)float_task,
                (const char *)"float_task",
                (uint16_t)FLOAT_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)FLOAT_TASK_PRIO,
                (TaskHandle_t *)&FLOATTask_Handler);
    //��ʼ����������
    xTaskCreate((TaskFunction_t)connect_task,
                (const char *)"connect_task",
                (uint16_t)CONNECT_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)CONNECT_TASK_PRIO,
                (TaskHandle_t *)&CONNECTTask_Handler);
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

//LED0������
void led0_task(void *pvParameters)
{
    Dprintf(EN_LOG,TAG,"led0_task start--\r\n");
    while (1)
    {
        LED0 = ~LED0;
        vTaskDelay(500);
    }
}

//LED1������
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
        0x01,  //��ͨ����
        0x02,  //����2
        921600, //������
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
//�����������
void float_task(void *pvParameters)
{
    static float float_num = 0.00;
    Dprintf(EN_LOG,TAG,"float_task start--\r\n");
    while (1)
    {
        float_num += 0.01f;
        //printf("float_num��ֵΪ: %.4f\r\n",float_num);
        vTaskDelay(1000);
        //CUartProtocol::get_this((void*)&para)->HeartBeatSend();
        //__iar_dlmalloc_stats();
    }
}

//����ͨ������
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
