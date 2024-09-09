#pragma once

#include "esphome/components/daikin_rotex_can/types.h"
#include "esphome/components/daikin_rotex_can/utils.h"
#include "esphome/components/daikin_rotex_can/Accessor.h"
#include "esphome/components/esp32_can/esp32_can.h"
#include <functional>

namespace esphome {
namespace daikin_rotex_can {

const uint16_t DC = 0xFFFF; // Don't care

class TRequest
{
public:
    using TEntityProvider = std::function<EntityBase*(const Accessor&)>; // TODO: Remove Accessor after config moved to python!!!!!!
    using TGetLambda = std::function<DataType(std::vector<uint8_t> const&, Accessor&)>;
    using TSetLambda = std::function<std::vector<uint8_t>(float const&)>;
public:
    TRequest(
        std::string const& id,
        std::array<uint8_t, 7> const& data,
        std::array<uint16_t, 7> const& expected_reponse,
        TEntityProvider entity_provider,
        TGetLambda lambda,
        TSetLambda setLambda,
        bool setter)
    : m_id(id)
    , m_data(data)
    , m_expected_reponse(expected_reponse)
    , m_entity_provider(entity_provider)
    , m_lambda(lambda)
    , m_set_lambda(setLambda)
    , m_last_update(0u)
    , m_last_request(0u)
    , m_setter(setter)
    {
    }

    TRequest(
        std::string const& id,
        std::array<uint8_t, 7> const& data,
        std::array<uint16_t, 7> const& expected_reponse,
        TEntityProvider entity_provider,
        TGetLambda lambda)
    : TRequest(id, data, expected_reponse, entity_provider, lambda, [](float) -> std::vector<uint8_t> { return {}; }, false)
    {
    }

    TRequest(
        std::string const& id,
        std::array<uint8_t, 7> const& data,
        std::array<uint16_t, 7> const& expected_reponse,
        TEntityProvider entity_provider,
        TGetLambda lambda,
        TSetLambda setLambda)
    : TRequest(id, data, expected_reponse, entity_provider, lambda, setLambda, true)
    {
    }

    TRequest(
        std::string const& id,
        TEntityProvider entity_provider,
        TSetLambda setLambda)
    : TRequest(id, {}, {}, entity_provider, [](auto const&, Accessor&) -> DataType { return 0u; }, setLambda, true)
    {
    }

    std::string const& get_id() const { return m_id; }

    std::string getName(Accessor const& accessor) const {
        EntityBase* pEntity = m_entity_provider(accessor);
        return pEntity->get_name().str();
    }

    EntityBase* getEntity(Accessor const& accessor) const {
        return m_entity_provider(accessor);
    }

    sensor::Sensor* get_sensor(Accessor const& accessor) const {
        return static_cast<sensor::Sensor*>(m_entity_provider(accessor));
    }

    bool isGetSupported(Accessor const& accessor) const {
        return m_entity_provider(accessor) != nullptr;
    }

    uint32_t getLastUpdate() const {
        return m_last_update;
    }

    bool hasSendGet() const {
        return get_response_canid() != 0x00;
    }

    uint16_t get_response_canid() const {
        return m_data.size() >= 7 ? (m_data[0] & 0xF0) * 8 + (m_data[1] & 0x0F) : 0x00;
    }

    bool isMatch(uint32_t can_id, std::vector<uint8_t> const& responseData) const;
    bool handle(Accessor&, uint32_t can_id, std::vector<uint8_t> const& responseData, uint32_t timestamp);

    void sendGet(Accessor const& accessor, esphome::esp32_can::ESP32Can* pCanBus);
    void sendSet(Accessor const& accessor, esphome::esp32_can::ESP32Can* pCanBus, float value);

    bool inProgress() const;
    bool isSetter() const { return m_setter; }

    std::string string(Accessor const& accessor) {
        return Utils::format(
            "TRequest<name: %s, data: %s>",
            getName(accessor).c_str(),
            Utils::to_hex(m_data).c_str()
        );
    }

private:
    std::string m_id;
    std::array<uint8_t, 7> m_data;
    std::array<uint16_t, 7> m_expected_reponse;
    TEntityProvider m_entity_provider;
    TGetLambda m_lambda;
    std::function<std::vector<uint8_t>(float const&)> m_set_lambda;
    uint32_t m_last_update;
    uint32_t m_last_request;
    bool m_setter;
};

}
}

