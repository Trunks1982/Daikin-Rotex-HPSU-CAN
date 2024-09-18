#pragma once

#include "esphome/components/daikin_rotex_can/BidiMap.h"
#include "esphome/components/daikin_rotex_can/utils.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text/text.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include <list>

namespace esphome {
namespace daikin_rotex_can {

class DaikinRotexCanComponent;

class Accessor {
    using THandleFunc = std::function<uint16_t(TMessage const&)>;
    using TSetFunc = std::function<void(TMessage&, uint16_t)>;

public:
    struct TEntityArguments {
        EntityBase* pEntity;
        std::string id;
        TMessage command;
        uint8_t data_offset;
        uint8_t data_size;
        float divider;
        BidiMap map;
        std::string update_entity;
        uint16_t update_interval;
        THandleFunc handle_lambda;
        TSetFunc set_lambda;
        bool handle_lambda_set;
        bool set_lambda_set;

        TEntityArguments(
            EntityBase* _pEntity,
            std::string const& _id,
            std::string const& _command,
            uint8_t _data_offset,
            uint8_t _data_size,
            float _divider,
            std::string const& _map,
            std::string const& _update_entity,
            uint16_t _update_interval,
            THandleFunc _handle_lambda,
            TSetFunc _set_lambda,
            bool _handle_lambda_set,
            bool _set_lambda_set
        )
        : pEntity(_pEntity)
        , id(_id)
        , command(Utils::str_to_bytes_array8(_command))
        , data_offset(_data_offset)
        , data_size(_data_size)
        , divider(_divider)
        , map(str_to_map(_map))
        , update_entity(_update_entity)
        , update_interval(_update_interval)
        , handle_lambda(_handle_lambda)
        , set_lambda(_set_lambda)
        , handle_lambda_set(_handle_lambda_set)
        , set_lambda_set(_set_lambda_set)
        {}
    };

    using TEntityArgumentsList = std::list<TEntityArguments>;

    Accessor(DaikinRotexCanComponent* pDaikinRotexCanComponent)
    : m_log_filter(nullptr)
    , m_custom_request_text(nullptr)
    , m_thermal_power(nullptr)
    , m_pDaikinRotexCanComponent(pDaikinRotexCanComponent) {
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

    static std::map<uint16_t, std::string> str_to_map(const std::string& input);
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