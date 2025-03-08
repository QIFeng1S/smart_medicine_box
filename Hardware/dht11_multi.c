#include "dht11_multi.h"
#include "Delay.h"

// 函数声明
static uint8_t DHT11_Read_Byte(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

// DHT11_1 控制函数
static void DHT11_1_Mode(uint8_t mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DHT11_1_GPIO_PIN;
    if(mode == 1)  // 输出模式
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    }
    else  // 输入模式
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    }
    GPIO_Init(DHT11_1_GPIO_PORT, &GPIO_InitStructure);
}

// DHT11_2 控制函数
static void DHT11_2_Mode(uint8_t mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DHT11_2_GPIO_PIN;
    if(mode == 1)
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    }
    else
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    }
    GPIO_Init(DHT11_2_GPIO_PORT, &GPIO_InitStructure);
}

// DHT11_3 控制函数
static void DHT11_3_Mode(uint8_t mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DHT11_3_GPIO_PIN;
    if(mode == 1)
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    }
    else
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    }
    GPIO_Init(DHT11_3_GPIO_PORT, &GPIO_InitStructure);
}

void DHT11_Multi_Init(void)
{
    RCC_APB2PeriphClockCmd(DHT11_1_GPIO_CLK | DHT11_2_GPIO_CLK | DHT11_3_GPIO_CLK, ENABLE);
    
    DHT11_1_Mode(1);
    DHT11_2_Mode(1);
    DHT11_3_Mode(1);
    
    GPIO_SetBits(DHT11_1_GPIO_PORT, DHT11_1_GPIO_PIN);
    GPIO_SetBits(DHT11_2_GPIO_PORT, DHT11_2_GPIO_PIN);
    GPIO_SetBits(DHT11_3_GPIO_PORT, DHT11_3_GPIO_PIN);
}

// 读取 DHT11_1 数据
uint8_t DHT11_1_Read_Data(uint8_t *temp, uint8_t *humi)
{
    uint8_t buf[5];
    uint8_t i;
    
    DHT11_1_Mode(1);
    GPIO_ResetBits(DHT11_1_GPIO_PORT, DHT11_1_GPIO_PIN);
    Delay_ms(20);
    GPIO_SetBits(DHT11_1_GPIO_PORT, DHT11_1_GPIO_PIN);
    Delay_us(30);
    
    DHT11_1_Mode(0);
    if(GPIO_ReadInputDataBit(DHT11_1_GPIO_PORT, DHT11_1_GPIO_PIN) == 0)
    {
        while(GPIO_ReadInputDataBit(DHT11_1_GPIO_PORT, DHT11_1_GPIO_PIN) == 0);
        while(GPIO_ReadInputDataBit(DHT11_1_GPIO_PORT, DHT11_1_GPIO_PIN) == 1);
        
        for(i = 0; i < 5; i++)
        {
            buf[i] = DHT11_Read_Byte(DHT11_1_GPIO_PORT, DHT11_1_GPIO_PIN);
        }
        
        if((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])
        {
            *humi = buf[0];
            *temp = buf[2];
        }
        return 0;
    }
    return 1;
}

// 读取 DHT11_2 数据
uint8_t DHT11_2_Read_Data(uint8_t *temp, uint8_t *humi)
{
    uint8_t buf[5];
    uint8_t i;
    
    DHT11_2_Mode(1);
    GPIO_ResetBits(DHT11_2_GPIO_PORT, DHT11_2_GPIO_PIN);
    Delay_ms(20);
    GPIO_SetBits(DHT11_2_GPIO_PORT, DHT11_2_GPIO_PIN);
    Delay_us(30);
    
    DHT11_2_Mode(0);
    if(GPIO_ReadInputDataBit(DHT11_2_GPIO_PORT, DHT11_2_GPIO_PIN) == 0)
    {
        while(GPIO_ReadInputDataBit(DHT11_2_GPIO_PORT, DHT11_2_GPIO_PIN) == 0);
        while(GPIO_ReadInputDataBit(DHT11_2_GPIO_PORT, DHT11_2_GPIO_PIN) == 1);
        
        for(i = 0; i < 5; i++)
        {
            buf[i] = DHT11_Read_Byte(DHT11_2_GPIO_PORT, DHT11_2_GPIO_PIN);
        }
        
        if((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])
        {
            *humi = buf[0];
            *temp = buf[2];
        }
        return 0;
    }
    return 1;
}

// 读取 DHT11_3 数据
uint8_t DHT11_3_Read_Data(uint8_t *temp, uint8_t *humi)
{
    uint8_t buf[5];
    uint8_t i;
    
    DHT11_3_Mode(1);
    GPIO_ResetBits(DHT11_3_GPIO_PORT, DHT11_3_GPIO_PIN);
    Delay_ms(20);
    GPIO_SetBits(DHT11_3_GPIO_PORT, DHT11_3_GPIO_PIN);
    Delay_us(30);
    
    DHT11_3_Mode(0);
    if(GPIO_ReadInputDataBit(DHT11_3_GPIO_PORT, DHT11_3_GPIO_PIN) == 0)
    {
        while(GPIO_ReadInputDataBit(DHT11_3_GPIO_PORT, DHT11_3_GPIO_PIN) == 0);
        while(GPIO_ReadInputDataBit(DHT11_3_GPIO_PORT, DHT11_3_GPIO_PIN) == 1);
        
        for(i = 0; i < 5; i++)
        {
            buf[i] = DHT11_Read_Byte(DHT11_3_GPIO_PORT, DHT11_3_GPIO_PIN);
        }
        
        if((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])
        {
            *humi = buf[0];
            *temp = buf[2];
        }
        return 0;
    }
    return 1;
}

// 读取一个字节
static uint8_t DHT11_Read_Byte(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    uint8_t i, temp = 0;
    
    for(i = 0; i < 8; i++)
    {
        while(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 0);
        Delay_us(40);
        if(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 1)
        {
            while(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 1);
            temp |= (uint8_t)(0x01 << (7 - i));
        }
    }
    return temp;
} 
