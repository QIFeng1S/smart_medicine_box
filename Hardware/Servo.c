#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void Servo_Init(void)
{
	PWM_Init();
}	

void ServoA_SetAngle(float Angle)
{
	PWM_SetCompare1(Angle / 180 * 2000 + 500);
}	

void ServoB_SetAngle(float Angle)
{
	PWM_SetCompare2(Angle / 180 * 2000 + 500);
}	

void ServoC_SetAngle(float Angle)
{
	PWM_SetCompare3(Angle / 180 * 2000 + 500);
}
