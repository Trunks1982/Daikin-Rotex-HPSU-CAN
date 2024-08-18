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

class DakinRotexControl : public esp32_can::ESP32Can {
public:

    void setup() override;
    void loop() override;
    void dump_config() override;

    void set_canbus(esphome::esp32_can::ESP32Can* pCanbus) { m_pCanbus = pCanbus; }

protected:

    esphome::esp32_can::ESP32Can* m_pCanbus;
};

} // namespace dakin_rotex_control
} // namespace esphome