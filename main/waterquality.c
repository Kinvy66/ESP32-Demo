#include "waterquality.h"


#define DEFAULT_VREF    1100       //默认参考电压，单位mV

#define WaterQuality_GPIO           5

#define WATERQUALITY_CHANNEL         ADC_CHANNEL_4               // ADC测量通道
#define width                       ADC_WIDTH_BIT_12            // ADC分辨率
#define atten                       ADC_ATTEN_DB_11             // ADC衰减
#define unit                        ADC_UNIT_1                  // ADC1


esp_adc_cal_characteristics_t *waterquality_adc_chars;


void waterquality_gpio_config(void)
{
    adc1_config_width(width);// 12位分辨率

    //ADC_ATTEN_DB_0:表示参考电压为1.1V
    //ADC_ATTEN_DB_2_5:表示参考电压为1.5V
    //ADC_ATTEN_DB_6:表示参考电压为2.2V
    //ADC_ATTEN_DB_11:表示参考电压为3.3V
    adc1_config_channel_atten( WATERQUALITY_CHANNEL,atten);// 设置通道0和3.3V参考电压

    // 分配内存
    waterquality_adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    // 对 ADC 特性进行初始化，使其能够正确地计算转换结果和补偿因素
    esp_adc_cal_characterize(unit, atten, width, DEFAULT_VREF, waterquality_adc_chars);
    
}

static unsigned int get_adc_value(uint8_t channel_X)
{
    unsigned int adc_value = 0;

    //读取采样值
    adc_value = adc1_get_raw(channel_X);
    //返回采样值
    return adc_value;
}

float get_waterquality_tds_value(void)
{
    int adc_max = 4095;
    int adc_new = 0;
    int Percentage_value = 0;
    float TDS_voltage = 0;
    float TDS_Value = 0;
    float temp_data=0.0;
    float compensationCoefficient=1.0;//温度校准系数
    float compensationVolatge;
    float kValue=1.0;

    adc_new = get_adc_value(WATERQUALITY_CHANNEL);
    TDS_voltage = (float) adc_new/adc_max*3.3;

    compensationCoefficient=1.0+0.02*((temp_data/10)-25.0); 
	compensationVolatge=TDS_voltage/compensationCoefficient;
	TDS_Value = (133.42*compensationVolatge*compensationVolatge*compensationVolatge - 255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5*kValue;
    
    return TDS_Value;
}
