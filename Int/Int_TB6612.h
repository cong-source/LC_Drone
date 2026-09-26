#ifndef __INT_TB6612_H
#define __INT_TB6612_H

#include "tim.h"





#define TB6612_1_AIN1_L HAL_GPIO_WritePin(Moter2_AN1_GPIO_Port,Moter2_AN1_Pin,GPIO_PIN_RESET)
#define TB6612_1_AIN1_H HAL_GPIO_WritePin(Moter2_AN1_GPIO_Port,Moter2_AN1_Pin,GPIO_PIN_SET)

#define TB6612_1_AIN2_L HAL_GPIO_WritePin(Moter2_AN2_GPIO_Port,Moter2_AN2_Pin,GPIO_PIN_RESET)
#define TB6612_1_AIN2_H HAL_GPIO_WritePin(Moter2_AN2_GPIO_Port,Moter2_AN2_Pin,GPIO_PIN_SET)

#define TB6612_1_BIN1_L HAL_GPIO_WritePin(Moter3_BN1_GPIO_Port,Moter3_BN1_Pin,GPIO_PIN_RESET)
#define TB6612_1_BIN1_H HAL_GPIO_WritePin(Moter3_BN1_GPIO_Port,Moter3_BN1_Pin,GPIO_PIN_SET)

#define TB6612_1_BIN2_L HAL_GPIO_WritePin(Moter3_BN1_GPIO_Port,Moter3_BN2_Pin,GPIO_PIN_RESET)
#define TB6612_1_BIN2_H HAL_GPIO_WritePin(Moter3_BN1_GPIO_Port,Moter3_BN2_Pin,GPIO_PIN_SET)



#define TB6612_2_AIN1_L HAL_GPIO_WritePin(Moter4_AN1_GPIO_Port,Moter4_AN1_Pin,GPIO_PIN_RESET)
#define TB6612_2_AIN1_H HAL_GPIO_WritePin(Moter4_AN1_GPIO_Port,Moter4_AN1_Pin,GPIO_PIN_SET)

#define TB6612_2_AIN2_L HAL_GPIO_WritePin(Moter4_AN2_GPIO_Port,Moter4_AN2_Pin,GPIO_PIN_RESET)
#define TB6612_2_AIN2_H HAL_GPIO_WritePin(Moter4_AN2_GPIO_Port,Moter4_AN2_Pin,GPIO_PIN_SET)

#define TB6612_2_BIN1_L HAL_GPIO_WritePin(Moter1_BN1_GPIO_Port,Moter1_BN1_Pin,GPIO_PIN_RESET)
#define TB6612_2_BIN1_H HAL_GPIO_WritePin(Moter1_BN1_GPIO_Port,Moter1_BN1_Pin,GPIO_PIN_SET)

#define TB6612_2_BIN2_L HAL_GPIO_WritePin(Moter1_BN1_GPIO_Port,Moter1_BN2_Pin,GPIO_PIN_RESET)
#define TB6612_2_BIN2_H HAL_GPIO_WritePin(Moter1_BN1_GPIO_Port,Moter1_BN2_Pin,GPIO_PIN_SET)

#define constrain(x,min,max)  (((x)<(min))?(min):(((x)>(max))?(max):(x)))

typedef struct
{
	float BTpitch;
	float BTroll;
	float BTyaw;	
    float throttle;	
}_stBT_Control_Angle;

extern _stBT_Control_Angle BT_Control_Angle;

void Int_TB6612_All_Motor(void);


void Int_TB6612_SetPWM(int16_t percent_pwm_motor1,int16_t percent_pwm_motor2,int16_t percent_pwm_motor3,int16_t percent_pwm_motor4);


void Motor_go_up(void);
void Motor_go_down(void);
void Motor_go_front(void);
void Motor_go_back(void);
void Motor_go_left(void);
void Motor_go_right(void);
void Motor_go_left_rotate(void);
void Motor_go_right_rotate(void);
void Motor_stop(void);


#endif
