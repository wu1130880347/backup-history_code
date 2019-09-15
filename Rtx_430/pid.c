#include "pid.h"

unsigned int in_data;
unsigned int get_data(void)
{
  in_data = 100;
  return in_data;
}

        /*====================================================================================================
        Initialize PID Structure
        =====================================================================================================*/
void PIDInit(void)
{
        spid.Proportion = 0; // Set PID Coefficients  比例常数 Proportional Const
        spid.Integral = 0;    //积分常数 Integral Const
        spid.Derivative = 0;   //微分常数 Derivative Const
        spid.Target_data = 0; // Set PID Setpoint 设定目标 Desired Value
        spid.SumError = 0;
        spid.LastError = 0;
        spid.PrevError = 0;
}
        /*====================================================================================================
        PID计算部分
        =====================================================================================================*/
int PIDCalc(unsigned int NextPoint )
{
        int dError,Error;
        Error = spid.Target_data - NextPoint;          // 偏差
        
        spid.SumError += Error;                     // 积分                                   
        dError = spid.LastError - spid.PrevError;    // 当前微分  
        spid.PrevError = spid.LastError;                           
        spid.LastError = Error; 
         
        return (spid.Proportion * Error            // 比例项           
        + spid.Integral * spid.SumError              // 积分项
        + spid.Derivative * dError);                // 微分项
}

void compare_data()
{
  in_data = get_data();
  PIDCalc(in_data);
  //work 控制器
  //与target_data比较
}


void set_pid(unsigned int P,unsigned int I,unsigned int D,unsigned int T)
{
        spid.Proportion = P; // Set PID Coefficients  比例常数 Proportional Const
        spid.Integral = I;    //积分常数 Integral Const
        spid.Derivative = D;   //微分常数 Derivative Const
        spid.Target_data = T; // Set PID Setpoint 设定目标 Desired Value
}