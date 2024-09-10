#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text/text.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/daikin_rotex_can/BidiMap.h"
#include "esphome/components/daikin_rotex_can/utils.h"
#include <list>

namespace esphome {
namespace daikin_rotex_can {

class DaikinRotexCanComponent;

class Accessor {
    struct TEntityArguments {
        EntityBase* pEntity;
        std::string id;
        std::string data;
        std::string expected_response;
        uint8_t data_offset;
        uint8_t data_size;
        float divider;
        BidiMap<uint8_t, std::string> map;
        std::string update_entity;
        std::string setter;
        TEntityArguments(
            EntityBase* _pEntity,
            std::string const& _id,
            std::string const& _data,
            std::string const& _expected_response,
            uint8_t _data_offset,
            uint8_t _data_size,
            float _divider,
            std::string const& _map,
            std::string const& _update_entity,
            std::string const& _setter
        )
        : pEntity(_pEntity)
        , id(_id)
        , data(_data)
        , expected_response(_expected_response)
        , data_offset(_data_offset)
        , data_size(_data_size)
        , divider(_divider)
        , map(Utils::str_to_map(_map))
        , update_entity(_update_entity)
        , setter(_setter)
        {}
    };
    using TEntityArgumentsList = std::list<TEntityArguments>;
public:
    Accessor(DaikinRotexCanComponent* pDaikinRotexCanComponent)
    : m_pDaikinRotexCanComponent(pDaikinRotexCanComponent) {
    }

    DaikinRotexCanComponent* getDaikinRotexCanComponent() const {
        return m_pDaikinRotexCanComponent;
    }

    TEntityArgumentsList const&  get_entities() const { return m_entities; }
    void set_entity(std::string const& name, TEntityArguments const& arg) { m_entities.push_back(arg); }

    // Texts

    text::Text* get_log_filter() const { return m_log_filter; }
    void set_log_filter(text::Text* pText) { m_log_filter = pText; }

    text::Text* get_custom_request_text() const { return m_custom_request_text; }
    void set_custom_request_text(text::Text* pText) { m_custom_request_text = pText; }

    // Sensors

    sensor::Sensor* get_thermal_power() const { return m_thermal_power; }
    void set_thermal_power(sensor::Sensor* pSensor) { m_thermal_power = pSensor; }

private:
    text::Text* m_log_filter;
    text::Text* m_custom_request_text;

    TEntityArgumentsList m_entities;

    // Sensors
    sensor::Sensor* m_thermal_power;

    DaikinRotexCanComponent* m_pDaikinRotexCanComponent;
};

}
}