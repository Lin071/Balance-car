#include "Control.h"


// ��־λ����
Flag flag;
// pid����
PID speedPid,stabilityPid,directPid;
// �������Ʊ���
int16_t leftSpeed,rightSpeed;
float totalSpeed;
int16_t leftWheelOut,rightWheelOut,stabilityOut;
// ң��ģʽ�µı���
int16_t speedIncrementalTemp,deviateSpeed;



// �������ܿ���(10msִ��һ��)
void motorControlOut(void)
{
    leftWheelOut = (int16_t)(speedPid.out - directPid.out);
    rightWheelOut = (int16_t)(speedPid.out + directPid.out);
    
    setMotorPwm(leftWheelOut,rightWheelOut);
}

float angle_z,angularVelocity_z; // ��λ����/s;
// ���򻷿���
void directLoop(void)
{
    
    short gx,gy,gz;
    MPU_Get_Gyroscope(&gx,&gy,&gz);
    
    angularVelocity_z = gz / 16.384f; 
    angle_z += angularVelocity_z * 0.01f;
    
    differentialFirstPidCalculate(&angularVelocity_z,&directPid.expect,&directPid);

    motorControlOut();
}


// �ٶȻ�����
void speedLoop(void)
{
    getSpeed(&leftSpeed,&rightSpeed);
    totalSpeed = (leftSpeed + rightSpeed) / 2.0f;

    pidCalculate2(&totalSpeed,&speedPid.expect,&speedPid,5000);
}

float pitch,roll,yaw;
// ���Ȼ�����
void stabilityLoop(void)
{
    mpu_dmp_get_data(&pitch,&roll,&yaw);  //===�õ�ŷ���ǣ���̬�ǣ�������
    pidCalculate(&roll,&stabilityPid.expect,&stabilityPid,10000);
    steerSynchronousOut(stabilityPid.out);
}


// RGB�ʵƿ���
void lightControl(void)
{
    switch(flag.rgbState)
    {
        case RGB_OFF:
             RGB_LED_Off();
             break;
        case RGB_FLY:
             RGB_LED_FLY();
             break;
        case RGB_RUNNING:
             RGB_LED_Runing();
             break;
        case RGB_RED:
             RGB_LED_Red();
             break;
        case RGB_BLUE:
             RGB_LED_Blue();
             break;
        case RGB_YELLOW:
             RGB_LED_Yellow();
             break;
        case RGB_WHITE:
             RGB_LED_White();
             break;
    }
}


// ������ʼ��
void parameterInit(void)
{
    // ��־λ��ʼ��
    flag.rgbState = RGB_RUNNING;
    flag.remoteState = CARSTOP_MODE;
    
    // PIDϵ����ʼ��
    speedPid.expect = 0;
    speedPid.kp = 10.50f;
    speedPid.ki = 0.07f;
    speedPid.kd = 0.0f;
    
    directPid.expect = 0.0f;
    directPid.kp = 0.0f;
    directPid.ki = 0.0f;
    directPid.kd = 0.0f;
    
    stabilityPid.expect = -0.0f;
    stabilityPid.kp = 3.40f;
    stabilityPid.ki = 19.10f;
    stabilityPid.kd = 0.0f;
}


// 
void pidCalculate2(float *value,float *expect,PID *pid,float limit)
{
	pid->error = *expect - *value;

	if(totalSpeed != 0){
		pid->integral += pid->error;
	}else{
		pid->integral = 0;
	}

	pid->integral = Limiting_float(pid->integral,-limit,limit);

	pid->out = pid->kp*pid->error + pid->ki*pid->integral + pid->kd*(pid->error - pid->errorLast);

	pid->errorLast = pid->error;
}


//λ��ʽPID���㹫ʽ
void pidCalculate(float *value,float *expect,PID *pid,float limit)
{
	pid->error = *expect - *value;

//	if(totalSpeed != 0){
//		pid->integral += pid->error;
//	}else{
//		pid->integral = 0;
//	}

	pid->integral = Limiting_float(pid->integral,-limit,limit);

	pid->out = pid->kp*pid->error + pid->ki*pid->integral + pid->kd*(pid->error - pid->errorLast);

	pid->errorLast = pid->error;
}


//΢������PID��ʽ
void differentialFirstPidCalculate(float *value,float *expect,PID *pid)
{
	pid->error = *expect - *value;

	pid->out = pid->kp*pid->error + pid->kd*(*value - pid->errorLast);

	pid->errorLast = *value;
}
