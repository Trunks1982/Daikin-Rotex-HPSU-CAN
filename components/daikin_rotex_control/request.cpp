#include "esphome/components/daikin_rotex_control/request.h"

namespace esphome {
namespace dakin_rotex_control {

void TRequest::sendGet(esphome::esp32_can::ESP32Can* pCanBus) {
    if (pCanBus == nullptr) {
        ESP_LOGE("request.h", "TRequest::sendGet() -> pCanbus is null!");
        return;
    }
    if (hasSendGet()) {
        const uint32_t can_id = 0x680;
        const bool use_extended_id = false;

        pCanBus->send_data(can_id, use_extended_id, { m_data.begin(), m_data.end() });
        Utils::log("request.h", "sendGet: name<%s> can_id<%s> data<%s>",
            m_name.c_str(), Utils::to_hex(can_id).c_str(), Utils::to_hex(m_data).c_str());

        m_last_request = 0;//millis();
    }
}

void TRequest::sendSet(esphome::esp32_can::ESP32Can* pCanBus, float value) {
    if (pCanBus == nullptr) {
        ESP_LOGE("request.h", "TRequest::sendSet() -> pCanbus is null!");
        return;
    }

    const uint32_t can_id = 0x680;
    const bool use_extended_id = false;

    auto data = m_set_lambda(value);

    pCanBus->send_data(can_id, use_extended_id, { data.begin(), data.end() });
    Utils::log("request.h", "sendSet: name<%s> value<%f> can_id<%s> data<%s>",
        m_name.c_str(), value, Utils::to_hex(can_id).c_str(), Utils::to_hex(data).c_str());

    sendGet(pCanBus);
}

}
}