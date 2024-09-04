#pragma once

#include "esphome/components/select/select.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/number/number.h"
#include "esphome/components/text/text.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome {
namespace daikin_rotex_can {

class Accessor {
public:

    // Texts

    text::Text* get_log_filter() const { return m_log_filter; }
    void set_log_filter(text::Text* pText) { m_log_filter = pText; }

    // Sensors

    sensor::Sensor* get_temperature_outside() const { return m_pTemperatureOutsideSensor; }
    void set_temperature_outside(sensor::Sensor* pSensor) { m_pTemperatureOutsideSensor = pSensor; }

    sensor::Sensor* get_tdhw1() const { return m_tdhw1; }
    void set_tdhw1(sensor::Sensor* pSensor) { m_tdhw1 = pSensor; }

    sensor::Sensor* get_target_hot_water_temperature() const { return m_target_hot_water_temperature; }
    void set_target_hot_water_temperature(sensor::Sensor* pSensor) { m_target_hot_water_temperature = pSensor; }

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

    sensor::Sensor* get_bypass_valve() const { return m_bypass_valve; }
    void set_bypass_valve(sensor::Sensor* pSensor) { m_bypass_valve = pSensor; }

    sensor::Sensor* get_dhw_mixer_position() const { return m_dhw_mixer_position; }
    void set_dhw_mixer_position(sensor::Sensor* pSensor) { m_dhw_mixer_position = pSensor; }

    sensor::Sensor* get_target_supply_temperature() const { return m_target_supply_temperature; }
    void set_target_supply_temperature(sensor::Sensor* pSensor) { m_target_supply_temperature = pSensor; }

    sensor::Sensor* get_daytime_supply_temperature() const { return m_daytime_supply_temperature; }
    void set_daytime_supply_temperature(sensor::Sensor* pSensor) { m_daytime_supply_temperature = pSensor; }

    sensor::Sensor* get_thermal_power() const { return m_thermal_power; }
    void set_thermal_power(sensor::Sensor* pSensor) { m_thermal_power = pSensor; }

    sensor::Sensor* get_heating_curve() const { return m_heating_curve; }
    void set_heating_curve(sensor::Sensor* pSensor) { m_heating_curve = pSensor; }

    sensor::Sensor* get_ehs_for_ch() const { return m_ehs_for_ch; }
    void set_ehs_for_ch(sensor::Sensor* pSensor) { m_ehs_for_ch = pSensor; }

    // Text Sensors

    text_sensor::TextSensor* get_operating_mode() const { return m_operating_mode; }
    void set_operating_mode(text_sensor::TextSensor* pSensor) { m_operating_mode = pSensor; }

    text_sensor::TextSensor* get_mode_of_operating() const { return m_mode_of_operating; }
    void set_mode_of_operating(text_sensor::TextSensor* pSensor) { m_mode_of_operating = pSensor; }

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

    // Numbers

    number::Number* get_target_hot_water_temperature_set() const { return m_target_hot_water_temperature_set; }
    void set_target_hot_water_temperature_set(number::Number* pNumber) { m_target_hot_water_temperature_set = pNumber; }

public:
    void update_thermal_power() {
        float power = 0.0f;
        if (get_mode_of_operating() != nullptr && get_water_flow() != nullptr && get_tv() != nullptr && get_tvbh() != nullptr && get_tr() != nullptr) {
            if (get_mode_of_operating()->state == "Warmwasserbereitung") {
                power = (get_tv()->state - get_tr()->state) * (4.19 * get_water_flow()->state) / 3600.0f;
            } else if (get_mode_of_operating()->state == "Heizen") {
                power = (get_tvbh()->state - get_tr()->state) * (4.19 * get_water_flow()->state) / 3600.0f;
            } else if (get_mode_of_operating()->state == "Kühlen") {
                power = (get_tvbh()->state - get_tr()->state) * (4.19 * get_water_flow()->state) / 3600.0f;
            }
        }

        get_thermal_power()->publish_state(power);
    }

private:
    text::Text* m_log_filter;

    sensor::Sensor* m_pTemperatureOutsideSensor;
    sensor::Sensor* m_tdhw1;
    sensor::Sensor* m_target_hot_water_temperature;
    sensor::Sensor* m_tv;
    sensor::Sensor* m_tvbh;
    sensor::Sensor* m_tr;
    sensor::Sensor* m_water_pressure;
    sensor::Sensor* m_water_flow;
    sensor::Sensor* m_circulation_pump;
    sensor::Sensor* m_bypass_valve;
    sensor::Sensor* m_dhw_mixer_position;
    sensor::Sensor* m_target_supply_temperature;
    sensor::Sensor* m_daytime_supply_temperature;
    sensor::Sensor* m_thermal_power;
    sensor::Sensor* m_heating_curve;
    sensor::Sensor* m_ehs_for_ch;

    text_sensor::TextSensor* m_operating_mode;
    text_sensor::TextSensor* m_mode_of_operating;
    text_sensor::TextSensor* m_error_code;

    binary_sensor::BinarySensor* m_status_kompressor;
    binary_sensor::BinarySensor* m_status_kesselpumpe;

    select::Select* m_operating_mode_select;

    number::Number* m_target_hot_water_temperature_set;
};

}
}