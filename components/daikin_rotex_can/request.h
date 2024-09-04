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
    using TGetCondition = std::function<bool(const Accessor&)>;
    using TGetLambda = std::function<DataType(std::vector<uint8_t> const&, Accessor&)>;
    using TSetLambda = std::function<std::vector<uint8_t>(float const&)>;
public:
    TRequest(std::string const& name,
        std::array<uint8_t, 7> const& data,
        uint32_t response_can_id,
        std::array<uint16_t, 7> const& expected_reponse,
        TGetCondition get_condition,
        TGetLambda lambda,
        TSetLambda setLambda)
    : m_name(name)
    , m_data(data)
    , m_response_can_id(response_can_id)
    , m_expected_reponse(expected_reponse)
    , m_get_condition(get_condition)
    , m_lambda(lambda)
    , m_set_lambda(setLambda)
    , m_last_update(0u)
    , m_last_request(0u)
    {
    }

    TRequest(std::string const& name,
        std::array<uint8_t, 7> const& data,
        uint32_t response_can_id,
        std::array<uint16_t, 7> const& expected_reponse,
        TGetCondition get_condition,
        TGetLambda lambda)
    : TRequest(name, data, response_can_id, expected_reponse, get_condition, lambda, [](float) -> std::vector<uint8_t> { return {}; })
    {
    }

    TRequest(std::string const& name,
        std::array<uint8_t, 7> const& data,
        std::array<uint16_t, 7> const& expected_reponse,
        TGetCondition get_condition,
        TGetLambda lambda,
        TSetLambda setLambda)
    : TRequest(name, data, 0x180, expected_reponse, get_condition, lambda, setLambda)
    {
    }

    TRequest(std::string const& name,
        std::array<uint8_t, 7> const& data,
        std::array<uint16_t, 7> const& expected_reponse,
        TGetCondition get_condition,
        TGetLambda lambda)
    : TRequest(name, data, 0x180, expected_reponse, get_condition, lambda)
    {
    }

    TRequest(std::string const& name,
        TSetLambda setLambda)
    : TRequest(name, {}, 0x00, {}, [](const Accessor&) -> bool { return true; }, [](auto const&, Accessor&) -> DataType { return 0u; }, setLambda)
    {
    }

    std::string const& getName() const {
        return m_name;
    }

    bool isGetSupported(Accessor const& accessor) const {
        return m_get_condition(accessor);
    }

    uint32_t getLastUpdate() const {
        return m_last_update;
    }

    bool hasSendGet() const {
        return m_response_can_id != 0x00;
    }

    bool isMatch(uint32_t can_id, std::vector<uint8_t> const& responseData) const;
    bool handle(uint32_t can_id, std::vector<uint8_t> const& responseData, uint32_t timestamp, Accessor&);

    void sendGet(esphome::esp32_can::ESP32Can* pCanBus);
    void sendSet(esphome::esp32_can::ESP32Can* pCanBus, float value);

    bool inProgress() const;

private:
    std::string m_name;
    std::array<uint8_t, 7> m_data;
    uint32_t m_response_can_id;
    std::array<uint16_t, 7> m_expected_reponse;
    TGetCondition m_get_condition;
    TGetLambda m_lambda;
    std::function<std::vector<uint8_t>(float const&)> m_set_lambda;
    uint32_t m_last_update;
    uint32_t m_last_request;
};

}
}

