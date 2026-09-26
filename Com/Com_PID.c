#include "Com_PID.h"


void PID_Update(PidObject* pid,const float dt)
{
    float error;
    float deriv;

    error = pid->desired - pid->measured; //当前角度与实际角度的误差
    pid->integ += error * dt;     //误差积分累加值
    deriv = (error - pid->prevError)/dt;  //前后两次误差做微分
    pid->out = pid->kp * error + pid->ki * pid->integ + pid->kd * deriv;//PID输出
    pid->prevError = error;  //更新上次的误差
}


void PID_Cascade(PidObject* pidAngE,PidObject* pidRate,const float dt)  //串级PID
{
    PID_Update(pidAngE,dt);      //先计算外环
    pidRate->desired = pidAngE->out;
    PID_Update(pidRate,dt);      //再计算内环
}

 
void PID_Reset(PidObject **pid,const uint8_t len)
{
    uint8_t i;
    for(i=0;i<len;i++)
    {
        pid[i]->integ = 0;
        pid[i]->prevError = 0;
        pid[i]->out = 0;
        pid[i]->desired=0;
    }
}


