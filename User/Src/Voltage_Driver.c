#include "Voltage_Driver.h"
#include "SysTick_Driver.h"



__IO uint16_t  ADC_ConvertedValue[50];

__IO uint16_t After_filter;    //���������ƽ��ֵ֮��Ľ��

DMA_HandleTypeDef hdma_adcx;
ADC_HandleTypeDef ADC_Handle;



ADC_ChannelConfTypeDef ADC_Config;

static void Voltage_ADC_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RHEOSTAT_ADC_CLK_ENABLE(); 
  // ʹ�� GPIO ʱ��
  RHEOSTAT_ADC_GPIO_CLK_ENABLE();
        
  // ���� IO
  GPIO_InitStructure.Pin = RHEOSTAT_ADC_GPIO_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;	    
//  GPIO_InitStructure.Pull = GPIO_NOPULL ; //������������
  HAL_GPIO_Init(RHEOSTAT_ADC_GPIO_PORT, &GPIO_InitStructure);		
}

static void Voltage_ADC_Mode_Config(void)
{
    // ------------------DMA Init �ṹ����� ��ʼ��--------------------------
    // ����DMAʱ��
    RHEOSTAT_ADC_DMA_CLK_ENABLE();
	
    // ���ݴ���ͨ��
     hdma_adcx.Instance = RHEOSTAT_ADC_DMA_STREAM;
  
     hdma_adcx.Init.Direction = DMA_PERIPH_TO_MEMORY;;            
     hdma_adcx.Init.PeriphInc = DMA_PINC_DISABLE;                 //��������ģʽ
     hdma_adcx.Init.MemInc = DMA_MINC_ENABLE;                     //�洢������ģʽ 
     hdma_adcx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adcx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
     hdma_adcx.Init.Mode= DMA_CIRCULAR;                         //ѭ��ģʽ
     hdma_adcx.Init.Priority=DMA_PRIORITY_MEDIUM;               //�е����ȼ�

    //��ʼ��DMA�������൱��һ����Ĺܵ����ܵ������кܶ�ͨ��
    HAL_DMA_Init(&hdma_adcx); 

    __HAL_LINKDMA( &ADC_Handle,DMA_Handle,hdma_adcx);
  
   //---------------------------------------------------------------------------
    RCC_PeriphCLKInitTypeDef ADC_CLKInit;
    // ����ADCʱ��
    ADC_CLKInit.PeriphClockSelection=RCC_PERIPHCLK_ADC;			//ADC����ʱ��
    ADC_CLKInit.AdcClockSelection=RCC_ADCPCLK2_DIV8;			  //��Ƶ����6ʱ��Ϊ72M/8=9MHz
    HAL_RCCEx_PeriphCLKConfig(&ADC_CLKInit);					      //����ADCʱ��
   
    ADC_Handle.Instance = RHEOSTAT_ADC;
    ADC_Handle.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //�Ҷ���
    ADC_Handle.Init.ScanConvMode=DISABLE;                      //��ɨ��ģʽ
    ADC_Handle.Init.ContinuousConvMode=ENABLE;                 //����ת��
    ADC_Handle.Init.NbrOfConversion=1;                         //1��ת���ڹ��������� Ҳ����ֻת����������1 
    ADC_Handle.Init.DiscontinuousConvMode=DISABLE;             //����������ģʽ
    ADC_Handle.Init.NbrOfDiscConversion=0;                     //����������ͨ����Ϊ0
    ADC_Handle.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //�������
    HAL_ADC_Init(&ADC_Handle);                                 //��ʼ�� 
 
 //---------------------------------------------------------------------------
    ADC_Config.Channel      = RHEOSTAT_ADC_CHANNEL;
    ADC_Config.Rank         = 1;
    // ����ʱ����	
    ADC_Config.SamplingTime = ADC_SAMPLETIME_55CYCLES_5 ;
    // ���� ADC ͨ��ת��˳��Ϊ1����һ��ת��������ʱ��Ϊ3��ʱ������
    HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);

    HAL_ADC_Start_DMA(&ADC_Handle, (uint32_t*)&ADC_ConvertedValue, 50);
		
}



void Voltage_Init(void)
{
	Voltage_ADC_GPIO_Config();
	Voltage_ADC_Mode_Config();
}

//��ƽ��ֵ����
static void Voltage_ADC_Filter(void)
{
	int  sum = 0;
	uint8_t  count; 
	

	for(count=0;count<50;count++)
	{
		sum += ADC_ConvertedValue[count];
		delay_ms(10);
		printf("temp[%d] = %d\n",count,ADC_ConvertedValue[count]);
	}
	After_filter = sum/50;
	
}

/***************************************
**�������ܣ���ȡ�ɼ���ѹֵ
**��	ʽ��V(sample) = V(REF) * Value/(0x0FFF + 1)
****************************************/
float Get_VoltageValue(void)
{
	float Value;
	Voltage_ADC_Filter();
	
 //(float) ADC_ConvertedValue/4096*(float)3.3; // ��ȡת����ADֵ
	Value = (float)After_filter * ((float)3.3 / 4096);

	return Value;
}






