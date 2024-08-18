#pragma once

#include "esphome/components/daikin_rotex_control/IPublisher.h"
#include "esphome/components/daikin_rotex_control/requests.h"
#include "esphome/components/esp32_can/esp32_can.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"

namespace esphome {
namespace dakin_rotex_control {

class DakinRotexControl: public Component, public IPublisher {
public:
    DakinRotexControl();
    void setup() override;
    void loop() override;
    void dump_config() override;
    void onPublish(std::string const& request_name, DataType const& variant) override;

    void set_canbus(esphome::esp32_can::ESP32Can* pCanbus) { m_pCanbus = pCanbus; }
    void set_temperature_outside_sensor(sensor::Sensor* pSensor) { m_pTemperatureOutsideSensor = pSensor; }

protected:

    esphome::esp32_can::ESP32Can* m_pCanbus;
    sensor::Sensor* m_pTemperatureOutsideSensor;

    TRequests m_data_requests;
};

} // namespace dakin_rotex_control
} // namespace esphome