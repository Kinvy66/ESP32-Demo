#ifndef _RAINDROP_H__
#define _RAINDROP_H__

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_rom_sys.h"
#include "esp_timer.h"
#include "driver/uart.h"
#include "rom/ets_sys.h"
#include "esp_system.h"
#include "driver/gptimer.h"
#include "esp_log.h"
#include "freertos/queue.h"
#include "driver/spi_master.h"
#include "nvs_flash.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_cali.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define DEFAULT_VREF    1100       //默认参考电压，单位mV

#define Raindrop_GPIO_AO    1
#define Raindrop_GPIO_DO    2

#define channel         ADC_CHANNEL_0               // ADC测量通道
#define width           ADC_WIDTH_BIT_12            // ADC分辨率
#define atten           ADC_ATTEN_DB_11             // ADC衰减
#define unit            ADC_UNIT_1                  // ADC1

void delay_us(unsigned int us);
void delay_ms(unsigned int ms);
void raindrop_gpio_config(void);
unsigned int get_raindrop_percentage_value(void);
#endif