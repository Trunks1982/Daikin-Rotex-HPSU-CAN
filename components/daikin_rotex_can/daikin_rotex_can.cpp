#include "esphome/components/daikin_rotex_can/daikin_rotex_can.h"
#include "esphome/components/daikin_rotex_can/request.h"
#include <string>
#include <vector>

namespace esphome {
namespace daikin_rotex_can {

static const char* TAG = "daikin_rotex_can";

DaikinRotexCanComponent::DaikinRotexCanComponent()
: m_accessor(this)
, m_data_requests()
, m_later_calls()
, m_dhw_run_lambdas()
{
}

void DaikinRotexCanComponent::validateConfig() {
    for (auto const& pair : m_accessor.get_sensors()) {
        const std::array<uint8_t, 7> data = Utils::str_to_bytes_array8(pair.second.data);
        const std::array<uint16_t, 7> expected_response = Utils::str_to_bytes_array16(pair.second.expected_response);

        /////////////////////// Sensors ///////////////////////
        m_data_requests.add({
            pair.second.id,
            data,
            expected_response,
            [pair](auto& accessor) -> EntityBase* { return pair.second.pSensor; },
            [pair, this](auto const& data, auto& accessor) -> DataType {
                float value = 0;

                if (pair.second.data_offset > 0 && (pair.second.data_offset + pair.second.data_size) <= 7) {
                    switch (pair.second.data_size) {
                    case 1:
                        value = data[pair.second.data_offset] / pair.second.divider;
                        pair.second.pSensor->publish_state(value);
                        break;
                    case 2:
                        value = ((data[pair.second.data_offset] << 8) + data[pair.second.data_offset + 1]) / pair.second.divider;
                        pair.second.pSensor->publish_state(value);
                        break;
                    default:
                        call_later([pair](){
                            ESP_LOGE("validateConfig", "Invalid data size: %d", pair.second.data_size);
                        });
                    }
                } else {
                    call_later([pair](){
                        ESP_LOGE("validateConfig", "Invalid data_offset: %d", pair.second.data_offset);
                    });
                }

                call_later([pair, this](){
                    if (!pair.second.update_entity.empty()) {
                        updateState(pair.second.update_entity);
                    }
                });

                call_later([pair, value, &accessor, this](){
                    if (!pair.second.set_entity.empty()) {
                        TRequest const* pRequest = m_data_requests.get(pair.second.set_entity);
                        if (pRequest != nullptr) {
                            EntityBase* pEntity = pRequest->getEntity(accessor);
                            if (number::Number* pNumber = dynamic_cast<number::Number*>(pEntity)) {
                                pNumber->publish_state(value);
                            } else {
                                ESP_LOGE("handle ", "id<%s>.set_entity<%s> has unsupported type!", 
                                    pair.second.id.c_str(), pair.second.set_entity.c_str());
                            }
                        } else {
                            ESP_LOGE("handle ", "id<%s>.set_entity<%s> is invalid!", 
                                pair.second.id.c_str(), pair.second.set_entity.c_str());
                        }
                    }
                });

                if (pair.second.id == "target_hot_water_temperature") {
                    call_later([&accessor](){
                        accessor.getDaikinRotexCanComponent()->run_dhw_lambdas();
                    });
                }

                return value;
            }
        });
    }

    /////////////////////// Binary Sensors ///////////////////////
    for (auto const& pair : m_accessor.get_binary_sensors()) {
        const std::array<uint8_t, 7> data = Utils::str_to_bytes_array8(pair.second.data);
        const std::array<uint16_t, 7> expected_response = Utils::str_to_bytes_array16(pair.second.expected_response);

        m_data_requests.add({
            pair.second.id,
            data,
            expected_response,
            [pair](auto& accessor) -> EntityBase* { return pair.second.pBinarySensor; },
            [pair, this](auto const& data, auto& accessor) -> DataType {
                bool value;

                if (pair.second.data_offset > 0 && (pair.second.data_offset + pair.second.data_size) <= 7) {
                    switch (pair.second.data_size) {
                    case 1: {
                        value = data[pair.second.data_offset];
                        pair.second.pBinarySensor->publish_state(value);
                        break;
                    }
                    default:
                        call_later([pair](){
                            ESP_LOGE("validateConfig", "Invalid data size: %d", pair.second.data_size);
                        });
                    }
                } else {
                    call_later([pair](){
                        ESP_LOGE("validateConfig", "Invalid data_offset: %d", pair.second.data_offset);
                    });
                }

                return value;
            }
        });
    }

    /////////////////////// Text Sensors ///////////////////////
    for (auto const& pair : m_accessor.get_text_sensors()) {
        const std::array<uint8_t, 7> data = Utils::str_to_bytes_array8(pair.second.data);
        const std::array<uint16_t, 7> expected_response = Utils::str_to_bytes_array16(pair.second.expected_response);

        m_data_requests.add({
            pair.second.id,
            data,
            expected_response,
            [pair](auto& accessor) -> EntityBase* { return pair.second.pTextSensor; },
            [pair, this](auto const& data, auto& accessor) -> DataType {
                std::string str;

                if (pair.second.data_offset > 0 && (pair.second.data_offset + pair.second.data_size) <= 7) {
                    switch (pair.second.data_size) {
                    case 1: {
                        const uint32_t value = data[pair.second.data_offset];
                        str = pair.second.map.getValue(value);

                        pair.second.pTextSensor->publish_state(str);
                        break;
                    }
                    case 2: {
                        const uint32_t value = (data[pair.second.data_offset] << 8) + data[pair.second.data_offset + 1];
                        str = pair.second.map.getValue(value);

                        pair.second.pTextSensor->publish_state(str);
                        break;
                    }
                    default:
                        call_later([pair](){
                            ESP_LOGE("validateConfig", "Invalid data size: %d", pair.second.data_size);
                        });
                    }
                } else {
                    call_later([pair](){
                        ESP_LOGE("validateConfig", "Invalid data_offset: %d", pair.second.data_offset);
                    });
                }

                call_later([pair, this](){
                    if (!pair.second.update_entity.empty()) {
                        updateState(pair.second.update_entity);
                    }
                });

                call_later([pair, str, &accessor, this](){
                    if (!pair.second.set_entity.empty()) {
                        TRequest const* pRequest = m_data_requests.get(pair.second.set_entity);
                        if (pRequest != nullptr) {
                            EntityBase* pEntity = pRequest->getEntity(accessor);
                            if (select::Select* pSelect = dynamic_cast<select::Select*>(pEntity)) {
                                //ESP_LOGE("validateConfig", "set select: %s", pair.second.set_entity.c_str());
                                pSelect->publish_state(str);
                            } else {
                                ESP_LOGE("handle ", "id<%s>.set_entity<%s> has unsupported type!", 
                                    pair.second.id.c_str(), pair.second.set_entity.c_str());
                            }
                        } else {
                            ESP_LOGE("handle ", "id<%s>.set_entity<%s> is invalid!", 
                                pair.second.id.c_str(), pair.second.set_entity.c_str());
                        }
                    }
                });

                return str;
            }
        });
    }

    /////////////////////// Selects ///////////////////////
    for (auto const& pair : m_accessor.get_selects()) {
        const std::array<uint8_t, 7> data = Utils::str_to_bytes_array8(pair.second.data);
        const std::array<uint16_t, 7> expected_response = Utils::str_to_bytes_array16(pair.second.expected_response);

        m_data_requests.add({
            pair.second.id,
            data,
            expected_response,
            [pair](auto& accessor) -> EntityBase* { return pair.second.pSelect; },
            [pair, this](auto const& data, auto& accessor) -> DataType {
                std::string str;

                if (pair.second.data_offset > 0 && (pair.second.data_offset + pair.second.data_size) <= 7) {
                    switch (pair.second.data_size) {
                    case 1: {
                        const uint32_t value = data[pair.second.data_offset];
                        str = pair.second.map.getValue(value);

                        pair.second.pSelect->publish_state(str);
                        break;
                    }
                    case 2: {
                        const uint32_t value = (data[pair.second.data_offset] << 8) + data[pair.second.data_offset + 1];
                        str = pair.second.map.getValue(value);

                        pair.second.pSelect->publish_state(str);
                        break;
                    }
                    default:
                        call_later([pair](){
                            ESP_LOGE("validateConfig", "Invalid data size: %d", pair.second.data_size);
                        });
                    }
                } else {
                    call_later([pair](){
                        ESP_LOGE("validateConfig", "Invalid data_offset: %d", pair.second.data_offset);
                    });
                }

                return str;
            },
            [pair](auto const& value) -> std::vector<uint8_t> {
                std::vector<uint8_t> data;
                uint8_t index = 0;
                for (uint16_t word : pair.second.set) {
                    data.push_back(word == esphome::daikin_rotex_can::DC ? value : static_cast<uint8_t>(word));
                }
                return data;
            }
        });

        call_later([pair](){
            ESP_LOGE("add", "select: %s", pair.second.id.c_str());
        }, 15*1000);
    }

    /////////////////////// Numbers ///////////////////////
    for (auto const& pair : m_accessor.get_numbers()) {
        const std::array<uint8_t, 7> data = Utils::str_to_bytes_array8(pair.second.data);
        const std::array<uint16_t, 7> expected_response = Utils::str_to_bytes_array16(pair.second.expected_response);

        m_data_requests.add({
            pair.second.id,
            data,
            expected_response,
            [pair](auto& accessor) -> EntityBase* { return pair.second.pNumber; },
            [pair, this](auto const& data, auto& accessor) -> DataType {
                float value;

                if (pair.second.data_offset > 0 && (pair.second.data_offset + pair.second.data_size) <= 7) {
                    switch (pair.second.data_size) {
                    case 1: {
                        value = data[pair.second.data_offset] / pair.second.divider;
                        pair.second.pNumber->publish_state(value);
                        break;
                    case 2:
                        value = ((data[pair.second.data_offset] << 8) + data[pair.second.data_offset + 1]) / pair.second.divider;
                        pair.second.pNumber->publish_state(value);
                        break;
                    }
                    default:
                        call_later([pair](){
                            ESP_LOGE("validateConfig", "Invalid data size: %d", pair.second.data_size);
                        });
                    }
                } else {
                    call_later([pair](){
                        ESP_LOGE("validateConfig", "Invalid data_offset: %d", pair.second.data_offset);
                    });
                }

                return value;
            },
            [pair](auto const& value) -> std::vector<uint8_t> {
                return Utils::replace_placeholders(pair.second.set, esphome::daikin_rotex_can::DC, static_cast<uint16_t>(value));
            }
        });

        call_later([pair](){
            ESP_LOGE("add", "select: %s", pair.second.id.c_str());
        }, 15*1000);
    }

    m_data_requests.removeInvalidRequests(m_accessor);
}

void DaikinRotexCanComponent::setup() {
    ESP_LOGI(TAG, "setup");
}

void DaikinRotexCanComponent::updateState(std::string const& id) {
    if (id == "thermal_power") {
        update_thermal_power();
    }
}

void DaikinRotexCanComponent::update_thermal_power() {
    text_sensor::TextSensor* mode_of_operating = m_data_requests.get_text_sensor(m_accessor, "mode_of_operating");
    sensor::Sensor* thermal_power = m_accessor.get_thermal_power();

    if (mode_of_operating != nullptr && thermal_power != nullptr) {
        sensor::Sensor* water_flow = m_data_requests.get_sensor(m_accessor, "water_flow");
        sensor::Sensor* tvbh = m_data_requests.get_sensor(m_accessor, "tvbh");
        sensor::Sensor* tv = m_data_requests.get_sensor(m_accessor, "tv");
        sensor::Sensor* tr = m_data_requests.get_sensor(m_accessor, "tr");

        float value = 0;
        if (mode_of_operating->state == "Warmwasserbereitung" && tv != nullptr && tr != nullptr && water_flow != nullptr) {
            value = (tv->state - tr->state) * (4.19 * water_flow->state) / 3600.0f;
        } else if (mode_of_operating->state == "Heizen" && tvbh != nullptr && tr != nullptr && water_flow != nullptr) {
            value = (tvbh->state - tr->state) * (4.19 * water_flow->state) / 3600.0f;
        } else if (mode_of_operating->state == "Kühlen" && tvbh != nullptr && tr != nullptr && water_flow != nullptr) {
            value = (tvbh->state - tr->state) * (4.19 * water_flow->state) / 3600.0f;
        }
        thermal_power->publish_state(value);
    }
}

///////////////// Texts /////////////////
void DaikinRotexCanComponent::custom_request(std::string const& value) {
    const uint32_t can_id = 0x680;
    const bool use_extended_id = false;

    const std::vector<uint8_t> buffer = Utils::str_to_bytes(value);

    if (!buffer.empty()) {
        esphome::esp32_can::ESP32Can* pCanbus = m_data_requests.getCanbus();
        pCanbus->send_data(can_id, use_extended_id, { buffer.begin(), buffer.end() });

        Utils::log("sendGet", "can_id<%s> data<%s>",
            Utils::to_hex(can_id).c_str(), value.c_str(), Utils::to_hex(buffer).c_str());
    }
}

///////////////// Selects /////////////////
void DaikinRotexCanComponent::set_generic_select(std::string const& id, std::string const& state) {
    for (auto& pair : m_accessor.get_selects()) {
        if (pair.second.id == id) {
            m_data_requests.sendSet(m_accessor, pair.second.pSelect->get_name(), pair.second.map.getKey(state));
            break;
        }
    }
}

///////////////// Numbers /////////////////
void DaikinRotexCanComponent::set_generic_number(std::string const& id, float value) {
    for (auto& pair : m_accessor.get_selects()) {
        if (pair.second.id == id) {
            m_data_requests.sendSet(m_accessor, pair.second.pSelect->get_name(), value);
            break;
        }
    }
}

///////////////// Buttons /////////////////
void DaikinRotexCanComponent::dhw_run() {
    TRequest const* pRequest = m_data_requests.get("target_hot_water_temperature");

    const float temp = pRequest->get_sensor(m_accessor)->get_raw_state();

    const std::string name = pRequest->getName(m_accessor);

    m_data_requests.sendSet(m_accessor, name, 70);

    m_dhw_run_lambdas.push_back([temp, name, this]() {
        m_data_requests.sendSet(m_accessor, name, temp);
    });
}

void DaikinRotexCanComponent::run_dhw_lambdas() {
    if (m_accessor.getDaikinRotexCanComponent() != nullptr) {
        if (!m_dhw_run_lambdas.empty()) {
            auto& lambda = m_dhw_run_lambdas.front();
            lambda();
            m_dhw_run_lambdas.pop_front();
        }
    }
}

void DaikinRotexCanComponent::loop() {
    m_data_requests.sendNextPendingGet(m_accessor);
    for (auto it = m_later_calls.begin(); it != m_later_calls.end(); ) {
        if (millis() > it->second) {
            it->first();
            it = m_later_calls.erase(it);
        } else {
            ++it;
        }
    }
}

void DaikinRotexCanComponent::handle(uint32_t can_id, std::vector<uint8_t> const& data) {
    m_data_requests.handle(m_accessor, can_id, data);
}

void DaikinRotexCanComponent::dump_config() {
    ESP_LOGCONFIG(TAG, "DaikinRotexCanComponent");
}

} // namespace daikin_rotex_can
} // namespace esphome