#include "App_Drone.h"
#include "Int_MPU6050.h"
#include "Com_Filter.h"
#include "Com_PID.h"
#include "Int_TB6612.h"
#include "bluetooth.h"

_stMPU MPU6050;
_stAngle Angle;
uint16_t MPU_Offset[6] = {0};

PidObject pidPith;
PidObject pidRoll;
PidObject pidYaw;

PidObject pidRateX;
PidObject pidRateY;
PidObject pidRateZ;

PidObject *pids[]= {&pidPith,&pidRoll,&pidYaw,&pidRateX,&pidRateY,&pidRateZ};

extern _stBT_Control_Angle BT_Control_Angle;

extern const float Gyro_G;


/* Motor control variables */
int16_t motor1 =0;
int16_t motor2 =0;
int16_t motor3 =0;
int16_t motor4 =0;



void App_Flight_MPU_DATA(void)
{
    

    Int_MPU6050_Get_Accel(&MPU6050.accX, &MPU6050.accY,  &MPU6050.accZ);
    Int_MPU6050_Get_Gyro(&MPU6050.gyroX, &MPU6050.gyroY,  &MPU6050.gyroZ);


    MPU6050.accX=MPU6050.accX-MPU_Offset[0];
    MPU6050.accY=MPU6050.accY-MPU_Offset[1];
    MPU6050.accZ=MPU6050.accZ-MPU_Offset[2];
    MPU6050.gyroX=MPU6050.gyroX-MPU_Offset[3];
    MPU6050.gyroY=MPU6050.gyroY-MPU_Offset[4];
    MPU6050.gyroZ=MPU6050.gyroZ-MPU_Offset[5];



    Kalman_1(&ekf[0], MPU6050.accX);
    MPU6050.accX = (int16_t)ekf[0].out;
    Kalman_1(&ekf[1], MPU6050.accY);
    MPU6050.accY = (int16_t)ekf[1].out;
    Kalman_1(&ekf[2], MPU6050.accZ);
    MPU6050.accZ = (int16_t)ekf[2].out;


    static int16_t lastGyro[3] = {0,0,0};
    MPU6050.gyroX = 0.85 * lastGyro[0] + 0.15 * MPU6050.gyroX;
    lastGyro[0] = MPU6050.gyroX;
    MPU6050.gyroY = 0.85 * lastGyro[1] + 0.15 * MPU6050.gyroY;
    lastGyro[1] = MPU6050.gyroY;
    MPU6050.gyroZ = 0.85 * lastGyro[2] + 0.15 * MPU6050.gyroZ;
    lastGyro[2] = MPU6050.gyroZ;


}


void App_Flight_MPU_Offsets(void)
{
    uint8_t gyro_i=30;
    uint32_t buff[6]={0};
    const uint8_t MAX_GYRO_QUIET = 5;
    const int8_t  MIN_GYRO_QUIET = -5;
    int16_t LastGyro[3] = {0};
    int16_t Err_Gyro[3] = {0};
    while (gyro_i--) {
        do {
            HAL_Delay(10);
            App_Flight_MPU_DATA();
            Err_Gyro[0]=MPU6050.gyroX-LastGyro[0];
            Err_Gyro[1]=MPU6050.gyroY-LastGyro[1];
            Err_Gyro[2]=MPU6050.gyroZ-LastGyro[2];
            LastGyro[0]=MPU6050.gyroX;
            LastGyro[1]=MPU6050.gyroY;
            LastGyro[2]=MPU6050.gyroZ;
        }while(
            Err_Gyro[0]>MAX_GYRO_QUIET || Err_Gyro[0]<MIN_GYRO_QUIET ||
            Err_Gyro[1]>MAX_GYRO_QUIET || Err_Gyro[1]<MIN_GYRO_QUIET ||
            Err_Gyro[2]>MAX_GYRO_QUIET || Err_Gyro[2]<MIN_GYRO_QUIET
        );
    }
    for (uint16_t i = 0; i < 356; i++) 
    {
        App_Flight_MPU_DATA();
        if (i >= 100)
         {
            buff[0] += MPU6050.accX;
            buff[1] += MPU6050.accY;
            buff[2] += MPU6050.accZ - 16384;
            buff[3] += MPU6050.gyroX;
            buff[4] += MPU6050.gyroY;
            buff[5] += MPU6050.gyroZ;
        }
    }
    for(uint8_t i=0;i<6;i++)
    {
        MPU_Offset[i]=buff[i]>>8;
    }
}

void App_Flight_PID__Control(float dt)
{
    



    pidPith.measured=Angle.pitch;
    pidRoll.measured=Angle.roll;
    pidYaw.measured=Angle.yaw;


    pidRateX.measured=MPU6050.gyroX *Gyro_G;
    pidRateY.measured=MPU6050.gyroY *Gyro_G;
    pidRateZ.measured=MPU6050.gyroZ *Gyro_G;


    PID_Cascade(&pidPith, &pidRateY, dt);
    PID_Cascade(&pidRoll, &pidRateX, dt);
    PID_Cascade(&pidYaw, &pidRateZ, dt);

    

   
}
void App_Flight_Motor_Control()
{
    motor1 = BT_Control_Angle.throttle + BT_Control_Angle.BTpitch + BT_Control_Angle.BTroll + BT_Control_Angle.BTyaw;
    motor2 = BT_Control_Angle.throttle - BT_Control_Angle.BTpitch + BT_Control_Angle.BTroll - BT_Control_Angle.BTyaw;
    motor3 = BT_Control_Angle.throttle - BT_Control_Angle.BTpitch - BT_Control_Angle.BTroll + BT_Control_Angle.BTyaw;
    motor4 = BT_Control_Angle.throttle + BT_Control_Angle.BTpitch - BT_Control_Angle.BTroll - BT_Control_Angle.BTyaw;

     motor1 = constrain(motor1,0,900);
     motor2 = constrain(motor2,0,900);
     motor3 = constrain(motor3,0,900);
     motor4 = constrain(motor4,0,900);

    motor1 += +pidRateX.out + pidRateY.out + pidRateZ.out;
    motor2 += +pidRateX.out - pidRateY.out - pidRateZ.out;
    motor3 += -pidRateX.out - pidRateY.out + pidRateZ.out;
    motor4 += -pidRateX.out + pidRateY.out - pidRateZ.out;

  



    Int_TB6612_SetPWM(motor1, motor2, motor3, motor4);
}
