#ifndef __IMU_H
#define __IMU_H

#include "App_Drone.h"
#include <math.h>

/* 平方宏 */
#define squa( Sq )   ( (float)(Sq) * (float)(Sq) )

/* 弧度转角度系数 / 陀螺仪量程系数 */
extern const float RtA;
extern const float Gyro_G;
extern const float Gyro_Gr;

/* 四元数互补滤波姿态解算 + 垂直方向加速度 */
void  GetAngle(const _stMPU *pMpu, _stAngle *pAngE, float dt);
float GetAccz(void);

#endif
