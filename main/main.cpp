#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "driver/rmt_tx.h"
#include "ws2812_control.h"
#include "dht.h"

#include <esp_netif.h>
#include <esp_wifi.h>
#include <nvs_flash.h>
#include <esp_random.h>
#include <Espressif_MQTT_Client.h>
#include <ThingsBoard.h>


#define WIFI_SSID "CMCC-3VQP"
#define WIFI_PASSWORD "PDHP4MCD"
#define TOKEN "EUB9awCOjfBa9Cfak6dd"
#define THINGSBOARD_SERVER "192.168.1.4"
#define THINGSBOARD_PORT 1883U

#define LED_GPIO  GPIO_NUM_14

constexpr char TEMPERATURE_KEY[] = "temperature";
constexpr char HUMIDITY_KEY[] = "humidity";

constexpr uint16_t MAX_MESSAGE_SEND_SIZE = 128U;
constexpr uint16_t MAX_MESSAGE_RECEIVE_SIZE = 128U;



// Initalize the Mqtt client instance
Espressif_MQTT_Client<> mqttClient;
// Initialize ThingsBoard instance with the maximum needed buffer size
ThingsBoard tb(mqttClient, MAX_MESSAGE_RECEIVE_SIZE, MAX_MESSAGE_SEND_SIZE);

// Status for successfully connecting to the given WiFi
bool wifi_connected = false;

void on_got_ip(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    wifi_connected = true;
}


/// @brief Initalizes WiFi connection,
// will endlessly delay until a connection has been successfully established
void InitWiFi() {
    const wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
  
    esp_netif_config_t netif_config = ESP_NETIF_DEFAULT_WIFI_STA();
    esp_netif_t *netif = esp_netif_new(&netif_config);
    assert(netif);
  
    ESP_ERROR_CHECK(esp_netif_attach_wifi_station(netif));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, ip_event_t::IP_EVENT_STA_GOT_IP, &on_got_ip, NULL));
    ESP_ERROR_CHECK(esp_wifi_set_default_wifi_sta_handlers());
    ESP_ERROR_CHECK(esp_wifi_set_storage(wifi_storage_t::WIFI_STORAGE_RAM));
  
    wifi_config_t wifi_config;
    memset(&wifi_config, 0, sizeof(wifi_config));
    strncpy(reinterpret_cast<char*>(wifi_config.sta.ssid), WIFI_SSID, strlen(WIFI_SSID) + 1);
    strncpy(reinterpret_cast<char*>(wifi_config.sta.password), WIFI_PASSWORD, strlen(WIFI_PASSWORD) + 1);
  
    ESP_LOGI("MAIN", "Connecting to %s...", wifi_config.sta.ssid);
    ESP_ERROR_CHECK(esp_wifi_set_mode(wifi_mode_t::WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(wifi_interface_t::WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_connect());
  }


static const char *TAG = "example_main";


#ifdef __cplusplus
extern "C" {
#endif
void app_main(void)
{
//     // Configure the GPIO pin for output
//     gpio_config_t led_conf;
//     led_conf.intr_type = GPIO_INTR_DISABLE;
//     led_conf.mode = GPIO_MODE_OUTPUT;
//     led_conf.pin_bit_mask = (1ULL << LED_GPIO);
//     led_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
//     led_conf.pull_up_en = GPIO_PULLUP_DISABLE;
//     gpio_config(&led_conf);

  

//     //  创建一个WS2812灯带
//      ws2812_strip_t* WS2812=ws2812_create();
    
//      //点亮灯带 颜色(255,0,0)
//      led_set_on(WS2812,COLOR_RGB(203, 20, 10));
    

     // Determine sensor type based on Kconfig
     dht_sensor_type_t sensor_type;
 #if CONFIG_EXAMPLE_TYPE_DHT11
     sensor_type = DHT_TYPE_DHT11;
 #elif CONFIG_EXAMPLE_TYPE_AM2301
     sensor_type = DHT_TYPE_AM2301;
 #elif CONFIG_EXAMPLE_TYPE_SI7021
     sensor_type = DHT_TYPE_SI7021;
 #else
     #error "No sensor type selected in Kconfig"
 #endif

     gpio_num_t gpio_num = (gpio_num_t)CONFIG_ESP_TEMP_SENSOR_GPIO; // GPIO number from Kconfig

     // Enable internal pull-up resistor if specified in Kconfig
    //  if (CONFIG_EXAMPLE_INTERNAL_PULLUP) {
        //  gpio_pullup_en(gpio_num);
    //  } else {
         gpio_pullup_dis(gpio_num);
    //  }

    //  while (true)
    //  {
    //      float humidity = 0, temperature = 0;

    //      esp_err_t result = dht_read_float_data(sensor_type, gpio_num, &humidity, &temperature);
    //      if (result == ESP_OK)
    //      {
    //          ESP_LOGI(TAG, "Humidity: %.1f%% Temperature: %.1f°C", humidity, temperature);
    //      }
    //      else
    //      {
    //          ESP_LOGE(TAG, "Failed to read sensor data: %s", esp_err_to_name(result));
    //      }

    //      vTaskDelay(pdMS_TO_TICKS(2000)); // Delay for 2 seconds
    //  }

    ESP_LOGI("MAIN", "[APP] Startup..");
    ESP_LOGI("MAIN", "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    ESP_LOGI("MAIN", "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    InitWiFi();

    #if ENCRYPTED
    mqttClient.set_server_certificate(ROOT_CERT);
    #endif // ENCRYPTED

    for (;;) {
        // Wait until we connected to WiFi
        if (!wifi_connected) {
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        if (!tb.connected()) {
            tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT);
        }

        float humidity = 0, temperature = 0;

        esp_err_t result = dht_read_float_data(sensor_type, gpio_num, &humidity, &temperature);
        if (result == ESP_OK)
        {
            ESP_LOGI(TAG, "Humidity: %.1f%% Temperature: %.1f°C", humidity, temperature);
        }
        else
        {
            ESP_LOGE(TAG, "Failed to read sensor data: %s", esp_err_to_name(result));
        }

        tb.sendTelemetryData(TEMPERATURE_KEY, temperature);
        tb.sendTelemetryData(HUMIDITY_KEY, humidity);

        tb.loop();


        vTaskDelay(pdMS_TO_TICKS(3000)); // Delay for 2 seconds

    }

}

#ifdef __cplusplus
}
#endif
