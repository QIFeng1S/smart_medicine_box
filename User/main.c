#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"
#include "servo.h"
#include "dht11_multi.h"
#include "Key.h"

int main(void)
{
	uint8_t temp1, temp2, temp3;    // 三个盒子的温度
	uint8_t humi1, humi2, humi3;    // 三个盒子的湿度
	uint8_t currentBox = 1;         // 当前选中的盒子编号
	uint8_t key;                    // 按键值
	uint8_t boxStatus[3] = {0, 0, 0};  // 三个盒子的状态，0表示关闭，1表示打开
	
	OLED_Init();
	OLED_ShowString(1, 1, "medicine_box");
	OLED_ShowString(2, 1, "Box:");
	OLED_ShowString(3, 1, "T:");
	OLED_ShowString(4, 1, "H:");
	
	Servo_Init();
	DHT11_Multi_Init();  // 初始化三个DHT11
	Key_Init();          // 初始化按键
	
	// 初始化舵机位置（关闭状态）
	ServoA_SetAngle(0);
	ServoB_SetAngle(0);
	ServoC_SetAngle(0);
	
	while (1)	
	{
		// 读取所有DHT11数据
		DHT11_1_Read_Data(&temp1, &humi1);
		DHT11_2_Read_Data(&temp2, &humi2);
		DHT11_3_Read_Data(&temp3, &humi3);
		
		// 检测按键
		key = Key_GetNum();
		if (key == 2)  // 按键2按下，切换选中的盒子
		{
			currentBox = (currentBox % 3) + 1;  // 循环切换1-3
		}
		else if (key == 1)  // 按键1按下，开关当前选中的盒子
		{
			boxStatus[currentBox - 1] = !boxStatus[currentBox - 1];  // 切换状态
			
			// 根据当前选中的盒子控制对应的舵机
			switch(currentBox)
			{
				case 1:
					ServoA_SetAngle(boxStatus[0] ? 90 : 0);  // 打开为90度，关闭为0度
					break;
				case 2:
					ServoB_SetAngle(boxStatus[1] ? 90 : 0);
					break;
				case 3:
					ServoC_SetAngle(boxStatus[2] ? 90 : 0);
					break;
			}
		}
		
		// 显示当前选中的盒子编号和状态
		OLED_ShowNum(2, 5, currentBox, 1);
		OLED_ShowString(2, 7, boxStatus[currentBox - 1] ? "Open " : "Close");
		
		// 显示温湿度
		switch(currentBox)
		{
			case 1:
				OLED_ShowNum(3, 3, temp1, 2);
				OLED_ShowChar(3, 5, 'C');
				OLED_ShowNum(4, 3, humi1, 2);
				OLED_ShowChar(4, 5, '%');
				break;
			case 2:
				OLED_ShowNum(3, 3, temp2, 2);
				OLED_ShowChar(3, 5, 'C');
				OLED_ShowNum(4, 3, humi2, 2);
				OLED_ShowChar(4, 5, '%');
				break;
			case 3:
				OLED_ShowNum(3, 3, temp3, 2);
				OLED_ShowChar(3, 5, 'C');
				OLED_ShowNum(4, 3, humi3, 2);
				OLED_ShowChar(4, 5, '%');
				break;
		}
		
		Delay_ms(1000);  // 每秒更新一次数据
	}
}
