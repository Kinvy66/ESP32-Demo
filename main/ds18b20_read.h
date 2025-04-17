#ifndef _DS18B20_READ_H__
#define _DS18B20_READ_H__


#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ds18b20.h"
#include "onewire_bus.h"

void ds18b20_sensor_detect(void) ;
float ds18b20_sensor_read(void);

#endif /* _DS18B20_READ_H__ */