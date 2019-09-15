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
void PIDInit(struct PID *pp)
{
        spid.Proportion = 0; // Set PID Coefficients  �������� Proportional Const
        spid.Integral = 0;    //���ֳ��� Integral Const
        spid.Derivative = 0;   //΢�ֳ��� Derivative Const
        spid.Target_data = 0; // Set PID Setpoint �趨Ŀ�� Desired Value
        spid.SumError = 0;
        spid.LastError = 0;
        spid.PrevError = 0;
}
        /*====================================================================================================
        PID���㲿��
        =====================================================================================================*/
int PIDCalc( struct PID *pp, unsigned int NextPoint )
{
        int dError,Error;
        Error = pp->Target_data - NextPoint;          // ƫ��           
        pp->SumError += Error;                     // ����                                   
        dError = pp->LastError - pp->PrevError;    // ��ǰ΢��  
        pp->PrevError = pp->LastError;                           
        pp->LastError = Error;                                        
        return (pp->Proportion * Error             // ������           
        + pp->Integral * pp->SumError              // ������
        + pp->Derivative * dError);                // ΢����
}

void compare_data()
{
  in_data = get_data();
  PIDCalc(&spid,in_data);
  //work ������
  //��target_data�Ƚ�
}


void set_pid(unsigned int P,unsigned int I,unsigned int D,unsigned int T)
{
        PIDInit ( &spid );    // Initialize Structure
        spid.Proportion = P; // Set PID Coefficients  �������� Proportional Const
        spid.Integral = I;    //���ֳ��� Integral Const
        spid.Derivative = D;   //΢�ֳ��� Derivative Const
        spid.Target_data = T; // Set PID Setpoint �趨Ŀ�� Desired Value
}