#pragma once

#include "esphome/components/select/select.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome {
namespace dakin_rotex_control {

class Accessor {
public:

    // Sensors

    sensor::Sensor* get_temperature_outside_sensor() const { return m_pTemperatureOutsideSensor; }
    void set_temperature_outside_sensor(sensor::Sensor* pSensor) { m_pTemperatureOutsideSensor = pSensor; }

    sensor::Sensor* get_tdhw1() const { return m_tdhw1; }
    void set_tdhw1(sensor::Sensor* pSensor) { m_tdhw1 = pSensor; }

    sensor::Sensor* get_water_pressure() const { return m_water_pressure; }
    void set_water_pressure(sensor::Sensor* pSensor) { m_water_pressure = pSensor; }

    // Text Sensors

    text_sensor::TextSensor* get_operation_mode_sensor() const { return m_pOperationModeSensor; }
    void set_operation_mode_sensor(text_sensor::TextSensor* pSensor) { m_pOperationModeSensor = pSensor; }

    // Selects

    select::Select* get_operation_mode_select() const { return m_pOperationModeSelect; }
    void set_operation_mode_select(select::Select* pSelect) { m_pOperationModeSelect = pSelect; }

private:
    sensor::Sensor* m_pTemperatureOutsideSensor;
    sensor::Sensor* m_tdhw1;
    sensor::Sensor* m_water_pressure;

    text_sensor::TextSensor* m_pOperationModeSensor;

    select::Select* m_pOperationModeSelect;
};

}
}