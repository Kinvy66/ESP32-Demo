#include "raindrop.h"

esp_adc_cal_characteristics_t *adc_chars;

void delay_ms(unsigned int ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}
void delay_us(unsigned int us)
{
    ets_delay_us(us);
}
/******************************************************************
 * 函 数 名 称：raindrop_gpio_config
 * 函 数 说 明：初始化雨滴传感器引脚
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：无
******************************************************************/
void raindrop_gpio_config(void)
{
    gpio_config_t Raindrop_config = {
        .pin_bit_mask = (1ULL<<Raindrop_GPIO_DO),    //配置引脚
        .mode =GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,        //不使能上拉
        .pull_down_en = GPIO_PULLDOWN_DISABLE,    //不使能下拉
        .intr_type = GPIO_INTR_DISABLE            //不使能引脚中断
    };
    gpio_config(&Raindrop_config);

    adc1_config_width(width);// 12位分辨率

    //ADC_ATTEN_DB_0:表示参考电压为1.1V
    //ADC_ATTEN_DB_2_5:表示参考电压为1.5V
    //ADC_ATTEN_DB_6:表示参考电压为2.2V
    //ADC_ATTEN_DB_11:表示参考电压为3.3V
    adc1_config_channel_atten( channel,atten);// 设置通道0和3.3V参考电压

    // 分配内存
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    // 对 ADC 特性进行初始化，使其能够正确地计算转换结果和补偿因素
    esp_adc_cal_characterize(unit, atten, width, DEFAULT_VREF, adc_chars);
}

/**********************************************************
 * 函 数 名 称：get_adc_value
 * 函 数 功 能：读取ADC值
 * 传 入 参 数：要采集的通道
 * 函 数 返 回：测量到的值
 * 作       者：LC
 * 备       注：无
**********************************************************/
static unsigned int get_adc_value(uint8_t channel_X)
{
    unsigned int adc_value = 0;

    //读取采样值
    adc_value = adc1_get_raw(channel_X);
    //返回采样值
    return adc_value;
}

/******************************************************************
 * 函 数 名 称：get_raindrop_percentage_value
 * 函 数 说 明：读取雨滴AO值，并且返回百分比
 * 函 数 形 参：无
 * 函 数 返 回：返回百分比
 * 作       者：LC
 * 备       注：无
******************************************************************/
unsigned int get_raindrop_percentage_value(void)
{
    int adc_max = 4095;
    int adc_new = 0;
    int Percentage_value = 0;

    adc_new = get_adc_value( channel );

    Percentage_value = (1-((float)adc_new/adc_max)) * 100;
    return Percentage_value;
}

/******************************************************************
 * 函 数 名 称：get_raindrop_do_value
 * 函 数 说 明：读取雨滴DO值，返回0或者1
 * 函 数 形 参：无
 * 函 数 返 回：
 * 作       者：LC
 * 备       注：无
******************************************************************/
unsigned char get_raindrop_do_value(void)
{
    return gpio_get_level(Raindrop_GPIO_DO);
}