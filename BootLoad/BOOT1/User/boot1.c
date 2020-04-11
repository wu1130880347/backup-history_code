#include "boot1.h"
#include "usart.h"
#include "sys.h"
//debug info output
#if ((defined USART_DEBUG) && (USART_DEBUG == __ON__))
//�Ƿ�򿪸��ļ��ڵĵ���LOG
static const char EN_LOG = BLUE;
//LOG����ļ����
static const char TAG[] = "boot1[boot1]: ";
#endif
typedef void (*pFunction)(void);
pFunction Jump_To_Application;

void Jump_To_App(void);

void Jump_To_App(void)
{
    Dprintf(EN_LOG, TAG, "Jump Address = 0x%8x\r\n", (__IO uint32_t *)IAPSTART);
    Jump_To_Application = (pFunction)(*(__IO uint32_t *)(IAPSTART + 4));
    __set_MSP(*(__IO uint32_t *)IAPSTART);
    Jump_To_Application();
}