#ifndef _PID_H_
#define _PID_H_
//#include "sys.h"
struct PID {
        unsigned int Target_data; // �趨Ŀ�� Desired Value
        unsigned int Proportion; // �������� Proportional Const
        unsigned int Integral; // ���ֳ��� Integral Const
        unsigned int Derivative; // ΢�ֳ��� Derivative Const
                 int LastError; // Error[-1]
                 int PrevError; // Error[-2]
                 int SumError; // Sums of Errors
        };

struct PID spid; // PID Control Structure

int PIDCalc( struct PID *pp,unsigned int NextPoint );
unsigned int get_data(void);
void PIDInit(struct PID *pp);
void compare_data();
void set_pid(unsigned int P,unsigned int I,unsigned int D,unsigned int T);

#endif