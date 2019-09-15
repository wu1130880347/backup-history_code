#ifndef _PID_H_
#define _PID_H_
#include "Source.h"
struct PID {
        unsigned int Target_data; // 设定目标 Desired Value
        unsigned int Proportion; // 比例常数 Proportional Const
        unsigned int Integral; // 积分常数 Integral Const
        unsigned int Derivative; // 微分常数 Derivative Const
                 int LastError; // Error[-1]
                 int PrevError; // Error[-2]
                 int SumError; // Sums of Errors
        };

static struct PID spid; // PID Control Structure

int PIDCalc(unsigned int NextPoint);
unsigned int get_data(void);
void PIDInit(void);
void compare_data();
void set_pid(unsigned int P,unsigned int I,unsigned int D,unsigned int T);

#endif