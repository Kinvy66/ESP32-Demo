#include "watersensor.h"

#define DEFAULT_VREF    1100       //默认参考电压，单位mV

#define WaterSensor_GPIO    1

#define WATERSENSOR_CHANNEL         ADC_CHANNEL_3               // ADC测量通道
#define width           ADC_WIDTH_BIT_12            // ADC分辨率
#define atten           ADC_ATTEN_DB_11             // ADC衰减
#define unit            ADC_UNIT_1                  // ADC1


esp_adc_cal_characteristics_t *watersensor_adc_chars;


void watersensor_gpio_config(void)
{
    adc1_config_width(width);// 12位分辨率

    //ADC_ATTEN_DB_0:表示参考电压为1.1V
    //ADC_ATTEN_DB_2_5:表示参考电压为1.5V
    //ADC_ATTEN_DB_6:表示参考电压为2.2V
    //ADC_ATTEN_DB_11:表示参考电压为3.3V
    adc1_config_channel_atten( WATERSENSOR_CHANNEL,atten);// 设置通道0和3.3V参考电压

    // 分配内存
    watersensor_adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    // 对 ADC 特性进行初始化，使其能够正确地计算转换结果和补偿因素
    esp_adc_cal_characterize(unit, atten, width, DEFAULT_VREF, watersensor_adc_chars);
    
}

static unsigned int get_adc_value(uint8_t channel_X)
{
    unsigned int adc_value = 0;

    //读取采样值
    adc_value = adc1_get_raw(channel_X);
    //返回采样值
    return adc_value;
}

unsigned int get_watersensor_percentage_value(void)
{
    int adc_max = 4095;
    int adc_new = 0;
    int Percentage_value = 0;

    adc_new = get_adc_value(WATERSENSOR_CHANNEL);
    
    Percentage_value = ((float)adc_new/adc_max) * 100;
    return Percentage_value;
}
