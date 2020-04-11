#ifndef __VOLTAGE_DRIVER_H_
#define	__VOLTAGE_DRIVER_H_

#include "stm32f1xx.h"


// ADC GPIO �궨��
#define RHEOSTAT_ADC_GPIO_PORT              GPIOA
#define RHEOSTAT_ADC_GPIO_PIN               GPIO_PIN_7
#define RHEOSTAT_ADC_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
    
// ADC ��ź궨��
#define RHEOSTAT_ADC                        ADC1
#define RHEOSTAT_ADC_CLK_ENABLE()           __HAL_RCC_ADC1_CLK_ENABLE(); 
#define RHEOSTAT_ADC_CHANNEL                ADC_CHANNEL_7

// ADC DMA ͨ���궨�壬��������ʹ��DMA����
#define RHEOSTAT_ADC_DMA_CLK_ENABLE()       __HAL_RCC_DMA1_CLK_ENABLE();
#define RHEOSTAT_ADC_DMA_STREAM             DMA1_Channel1

void Voltage_Init(void);
float Get_VoltageValue(void);

#endif /* __VOLTAGE_DRIVER_H_ */


