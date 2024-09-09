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

    struct TBinaryArguments {
        binary_sensor::BinarySensor* pBinarySensor;
        std::string id;
        std::string data;
        std::string expected_response;
        uint8_t data_offset;
        uint8_t data_size;
        std::string update_entity;
        std::string set_entity;
    };
    using TBinarySensorMap = std::map<std::string, TBinaryArguments>;

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

    struct TSelectArguments {
        select::Select* pSelect;
        std::string id;
        std::string data;
        std::string expected_response;
        uint8_t data_offset;
        uint8_t data_size;
        BidiMap<uint8_t, std::string> map;
        std::array<uint16_t, 7> set;

        TSelectArguments(
            select::Select* _pSelect,
            std::string _id,
            std::string _data,
            std::string _expected_response,
            uint8_t _data_offset,
            uint8_t _data_size,
            std::string const& _map,
            std::string _set
        )
        : pSelect(_pSelect)
        , id(_id)
        , data(_data)
        , expected_response(_expected_response)
        , data_offset(_data_offset)
        , data_size(_data_size)
        , map(Utils::str_to_map(_map))
        , set(Utils::str_to_bytes_array16(_set))
        {}
    };
    using TSelectMap = std::map<std::string, TSelectArguments>;
    struct TNumberArguments {
        number::Number* pNumber;
        std::string id;
        std::string data;
        std::string expected_response;
        uint8_t data_offset;
        uint8_t data_size;
        float divider;
        std::array<uint16_t, 7> set;
        TNumberArguments(
            number::Number* _pNumber,
            std::string _id,
            std::string _data,
            std::string _expected_response,
            uint8_t _data_offset,
            uint8_t _data_size,
            float _divider,
            std::string _set
        )
        : pNumber(_pNumber)
        , id(_id)
        , data(_data)
        , expected_response(_expected_response)
        , data_offset(_data_offset)
        , data_size(_data_size)
        , divider(_divider)
        , set(Utils::str_to_bytes_array16(_set))
        {}
    };
    using TNumberMap = std::map<std::string, TNumberArguments>;
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
    void set_sensor(std::string const& name, TArguments const& arg) { m_sensors.insert({name, arg}); }

    sensor::Sensor* get_thermal_power() const { return m_thermal_power; }
    void set_thermal_power(sensor::Sensor* pSensor) { m_thermal_power = pSensor; }

    // Binary Sensors

    TBinarySensorMap const& get_binary_sensors() const { return m_binary_sensors; }
    void set_binary_sensor(std::string const& name, TBinaryArguments const& arg) { m_binary_sensors.insert({name, arg}); }

    // Text Sensors

    TTextSensorMap const& get_text_sensors() const { return m_text_sensors; }
    void set_text_sensor(std::string const& name, TTextArguments const& arg) { m_text_sensors.insert({name, arg}); }

    // Selects

    TSelectMap const&  get_selects() const { return m_selects; }
    void set_select(std::string const& name, TSelectArguments const& arg) { m_selects.insert({name, arg}); }

    // Numbers
    TNumberMap const&  get_numbers() const { return m_numbers; }
    void set_number(std::string const& name, TNumberArguments const& arg) { m_numbers.insert({name, arg}); }

private:
    text::Text* m_log_filter;
    text::Text* m_custom_request_text;

    TSensorMap m_sensors;
    TBinarySensorMap m_binary_sensors;
    TTextSensorMap m_text_sensors;
    TSelectMap m_selects;
    TNumberMap m_numbers;

    // Sensors
    sensor::Sensor* m_thermal_power;

    DaikinRotexCanComponent* m_pDaikinRotexCanComponent;
};

}
}