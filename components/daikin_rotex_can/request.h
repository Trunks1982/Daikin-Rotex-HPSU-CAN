#pragma once

#include "esphome/components/daikin_rotex_can/types.h"
#include "esphome/components/daikin_rotex_can/utils.h"
#include "esphome/components/esp32_can/esp32_can.h"
#include <functional>

namespace esphome {
namespace daikin_rotex_can {

class TRequest
{
    static const uint16_t DC = 0xFFFF; // Don't care

public:
    using TVariant = std::variant<uint32_t, uint8_t, float, bool, std::string>;
    using TGetLambda = std::function<TVariant(TMessage const&)>;
    using TSetLambda = std::function<TMessage(float const&)>;
public:
    TRequest(
        std::string const& id,
        TMessage const& command,
        EntityBase* entity,
        TGetLambda lambda,
        TSetLambda setLambda,
        uint16_t update_interval)
    : m_id(id)
    , m_command(command)
    , m_expected_reponse(TRequest::calculate_reponse(command))
    , m_entity(entity)
    , m_lambda(lambda)
    , m_set_lambda(setLambda)
    , m_last_handle_timestamp(0u)
    , m_last_get_timestamp(0u)
    , m_update_interval(update_interval)
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

    bool isGetSupported() const {
        return m_entity != nullptr;
    }

    uint32_t getLastUpdate() const {
        return m_last_handle_timestamp;
    }

    bool isMatch(uint32_t can_id, TMessage const& responseData) const;
    bool handle(uint32_t can_id, TMessage const& responseData, uint32_t timestamp);

    bool sendGet(esphome::esp32_can::ESP32Can* pCanBus);
    bool sendSet(esphome::esp32_can::ESP32Can* pCanBus, float value);

    bool isGetNeeded() const;

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

private:
    std::string m_id;
    TMessage m_command;
    std::array<uint16_t, 7> m_expected_reponse;
    EntityBase* m_entity;
    TGetLambda m_lambda;
    TSetLambda m_set_lambda;
    uint32_t m_last_handle_timestamp;
    uint32_t m_last_get_timestamp;
    uint16_t m_update_interval;
};

inline bool TRequest::isGetNeeded() const {
    const uint32_t update_interval = get_update_interval() * 1000;
    return getLastUpdate() == 0 || (millis() > (getLastUpdate() + update_interval));
}

}
}

