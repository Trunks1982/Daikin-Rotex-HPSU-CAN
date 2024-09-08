#pragma once

#include "esphome/components/select/select.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/number/number.h"
#include "esphome/components/text/text.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/daikin_rotex_can/BidiMap.h"
#include "esphome/components/daikin_rotex_can/utils.h"

namespace esphome {
namespace daikin_rotex_can {

class DaikinRotexCanComponent;

class Accessor {
    struct TArguments {
        sensor::Sensor* pSensor;
        std::string id;
        std::string data;
        std::string expected_response;
        uint8_t data_offset;
        uint8_t data_size;
        float divider;
        std::string update_entity;
        std::string set_entity;
    };
    using TSensorMap = std::map<std::string, TArguments>;

    struct TTextArguments {
        text_sensor::TextSensor* pTextSensor;
        std::string id;
        std::string data;
        std::string expected_response;
        uint8_t data_offset;
        uint8_t data_size;
        BidiMap<uint8_t, std::string> map;
        std::string update_entity;
        std::string set_entity;

        TTextArguments(
            text_sensor::TextSensor* _pTextSensor,
            std::string _id,
            std::string _data,
            std::string _expected_response,
            uint8_t _data_offset,
            uint8_t _data_size,
            std::string const& _map,
            std::string const& _update_entity,
            std::string const& _set_entity
        )
        : pTextSensor(_pTextSensor)
        , id(_id)
        , data(_data)
        , expected_response(_expected_response)
        , data_offset(_data_offset)
        , data_size(_data_size)
        , map(Utils::str_to_map(_map))
        , update_entity(_update_entity)
        , set_entity(_set_entity)
        {}
    };
    using TTextSensorMap = std::map<std::string, TTextArguments>;
public:
    Accessor(DaikinRotexCanComponent* pDaikinRotexCanComponent)
    : m_pDaikinRotexCanComponent(pDaikinRotexCanComponent) {
    }

    DaikinRotexCanComponent* getDaikinRotexCanComponent() const {
        return m_pDaikinRotexCanComponent;
    }

    // Texts

    text::Text* get_log_filter() const { return m_log_filter; }
    void set_log_filter(text::Text* pText) { m_log_filter = pText; }

    text::Text* get_custom_request_text() const { return m_custom_request_text; }
    void set_custom_request_text(text::Text* pText) { m_custom_request_text = pText; }

    // Sensors

    TSensorMap const&  get_sensors() const { return m_sensors; }
    sensor::Sensor* get_sensor(std::string const& name) const { return m_sensors.at(name).pSensor; }
    void set_sensor(std::string const& name, TArguments const& arg) { m_sensors.insert({name, arg}); }

    sensor::Sensor* get_thermal_power() const { return m_thermal_power; }
    void set_thermal_power(sensor::Sensor* pSensor) { m_thermal_power = pSensor; }

    // Text Sensors

    TTextSensorMap const&  get_text_sensors() const { return m_text_sensors; }
    void set_text_sensor(std::string const& name, TTextArguments const& arg) { m_text_sensors.insert({name, arg}); }

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

    select::Select* get_hk_function_select() const { return m_hk_function_select; }
    void set_hk_function_select(select::Select* pSelect) { m_hk_function_select = pSelect; }

    select::Select* get_sg_mode_select() const { return m_sg_mode_select; }
    void set_sg_mode_select(select::Select* pSelect) { m_sg_mode_select = pSelect; }

    select::Select* get_smart_grid_select() const { return m_smart_grid_select; }
    void set_smart_grid_select(select::Select* pSelect) { m_smart_grid_select = pSelect; }

    // Numbers

    number::Number* get_target_hot_water_temperature_set() const { return m_target_hot_water_temperature_set; }
    void set_target_hot_water_temperature_set(number::Number* pNumber) { m_target_hot_water_temperature_set = pNumber; }

    number::Number* get_target_room1_temperature_set() const { return m_target_room1_temperature_set; }
    void set_target_room1_temperature_set(number::Number* pSensor) { m_target_room1_temperature_set = pSensor; }

    number::Number* get_flow_temperature_day_set() const { return m_flow_temperature_day_set; }
    void set_flow_temperature_day_set(number::Number* pNumber) { m_flow_temperature_day_set = pNumber; }

    number::Number* get_max_target_flow_temp_set() const { return m_max_target_flow_temp_set; }
    void set_max_target_flow_temp_set(number::Number* pNumber) { m_max_target_flow_temp_set = pNumber; }

    number::Number* get_min_target_flow_temp_set() const { return m_min_target_flow_temp_set; }
    void set_min_target_flow_temp_set(number::Number* pNumber) { m_min_target_flow_temp_set = pNumber; }

    number::Number* get_heating_curve_set() const { return m_heating_curve_set; }
    void set_heating_curve_set(number::Number* pNumber) { m_heating_curve_set = pNumber; }

    number::Number* get_circulation_pump_min_set() const { return m_circulation_pump_min_set; }
    void set_circulation_pump_min_set(number::Number* pNumber) { m_circulation_pump_min_set = pNumber; }

    number::Number* get_circulation_pump_max_set() const { return m_circulation_pump_max_set; }
    void set_circulation_pump_max_set(number::Number* pNumber) { m_circulation_pump_max_set = pNumber; }

private:
    text::Text* m_log_filter;
    text::Text* m_custom_request_text;

    TSensorMap m_sensors;
    TTextSensorMap m_text_sensors;

    // Sensors
    sensor::Sensor* m_thermal_power;

    // Text Sensors
    text_sensor::TextSensor* m_error_code;

    // Binary Sensors
    binary_sensor::BinarySensor* m_status_kompressor;
    binary_sensor::BinarySensor* m_status_kesselpumpe;

    // Selects
    select::Select* m_operating_mode_select;
    select::Select* m_hk_function_select;
    select::Select* m_sg_mode_select;
    select::Select* m_smart_grid_select;

    // Numbers
    number::Number* m_target_hot_water_temperature_set;
    number::Number* m_target_room1_temperature_set;
    number::Number* m_flow_temperature_day_set;
    number::Number* m_max_target_flow_temp_set;
    number::Number* m_min_target_flow_temp_set;
    number::Number* m_heating_curve_set;
    number::Number* m_circulation_pump_min_set;
    number::Number* m_circulation_pump_max_set;

    DaikinRotexCanComponent* m_pDaikinRotexCanComponent;
};

}
}