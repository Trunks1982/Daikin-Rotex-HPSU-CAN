#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/components/select/select.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome {
namespace dakin_rotex_control {

class Accessor {
public:

    sensor::Sensor* get_temperature_outside_sensor() const;
    void set_temperature_outside_sensor(sensor::Sensor* pSensor);

    text_sensor::TextSensor* get_operation_mode_sensor() const;
    void set_operation_mode_sensor(text_sensor::TextSensor* pSensor);

    select::Select* get_operation_mode_select() const;
    void set_operation_mode_select(select::Select* pSelect);

private:
    sensor::Sensor* m_pTemperatureOutsideSensor;
    text_sensor::TextSensor* m_pOperationModeSensor;
    select::Select* m_pOperationModeSelect;
};

inline sensor::Sensor* Accessor::get_temperature_outside_sensor() const {
    return m_pTemperatureOutsideSensor;
}

inline void Accessor::set_temperature_outside_sensor(sensor::Sensor* pSensor) {
    m_pTemperatureOutsideSensor = pSensor;
}

inline text_sensor::TextSensor* Accessor::get_operation_mode_sensor() const {
    return m_pOperationModeSensor;
}

inline void Accessor::set_operation_mode_sensor(text_sensor::TextSensor* pSensor) {
    m_pOperationModeSensor = pSensor;
}

inline select::Select* Accessor::get_operation_mode_select() const {
    return m_pOperationModeSelect;
}

inline void Accessor::set_operation_mode_select(select::Select* pSelect) {
    m_pOperationModeSelect = pSelect;
}

}
}