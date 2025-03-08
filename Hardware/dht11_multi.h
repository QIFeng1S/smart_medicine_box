#ifndef __DHT11_MULTI_H
#define __DHT11_MULTI_H

#include "stm32f10x.h"

// DHT11_1 配置（盒子1）
#define DHT11_1_GPIO_PORT  GPIOA
#define DHT11_1_GPIO_PIN   GPIO_Pin_6
#define DHT11_1_GPIO_CLK   RCC_APB2Periph_GPIOA

// DHT11_2 配置（盒子2）
#define DHT11_2_GPIO_PORT  GPIOA
#define DHT11_2_GPIO_PIN   GPIO_Pin_7
#define DHT11_2_GPIO_CLK   RCC_APB2Periph_GPIOA

// DHT11_3 配置（盒子3）
#define DHT11_3_GPIO_PORT  GPIOB
#define DHT11_3_GPIO_PIN   GPIO_Pin_0
#define DHT11_3_GPIO_CLK   RCC_APB2Periph_GPIOB

void DHT11_Multi_Init(void);
uint8_t DHT11_1_Read_Data(uint8_t *temp, uint8_t *humi);
uint8_t DHT11_2_Read_Data(uint8_t *temp, uint8_t *humi);
uint8_t DHT11_3_Read_Data(uint8_t *temp, uint8_t *humi);

#endif 
