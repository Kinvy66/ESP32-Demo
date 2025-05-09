#ifndef __WINDSPEED_H__
#define __WINDSPEED_H__

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

void windspeed_gpio_config(void);
float get_windspeed_value(void);

#endif /* _WINDSPEED_H__ */