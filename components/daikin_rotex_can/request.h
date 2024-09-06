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
    using TEntityProvider = std::function<EntityBase*(const Accessor&)>;
    using TGetLambda = std::function<DataType(std::vector<uint8_t> const&, Accessor&)>;
    using TSetLambda = std::function<std::vector<uint8_t>(float const&)>;
public:
    TRequest(
        std::array<uint8_t, 7> const& data,
        uint32_t response_can_id,
        std::array<uint16_t, 7> const& expected_reponse,
        TEntityProvider entity_provider,
        TGetLambda lambda,
        TSetLambda setLambda,
        bool setter)
    : m_data(data)
    , m_response_can_id(response_can_id)
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
        std::array<uint8_t, 7> const& data,
        uint32_t response_can_id,
        std::array<uint16_t, 7> const& expected_reponse,
        TEntityProvider entity_provider,
        TGetLambda lambda)
    : TRequest(data, response_can_id, expected_reponse, entity_provider, lambda, [](float) -> std::vector<uint8_t> { return {}; }, false)
    {
    }

    TRequest(
        std::array<uint8_t, 7> const& data,
        std::array<uint16_t, 7> const& expected_reponse,
        TEntityProvider entity_provider,
        TGetLambda lambda,
        TSetLambda setLambda)
    : TRequest(data, 0x180, expected_reponse, entity_provider, lambda, setLambda, true)
    {
    }

    TRequest(
        std::array<uint8_t, 7> const& data,
        std::array<uint16_t, 7> const& expected_reponse,
        TEntityProvider entity_provider,
        TGetLambda lambda)
    : TRequest(data, 0x180, expected_reponse, entity_provider, lambda)
    {
    }

    TRequest(
        TEntityProvider entity_provider,
        TSetLambda setLambda)
    : TRequest({}, 0x00, {}, entity_provider, [](auto const&, Accessor&) -> DataType { return 0u; }, setLambda, true)
    {
    }

    std::string getName(Accessor const& accessor) const {
        EntityBase* pEntity = m_entity_provider(accessor);
        return pEntity->get_name().str();
    }

    bool isGetSupported(Accessor const& accessor) const {
        return m_entity_provider(accessor) != nullptr;
    }

    uint32_t getLastUpdate() const {
        return m_last_update;
    }

    bool hasSendGet() const {
        return m_response_can_id != 0x00;
    }

    bool isMatch(uint32_t can_id, std::vector<uint8_t> const& responseData) const;
    bool handle(Accessor&, uint32_t can_id, std::vector<uint8_t> const& responseData, uint32_t timestamp);

    void sendGet(Accessor const& accessor, esphome::esp32_can::ESP32Can* pCanBus);
    void sendSet(Accessor const& accessor, esphome::esp32_can::ESP32Can* pCanBus, float value);

    bool inProgress() const;
    bool isSetter() const { return m_setter; }

private:
    std::array<uint8_t, 7> m_data;
    uint32_t m_response_can_id;
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

