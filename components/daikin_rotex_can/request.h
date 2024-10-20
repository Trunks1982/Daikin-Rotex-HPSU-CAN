#pragma once

#include "esphome/components/daikin_rotex_can/types.h"
#include "esphome/components/daikin_rotex_can/utils.h"
#include "esphome/components/daikin_rotex_can/Accessor.h"
#include "esphome/components/esp32_can/esp32_can.h"
#include "esphome/core/entity_base.h"
#include <functional>

namespace esphome {
namespace daikin_rotex_can {

class GenericSelect;

class TRequest
{
    static const uint16_t DC = 0xFFFF; // Don't care

public:
    using THandleFunc = std::function<uint16_t(TMessage const&)>;
    using TSetFunc = std::function<void(TMessage&, uint16_t)>;
    using TVariant = std::variant<uint32_t, uint8_t, float, bool, std::string>;
    using TPostHandleLabda = std::function<void(TRequest*)>;

    struct TEntityArguments {
        EntityBase* pEntity;
        std::string id;
        uint16_t can_id;
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

        TEntityArguments()
        : pEntity(nullptr)
        , id("")
        , can_id(0x0)
        , command({})
        , data_offset(0)
        , data_size(0)
        , divider(1)
        , map({})
        , update_entity({})
        , update_interval(1000)
        , handle_lambda([](TMessage const&){ return 0; })
        , set_lambda([](TMessage&, uint16_t){})
        , handle_lambda_set(false)
        , set_lambda_set(false)
        {
        }

        TEntityArguments(
            EntityBase* _pEntity,
            std::string const& _id,
            uint16_t _can_id,
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
        , can_id(_can_id)
        , command(Utils::str_to_bytes_array8(_command))
        , data_offset(_data_offset)
        , data_size(_data_size)
        , divider(_divider)
        , map(Utils::str_to_map(_map))
        , update_entity(_update_entity)
        , update_interval(_update_interval)
        , handle_lambda(_handle_lambda)
        , set_lambda(_set_lambda)
        , handle_lambda_set(_handle_lambda_set)
        , set_lambda_set(_set_lambda_set)
        {}
    };

public:
    TRequest()
    {
    }

    std::string const& get_id() const { return m_id; }

    std::string getName() const {
        return m_entity != nullptr ? m_entity->get_name().str() : "<INVALID>";
    }

    EntityBase* get_entity() const {
        return m_entity;
    }

    sensor::Sensor* get_sensor() const {
        return dynamic_cast<sensor::Sensor*>(m_entity);
    }

    number::Number* get_number() const {
        return dynamic_cast<number::Number*>(m_entity);
    }

    GenericSelect* get_select() const;

    bool isGetSupported() const {
        return m_entity != nullptr;
    }

    uint32_t getLastUpdate() const {
        return m_last_handle_timestamp;
    }

    void set_canbus(esphome::esp32_can::ESP32Can* pCanbus) {
        m_pCanbus = pCanbus;
    }

    void set_entity(std::string const& name, TEntityArguments const& arg) { 
        m_id = arg.id;
        m_can_id = arg.can_id;
        m_command = arg.command;
        m_expected_reponse = TRequest::calculate_reponse(arg.command);
        m_entity = arg.pEntity;
        m_update_interval = arg.update_interval;
        m_config = arg;
    }

    void set_post_handle(TPostHandleLabda&& func) {
        m_post_handle_lambda = std::move(func);
    }

    std::string const& get_update_entity() {
        return m_config.update_entity;
    }

    TEntityArguments const& get_config() const {
        return m_config;
    }

    bool isMatch(uint32_t can_id, TMessage const& responseData) const;
    bool handle(uint32_t can_id, TMessage const& responseData, uint32_t timestamp);

    bool sendGet(esphome::esp32_can::ESP32Can* pCanBus);
    bool sendSet(esphome::esp32_can::ESP32Can* pCanBus, float value);

    bool isGetNeeded() const;
    bool is_command_set() const;

    bool isGetInProgress() const;
    uint16_t get_update_interval() const { return m_update_interval; }

    static std::array<uint16_t, 7> calculate_reponse(TMessage const& message);

    std::string string() {
        return Utils::format(
            "TRequest<name: %s, command: %s>",
            getName().c_str(),
            Utils::to_hex(m_command).c_str()
        );
    }

protected:
    virtual TVariant handleValue(uint16_t value) = 0;

protected:
    TEntityArguments m_config;
    esphome::esp32_can::ESP32Can* m_pCanbus;

private:
    std::string m_id;
    uint16_t m_can_id;
    TMessage m_command;
    std::array<uint16_t, 7> m_expected_reponse;
    EntityBase* m_entity;
    uint32_t m_last_handle_timestamp;
    uint32_t m_last_get_timestamp;
    uint16_t m_update_interval;
    TPostHandleLabda m_post_handle_lambda;
};

inline bool TRequest::isGetNeeded() const {
    const uint32_t update_interval = get_update_interval() * 1000;
    return getLastUpdate() == 0 || (millis() > (getLastUpdate() + update_interval));
}

inline bool TRequest::is_command_set() const {
    for (auto& b : m_command) {
        if (b != 0x00) {
            return true;
        }
    }
    return false;
}

}
}

