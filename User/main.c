#include "stm32f10x.h"                  // STM32F10x设备头文件
#include "Delay.h"                     // 延时函数
#include "OLED.h"                      // OLED显示驱动
#include "AD.h"                        // 模数转换
#include "servo.h"                     // 舵机控制
#include "dht11_multi.h"               // 多路DHT11温湿度传感器
#include "Key.h"                       // 按键控制
#include "Serial.h"                    // 串口通信

int main(void)
{
	uint8_t temp1, temp2, temp3;    // 三个药盒的温度值
	uint8_t humi1, humi2, humi3;    // 三个药盒的湿度值
	uint8_t currentBox = 1;         // 当前选中的药盒编号（1-3）
	uint8_t key;                    // 按键值
	uint8_t boxStatus[3] = {0, 0, 0};  // 三个药盒的开关状态数组，0表示关闭，1表示打开
	uint8_t receivedData;           // 串口接收到的数据
	char sendBuf[50];              // 串口发送数据缓冲区
	
	// 初始化各个外设
	OLED_Init();                   // 初始化OLED显示屏
	OLED_ShowString(1, 1, "medicine_box");  // 显示标题
	OLED_ShowString(2, 1, "Box:");         // 显示药盒编号标签
	OLED_ShowString(3, 1, "T:");           // 显示温度标签
	OLED_ShowString(4, 1, "H:");           // 显示湿度标签
	
	Servo_Init();                  // 初始化舵机
	DHT11_Multi_Init();            // 初始化三个DHT11温湿度传感器
	Key_Init();                    // 初始化按键
	Serial_Init();                 // 初始化串口
	
	// 初始化所有舵机为关闭状态（0度）
	ServoA_SetAngle(0);
	ServoB_SetAngle(0);
	ServoC_SetAngle(0);
	
	while (1)	
	{
		// 读取三个DHT11传感器的温湿度数据
		DHT11_1_Read_Data(&temp1, &humi1);
		DHT11_2_Read_Data(&temp2, &humi2);
		DHT11_3_Read_Data(&temp3, &humi3);
		
		// 检测按键输入和串口数据
		key = Key_GetNum();
		if (Serial_GetRxFlag())  // 如果接收到串口数据
		{
			receivedData = Serial_GetRxData();
			if (receivedData >= '1' && receivedData <= '3')  // 收到1-3，切换选中的药盒
			{
				currentBox = receivedData - '0';
				// 发送当前选中药盒的温湿度数据
				sprintf(sendBuf, "Box%d: T=%dC, H=%d%%\r\n", 
					currentBox, 
					(currentBox == 1) ? temp1 : ((currentBox == 2) ? temp2 : temp3),
					(currentBox == 1) ? humi1 : ((currentBox == 2) ? humi2 : humi3));
				Serial_SendString(sendBuf);
			}
			else if (receivedData == 'o' || receivedData == 'O')  // 收到o/O，打开当前药盒
			{
				boxStatus[currentBox - 1] = 1;
				switch(currentBox)
				{
					case 1: ServoA_SetAngle(90); break;  // 舵机转到90度表示打开
					case 2: ServoB_SetAngle(90); break;
					case 3: ServoC_SetAngle(90); break;
				}
				sprintf(sendBuf, "Box%d opened\r\n", currentBox);
				Serial_SendString(sendBuf);
			}
			else if (receivedData == 'c' || receivedData == 'C')  // 收到c/C，关闭当前药盒
			{
				boxStatus[currentBox - 1] = 0;
				switch(currentBox)
				{
					case 1: ServoA_SetAngle(0); break;   // 舵机转到0度表示关闭
					case 2: ServoB_SetAngle(0); break;
					case 3: ServoC_SetAngle(0); break;
				}
				sprintf(sendBuf, "Box%d closed\r\n", currentBox);
				Serial_SendString(sendBuf);
			}
			else if (receivedData == 's' || receivedData == 'S')  // 收到s/S，发送所有药盒的状态
			{
				sprintf(sendBuf, "Box1: T=%dC, H=%d%%, %s\r\n", 
					temp1, humi1, boxStatus[0] ? "Open" : "Close");
				Serial_SendString(sendBuf);
				sprintf(sendBuf, "Box2: T=%dC, H=%d%%, %s\r\n", 
					temp2, humi2, boxStatus[1] ? "Open" : "Close");
				Serial_SendString(sendBuf);
				sprintf(sendBuf, "Box3: T=%dC, H=%d%%, %s\r\n", 
					temp3, humi3, boxStatus[2] ? "Open" : "Close");
				Serial_SendString(sendBuf);
			}
		}
		
		if (key == 2)  // 按键2按下，切换选中的药盒
		{
			currentBox = (currentBox % 3) + 1;  // 循环切换1-3
			// 发送当前选中药盒的温湿度数据
			sprintf(sendBuf, "Box%d: T=%dC, H=%d%%\r\n", 
				currentBox, 
				(currentBox == 1) ? temp1 : ((currentBox == 2) ? temp2 : temp3),
				(currentBox == 1) ? humi1 : ((currentBox == 2) ? humi2 : humi3));
			Serial_SendString(sendBuf);
		}
		else if (key == 1)  // 按键1按下，切换当前选中药盒的开关状态
		{
			boxStatus[currentBox - 1] = !boxStatus[currentBox - 1];  // 切换状态
			
			// 根据当前选中的药盒控制对应的舵机
			switch(currentBox)
			{
				case 1:
					ServoA_SetAngle(boxStatus[0] ? 90 : 0);  // 根据状态设置舵机角度
					break;
				case 2:
					ServoB_SetAngle(boxStatus[1] ? 90 : 0);
					break;
				case 3:
					ServoC_SetAngle(boxStatus[2] ? 90 : 0);
					break;
			}
			
			// 发送药盒状态改变信息
			sprintf(sendBuf, "Box%d %s\r\n", currentBox, boxStatus[currentBox - 1] ? "opened" : "closed");
			Serial_SendString(sendBuf);
		}
		
		// 在OLED上显示当前选中的药盒编号和状态
		OLED_ShowNum(2, 5, currentBox, 1);
		OLED_ShowString(2, 7, boxStatus[currentBox - 1] ? "Open " : "Close");
		
		// 根据当前选中的药盒显示对应的温湿度数据
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
		
		Delay_ms(1000);  // 延时1秒，每秒更新一次数据
	}
}
