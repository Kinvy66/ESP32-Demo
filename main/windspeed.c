#include "windspeed.h"

#define DEFAULT_VREF    1100       //默认参考电压，单位mV

#define WindSpeed_GPIO           6

#define WINSPEED_CHANNEL            ADC_CHANNEL_5               // ADC测量通道
#define width                       ADC_WIDTH_BIT_12            // ADC分辨率
#define atten                       ADC_ATTEN_DB_11             // ADC衰减
#define unit                        ADC_UNIT_1                  // ADC1


esp_adc_cal_characteristics_t *windspeed_adc_chars;


void windspeed_gpio_config(void)
{
    adc1_config_width(width);// 12位分辨率

    //ADC_ATTEN_DB_0:表示参考电压为1.1V
    //ADC_ATTEN_DB_2_5:表示参考电压为1.5V
    //ADC_ATTEN_DB_6:表示参考电压为2.2V
    //ADC_ATTEN_DB_11:表示参考电压为3.3V
    adc1_config_channel_atten( WINSPEED_CHANNEL,atten);// 设置通道0和3.3V参考电压

    // 分配内存
    windspeed_adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    // 对 ADC 特性进行初始化，使其能够正确地计算转换结果和补偿因素
    esp_adc_cal_characterize(unit, atten, width, DEFAULT_VREF, windspeed_adc_chars);
    
}

static unsigned int get_adc_value(uint8_t channel_X)
{
    unsigned int adc_value = 0;

    //读取采样值
    adc_value = adc1_get_raw(channel_X);
    //返回采样值
    return adc_value;
}

static long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
   

float get_windspeed_value(void)
{
    int adc_max = 4095;
    int adc_new = 0;
    float windspeed_value = 0;
    int vol = 0;

    // for (int i = 0; i < 5; i++)
    // {
    //     adc_new += get_adc_value(WINSPEED_CHANNEL);
    //     vTaskDelay(5 / portTICK_PERIOD_MS);
    // }
    // adc_new = adc_new / 5; // 取平均值

    adc_new = get_adc_value(WINSPEED_CHANNEL);


        if(adc_new<=3)adc_new=0;	//滤除干扰
		vol = map(adc_new, 0, 4095, 0, 5000);	//换算电压mV
		windspeed_value = 0.027*vol;				//计算风速

    return windspeed_value;
}
