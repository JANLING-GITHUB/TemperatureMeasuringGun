#ifndef __VOLTAGE_DRIVER_H_
#define	__VOLTAGE_DRIVER_H_

#include "stm32f1xx.h"

#define USE_DMA //???,?????DMA??

// ADC GPIO ???
#define VOLTAGE_ADC_GPIO_PORT              GPIOA
#define VOLTAGE_ADC_GPIO_PIN               GPIO_PIN_7
#define VOLTAGE_ADC_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
    
// ADC ?????
#define VOLTAGE_ADC                        ADC1
#define VOLTAGE_ADC_CLK_ENABLE()            __HAL_RCC_ADC1_CLK_ENABLE(); 
#define VOLTAGE_ADC_CHANNEL                ADC_CHANNEL_7

#ifdef USE_DMA
// ADC DMA ?????
#define VOLTAGE_ADC_DMA_CLK_ENABLE()       __HAL_RCC_DMA1_CLK_ENABLE();                                      
#define VOLTAGE_ADC_DMA_CHANNEL             DMA1_Channel1  
static void Voltage_ADC_DMA_Config(void);

#else
// ADC ?????
#define Voltage_ADC_IRQ                    ADC1_IRQn
#define Voltage_ADC_INT_FUNCTION           ADC1_IRQHandler


extern __IO uint32_t ADC_ConvertedValue;

#endif //#ifdef USE_DMA
void Voltage_Init(void);
float Get_VoltageValue(void);

#endif /* __VOLTAGE_DRIVER_H_ */
