#pragma once

#include "esphome/components/daikin_rotex_can/types.h"
#include "esphome/components/daikin_rotex_can/utils.h"
#include "esphome/components/esp32_can/esp32_can.h"
#include <functional>

namespace esphome {
namespace daikin_rotex_can {

const uint16_t DC = 0xFFFF; // Don't care

class TRequest
{
public:
    using TGetLambda = std::function<DataType(std::vector<uint8_t> const&)>;
    using TSetLambda = std::function<std::vector<uint8_t>(float const&)>;
public:
    TRequest(
        std::string const& id,
        std::array<uint8_t, 7> const& data,
        std::array<uint16_t, 7> const& expected_reponse,
        EntityBase* entity,
        TGetLambda lambda,
        TSetLambda setLambda,
        bool setter,
        uint16_t update_interval)
    : m_id(id)
    , m_data(data)
    , m_expected_reponse(expected_reponse)
    , m_entity(entity)
    , m_lambda(lambda)
    , m_set_lambda(setLambda)
    , m_last_update(0u)
    , m_last_request(0u)
    , m_setter(setter)
    , m_update_interval(update_interval)
    {
    }

    std::string const& get_id() const { return m_id; }

    std::string getName() const {
        return m_entity != nullptr ? m_entity->get_name().str() : "<INVALID>";
    }

    EntityBase* getEntity() const {
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
        return m_last_update;
    }

    uint16_t get_response_canid() const {
        return m_data.size() >= 7 ? (m_data[0] & 0xF0) * 8 + (m_data[1] & 0x0F) : 0x00;
    }

    bool isMatch(uint32_t can_id, std::vector<uint8_t> const& responseData) const;
    bool handle(uint32_t can_id, std::vector<uint8_t> const& responseData, uint32_t timestamp);

    bool sendGet(esphome::esp32_can::ESP32Can* pCanBus);
    bool sendSet(esphome::esp32_can::ESP32Can* pCanBus, float value);

    bool inProgress() const;
    bool isSetter() const { return m_setter; }
    uint16_t get_update_interval() const { return m_update_interval; }

    std::string string() {
        return Utils::format(
            "TRequest<name: %s, data: %s>",
            getName().c_str(),
            Utils::to_hex(m_data).c_str()
        );
    }

private:
    std::string m_id;
    std::array<uint8_t, 7> m_data;
    std::array<uint16_t, 7> m_expected_reponse;
    EntityBase* m_entity;
    TGetLambda m_lambda;
    std::function<std::vector<uint8_t>(float const&)> m_set_lambda;
    uint32_t m_last_update;
    uint32_t m_last_request;
    bool m_setter;
    uint16_t m_update_interval;
};

}
}

