#include "esphome/components/daikin_rotex_can/request.h"
#include "esphome/components/esp32_can/esp32_can.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace daikin_rotex_can {

std::array<uint16_t, 7> TRequest::calculate_reponse(TMessage const& message) {
    const uint16_t DC = 0xFFFF;
    std::array<uint16_t, 7> response = {DC, DC, DC, DC, DC, DC, DC};
    if (message[2] == 0xFA) {   // https://github.com/crycode-de/ioBroker.canbus/blob/master/well-known-messages/configs/rotex-hpsu.md
        response[2] = message[2];
        response[3] = message[3];
        response[4] = message[4];
    } else {
        response[2] = message[2];
    }
    return response;
}


bool TRequest::isGetInProgress() const {
    uint32_t mil = millis();
    return m_last_get_timestamp > m_last_handle_timestamp && ((mil - m_last_get_timestamp) < 3*1000); // Consider 3 sek => package is lost
}

bool TRequest::isMatch(uint32_t can_id, TMessage const& responseData) const {
    const uint16_t response_can_id = (m_command[0] & 0xF0) * 8 + (m_command[1] & 0x0F);

    //if (can_id == response_can_id())
    {
        if ((responseData[0] & 0x0F) == 0x02) { // is a response
            for (uint32_t index = 0; index < responseData.size(); ++index) {
                if (m_expected_reponse[index] != DC && responseData[index] != m_expected_reponse[index]) {
                    return false;
                }
            }
            return responseData.size() == 7;
        }
    }
    return false;
}

bool TRequest::handle(uint32_t can_id, TMessage const& responseData, uint32_t timestamp) {
    if (isMatch(can_id, responseData)) {
        const TVariant variant = m_lambda(responseData);
        std::string value;
        if (std::holds_alternative<uint32_t>(variant)) {
            value = std::to_string(std::get<uint32_t>(variant));
        } else if (std::holds_alternative<uint8_t>(variant)) {
            value = std::to_string(std::get<uint8_t>(variant));
        } else if (std::holds_alternative<float>(variant)) {
            value = std::to_string(std::get<float>(variant));
        } else if (std::holds_alternative<bool>(variant)) {
            value = std::get<bool>(variant);
        } else if (std::holds_alternative<std::string>(variant)) {
            value = std::get<std::string>(variant);
        } else {
            value = "Unsupported value type!";
        }

        Utils::log("handle ", "%s<%s> can_id<%s> data<%s>",
            getName().c_str(), value.c_str(), Utils::to_hex(can_id).c_str(), Utils::to_hex(responseData).c_str());

        m_last_handle_timestamp = timestamp;
        return true;
    }
    return false;
}

bool TRequest::sendGet(esphome::esp32_can::ESP32Can* pCanBus) {
    if (pCanBus == nullptr) {
        ESP_LOGE("sendGet", "pCanbus is null!");
        return false;
    }

    const uint32_t can_id = 0x680;
    const bool use_extended_id = false;

    pCanBus->send_data(can_id, use_extended_id, { m_command.begin(), m_command.end() });

    Utils::log("sendGet", "%s can_id<%s> command<%s>",
        getName().c_str(), Utils::to_hex(can_id).c_str(), Utils::to_hex(m_command).c_str());

    m_last_get_timestamp = millis();
    return true;
}

bool TRequest::sendSet(esphome::esp32_can::ESP32Can* pCanBus, float value) {
    if (pCanBus == nullptr) {
        ESP_LOGE("sendSet", "pCanbus is null!");
        return false;
    }

    const uint32_t can_id = 0x680;
    const bool use_extended_id = false;

    auto command = m_set_lambda(value);

    pCanBus->send_data(can_id, use_extended_id, { command.begin(), command.end() });
    Utils::log("sendSet", "name<%s> value<%f> can_id<%s> command<%s>",
        getName().c_str(), value, Utils::to_hex(can_id).c_str(), Utils::to_hex(command).c_str());

    sendGet(pCanBus);

    return true;
}

}
}