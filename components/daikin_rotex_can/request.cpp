#include "esphome/components/daikin_rotex_can/request.h"
#include "esphome/components/daikin_rotex_can/Accessor.h"   // ?
#include "esphome/core/hal.h"

namespace esphome {
namespace daikin_rotex_can {

bool TRequest::inProgress() const {
    uint32_t mil = millis();
    return m_last_request > m_last_update && ((mil - m_last_request) < 3*1000);
}

bool TRequest::isMatch(uint32_t can_id, std::vector<uint8_t> const& responseData) const {
    if (can_id == m_response_can_id) {
        for (uint32_t index = 0; index < responseData.size(); ++index) {
            if (m_expected_reponse[index] != DC && responseData[index] != m_expected_reponse[index]) {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool TRequest::handle(Accessor& accessor, uint32_t can_id, std::vector<uint8_t> const& responseData, uint32_t timestamp) {
    if (isMatch(can_id, responseData)) {
        DataType variant = m_lambda(responseData, accessor);
        std::string value;
        if (std::holds_alternative<uint32_t>(variant)) {
            value = std::to_string(std::get<uint32_t>(variant));
        } else if (std::holds_alternative<uint8_t>(variant)) {
            value = std::to_string(std::get<uint8_t>(variant));
        } else if (std::holds_alternative<float>(variant)) {
            value = std::to_string(std::get<float>(variant));
        } else if (std::holds_alternative<std::string>(variant)) {
            value = std::get<std::string>(variant);
        } else {
            value = "Unsupported value type!";
        }
        Utils::log("request.cpp", "handled: %s<%s> can_id<%s> data<%s>",
            getName(accessor).c_str(), value.c_str(), Utils::to_hex(can_id).c_str(), Utils::to_hex(responseData).c_str());

        m_last_update = timestamp;
        return true;
    }
    return false;
}

void TRequest::sendGet(Accessor const& accessor, esphome::esp32_can::ESP32Can* pCanBus) {
    if (pCanBus == nullptr) {
        ESP_LOGE("request.cpp", "TRequest::sendGet() -> pCanbus is null!");
        return;
    }
    if (hasSendGet()) {
        const uint32_t can_id = 0x680;
        const bool use_extended_id = false;

        pCanBus->send_data(can_id, use_extended_id, { m_data.begin(), m_data.end() });
        Utils::log("request.cpp", "sendGet: %s can_id<%s> data<%s>",
            getName(accessor).c_str(), Utils::to_hex(can_id).c_str(), Utils::to_hex(m_data).c_str());

        m_last_request = millis();
    }
}

void TRequest::sendSet(Accessor const& accessor, esphome::esp32_can::ESP32Can* pCanBus, float value) {
    if (pCanBus == nullptr) {
        ESP_LOGE("request.cpp", "TRequest::sendSet() -> pCanbus is null!");
        return;
    }

    const uint32_t can_id = 0x680;
    const bool use_extended_id = false;

    auto data = m_set_lambda(value);

    pCanBus->send_data(can_id, use_extended_id, { data.begin(), data.end() });
    Utils::log("request.cpp", "sendSet: name<%s> value<%f> can_id<%s> data<%s>",
        getName(accessor).c_str(), value, Utils::to_hex(can_id).c_str(), Utils::to_hex(data).c_str());

    sendGet(accessor, pCanBus);
}

}
}