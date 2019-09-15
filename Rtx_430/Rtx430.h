#ifndef __RTX430_H_
#define __RTX430_H_

/*************************************************************************************/
/** Author:linger                                                                   **/
/** Email:ling_re@sina.com                                                          **/
/** This file is part of the 'RTX-430' Real-Time Operating System Source Package    **/
/*************************************************************************************/
/**                                                                                 **/
/** 千万注意：                                                                      **/
/**              在中断程序开始处一定要调用 " RTX_TS|=RTX_TS_DELAY; " 这一语句      **/
/**                                                                                 **/
/*************************************************************************************/

#include  "msp430x14x.h"
#include  "Task.h"

#define  TIMESHARING        5        /* 每个任务的最大运行时间       */
#define  RTX_STACKFREE      40       /* 当前任务的最小堆栈空间       */
#define  RTX_MAXTASKN       10       /* 最大任务数                   */
#define  INT_CLOCK          500    /* 每个定时中断的时钟数         */
#define  RAMBOTTOM          0x300    /* 最大 RAM 数                  */
#define  RAMTOP             0x9FF    /* 最大 RAM 数                  */

#define  RTX_TIMESHARING    (TIMESHARING)
#define  RTX_CLOCK          INT_CLOCK
#define  RTX_RAMBOTTOM      RAMBOTTOM/2*2               
#define  RTX_RAMTOP         RAMTOP/2*2              

#define  K_SIG              1
#define  K_TMO              2
#define  SIG_EVENT          4
#define  TMO_EVENT          8
#define  K_READY            16
#define  K_ACTIVE           32
#define  K_ROBIN            64
#define  K_IVL              128

#define  B_WAITSIG          0
#define  B_WAITTIM          1
#define  B_SIGNAL           2
#define  B_TIMEOUT          3
#define  B_READY            4
#define  B_ACTIVE           5
#define  B_ROBIN            6
#define  B_INTERVAL         7

#define  RTX_TS_REQ         1
#define  RTX_TS_DELAY       2         /* 进入中断处理程序后，要将 RTX_TS 中的该位置为 1 */


void Rtx430_Init(void);
void timer0_comm(void);
void clear_watchdog(void);
void os_system_init(void);
unsigned char task_switch(void);
unsigned char os_system_start(void);
unsigned char os_delete_task(unsigned char task_no);
unsigned char os_send_signal(unsigned char task_no);
unsigned char isr_send_signal(unsigned char task_no);
unsigned char os_clear_signal(unsigned char task_no);
unsigned char os_wait(unsigned type, unsigned timeout);
unsigned char os_create_task(unsigned int proc_name, unsigned char task_no);

extern int  getsp(void);              // Get Stack Pointer
extern void putsp(int sp_value);      // Get Stack Pointer

extern void save_pop(void);
extern void save_push(void);
extern void restore_sr(void);
#endif