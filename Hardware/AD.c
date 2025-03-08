#include "stm32f10x.h"

void AD_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //    72M/6

    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;   // 模拟输入模式（ADC专属）
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //单次转换
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //右对齐
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //不使用外部触发
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  //独立模式，其他模式都是ADC1和ADC2的双ADC模式
    ADC_InitStructure.ADC_NbrOfChannel = 1; //通道数目
    ADC_InitStructure.ADC_ScanConvMode = DISABLE; //非扫描
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Cmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1) == SET);
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1) ==SET);

}

uint16_t AD_GetValue(uint8_t ADC_Channel)
{   

    ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5); //规则通道，在序列1写入通道0

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    return ADC_GetConversionValue(ADC1);
}
