#include "Int_TB6612.h"
_stBT_Control_Angle BT_Control_Angle;






/**
 * @description: 设置所有电机默认方向
 * @return {*}
 */
void Int_TB6612_All_Motor()
{
    /* 第一个电机控制方向等 */
    TB6612_2_BIN1_L;
    TB6612_2_BIN2_H;
    /* 第二个电机控制方向等 */
    TB6612_1_AIN1_L;
    TB6612_1_AIN2_H;
    /* 第三个电机控制方向等 */
    TB6612_1_BIN1_L;
    TB6612_1_BIN2_H;
    /* 第四个电机控制方向等 */
    TB6612_2_AIN1_L;
    TB6612_2_AIN2_H;

}







/**
 * @description: 设置四个电机的PWM占空比
 * @param {int} percent_pwm_motor1 电机1占空比(0-1000)
 * @param {int} percent_pwm_motor2 电机2占空比(0-1000)
 * @param {int} percent_pwm_motor3 电机3占空比(0-1000)
 * @param {int} percent_pwm_motor4 电机4占空比(0-1000)
 * @return {*}
 */
void Int_TB6612_SetPWM(int16_t percent_pwm_motor1,int16_t percent_pwm_motor2,int16_t percent_pwm_motor3,int16_t percent_pwm_motor4)
{


    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, (percent_pwm_motor3 * 7200) / 1000);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, (percent_pwm_motor2 * 7200) / 1000);

    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, (percent_pwm_motor1 * 7200) / 1000);
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, (percent_pwm_motor4 * 7200) / 1000);
}




 /* 上升 */
void Motor_go_up(void)
{   
    
    
    BT_Control_Angle.throttle += 100;     /* 油门加大 */
    BT_Control_Angle.throttle = constrain(BT_Control_Angle.throttle, 0, 1000);

    
}

 /* 下降 */
 void Motor_go_down(void)
{   
    
    BT_Control_Angle.throttle -= 100;     /* 油门减少 */

    BT_Control_Angle.throttle = constrain(BT_Control_Angle.throttle, 0, 1000);

    
}

/* 前进 pith*/
void Motor_go_front(void)
{ 
    
    BT_Control_Angle.BTpitch += 100;     
    BT_Control_Angle.BTpitch = constrain(BT_Control_Angle.BTpitch, -500, 500);

}

/* 后退 pitch*/
void Motor_go_back(void)
{ 
    
    BT_Control_Angle.BTpitch -= 100;     
    BT_Control_Angle.BTpitch = constrain(BT_Control_Angle.BTpitch, -500, 500);

}


/* 左移 roll*/
void Motor_go_left(void)
{ 
    BT_Control_Angle.BTroll += 100;     
    BT_Control_Angle.BTroll = constrain(BT_Control_Angle.BTroll, -500, 500);

}


/* 右移 roll*/
void Motor_go_right(void)
{ 
    BT_Control_Angle.BTroll -= 100;     
    BT_Control_Angle.BTroll = constrain(BT_Control_Angle.BTroll, -500, 500);

}



/* 左旋转 yaw*/
void Motor_go_left_rotate(void)
{ 
    BT_Control_Angle.BTyaw += 100;     
   BT_Control_Angle.BTyaw = constrain(BT_Control_Angle.BTyaw, -500, 500);

}
/* 右旋转 yaw*/ 
void Motor_go_right_rotate(void)
{ 
    BT_Control_Angle.BTyaw -= 100;     
    BT_Control_Angle.BTyaw = constrain(BT_Control_Angle.BTyaw, -500, 500);

}


/* 停止*/
void Motor_stop(void)
{ 
    BT_Control_Angle.throttle = 0;
    BT_Control_Angle.BTpitch  = 0;
    BT_Control_Angle.BTroll   = 0;
    BT_Control_Angle.BTyaw    = 0;
  
 
  
    Int_TB6612_SetPWM(0,0,0,0);

}



