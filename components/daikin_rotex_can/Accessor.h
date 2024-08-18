#pragma once

#include "esphome/components/select/select.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome {
namespace daikin_rotex_can {

class Accessor {
public:

    // Sensors

    sensor::Sensor* get_temperature_outside() const { return m_pTemperatureOutsideSensor; }
    void set_temperature_outside(sensor::Sensor* pSensor) { m_pTemperatureOutsideSensor = pSensor; }

    sensor::Sensor* get_tdhw1() const { return m_tdhw1; }
    void set_tdhw1(sensor::Sensor* pSensor) { m_tdhw1 = pSensor; }

    sensor::Sensor* get_tv() const { return m_tv; }
    void set_tv(sensor::Sensor* pSensor) { m_tv = pSensor; }

    sensor::Sensor* get_tvbh() const { return m_tvbh; }
    void set_tvbh(sensor::Sensor* pSensor) { m_tvbh = pSensor; }

    sensor::Sensor* get_tr() const { return m_tr; }
    void set_tr(sensor::Sensor* pSensor) { m_tr = pSensor; }

    sensor::Sensor* get_water_pressure() const { return m_water_pressure; }
    void set_water_pressure(sensor::Sensor* pSensor) { m_water_pressure = pSensor; }

    sensor::Sensor* get_water_flow() const { return m_water_flow; }
    void set_water_flow(sensor::Sensor* pSensor) { m_water_flow = pSensor; }

    sensor::Sensor* get_circulation_pump() const { return m_circulation_pump; }
    void set_circulation_pump(sensor::Sensor* pSensor) { m_circulation_pump = pSensor; }

    // Text Sensors

    text_sensor::TextSensor* get_operating_mode() const { return m_operating_mode; }
    void set_operating_mode(text_sensor::TextSensor* pSensor) { m_operating_mode = pSensor; }

    text_sensor::TextSensor* get_error_code() const { return m_error_code; }
    void set_error_code(text_sensor::TextSensor* pSensor) { m_error_code = pSensor; }

    // Binary Sensors

    binary_sensor::BinarySensor* get_status_kompressor() const { return m_status_kompressor; }
    void set_status_kompressor(binary_sensor::BinarySensor* pSensor) { m_status_kompressor = pSensor; }

    binary_sensor::BinarySensor* get_status_kesselpumpe() const { return m_status_kesselpumpe; }
    void set_status_kesselpumpe(binary_sensor::BinarySensor* pSensor) { m_status_kesselpumpe = pSensor; }

    // Selects

    select::Select* get_operating_mode_select() const { return m_operating_mode_select; }
    void set_operating_mode_select(select::Select* pSelect) { m_operating_mode_select = pSelect; }

private:
    sensor::Sensor* m_pTemperatureOutsideSensor;
    sensor::Sensor* m_tdhw1;
    sensor::Sensor* m_tv;
    sensor::Sensor* m_tvbh;
    sensor::Sensor* m_tr;
    sensor::Sensor* m_water_pressure;
    sensor::Sensor* m_water_flow;
    sensor::Sensor* m_circulation_pump;

    text_sensor::TextSensor* m_operating_mode;
    text_sensor::TextSensor* m_error_code;

    binary_sensor::BinarySensor* m_status_kompressor;
    binary_sensor::BinarySensor* m_status_kesselpumpe;

    select::Select* m_operating_mode_select;
};

}
}