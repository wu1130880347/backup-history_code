
#include "Task.h"
#include "Source.h"
/*******************************************************************/

void task_test1(void)      /* 测试任务1 */
{

     while(1)
     {
       P2DIR = 0xff;
      
       while(1)
       {
         P2OUT = 0xf2;
       }
        
        //os_wait(K_SIG,0);
     };
}

void task_test2(void)      /* 测试任务2 */
{


     while(1)
     {
       P2DIR = 0xff;

       while(1){
       
         P2OUT = 0xf1;
       }
        
        //os_delete_task(2);
     };
}

void task_test3(void)      /* 测试任务3 */
{


     while(1)
     {
       P2DIR = 0xff;

       while(1)
       {
         P2OUT = 0xf4;

       }
       
     };
}