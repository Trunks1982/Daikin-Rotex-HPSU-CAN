#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/core/log.h"
#include "DakinRotexControl.h"

namespace esphome {
namespace dakin_rotex_control {

static const char *TAG = "dakin_rotex_control";

void DakinRotexControl::setup() {
    ESP_LOGI(TAG, "setup");
}

void DakinRotexControl::loop() {
    ESP_LOGI(TAG, "loop");
    m_pTemperatureOutsideSensor->publish_state(45.2);
}

void DakinRotexControl::dump_config() {
    ESP_LOGCONFIG(TAG, "DakinRotexControl");
}

} // namespace dakin_rotex_control
} // namespace esphome