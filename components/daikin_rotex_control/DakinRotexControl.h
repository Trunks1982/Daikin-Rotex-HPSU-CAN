#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/esp32_can/esp32_can.h"
#include "esphome/components/canbus/canbus.h"

namespace esphome {
namespace esp32_can {
    class ESP32Can;
}

namespace dakin_rotex_control {

class DakinRotexControl: public Component {
public:

    void setup() override;
    void loop() override;
    void dump_config() override;

    void set_canbus(esphome::esp32_can::ESP32Can* pCanbus) { m_pCanbus = pCanbus; }
    void set_temperature_outside_sensor(sensor::Sensor* pSensor) { m_pTemperatureOutsideSensor = pSensor; }

protected:

    esphome::esp32_can::ESP32Can* m_pCanbus;
    sensor::Sensor* m_pTemperatureOutsideSensor;
};

} // namespace dakin_rotex_control
} // namespace esphome