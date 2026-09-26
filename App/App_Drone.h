#ifndef __APP_DRONE_H
#define __APP_DRONE_H


#include "Int_MPU6050.h"
#include "Com_Filter.h"
#include "math.h"

#include "oled.h"
#include "Com_PID.h"
#include "Int_TB6612.h"
#include "usart.h"
#include "stm32f1xx_hal_uart.h"

#define PI 3.14159265




typedef struct
{
	int16_t accX;
	int16_t accY;
	int16_t accZ;
	int16_t gyroX;
	int16_t gyroY;
	int16_t gyroZ;
}_stMPU;

typedef struct
{
	float pitch;
	float roll;
	float yaw;	
}_stAngle;


extern _stMPU MPU6050;
extern _stAngle Angle;
extern PidObject *pids[];




void App_Flight_MPU_DATA(void);
void App_Flight_MPU_Offsets(void);
void App_Flight_Motor_Control(void);
void App_Flight_PID__Control(float dt);

#endif
