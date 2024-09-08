#include "esphome/components/daikin_rotex_can/daikin_rotex_can.h"
#include "esphome/components/daikin_rotex_can/request.h"
#include "esphome/components/daikin_rotex_can/BidiMap.h"
#include <string>
#include <vector>

namespace esphome {
namespace daikin_rotex_can {

static const char* TAG = "daikin_rotex_can";

static const BidiMap<uint8_t, std::string> map_betriebsmodus {
    {0x01, "Bereitschaft"},
    {0x03, "Heizen"},
    {0x04, "Absenken"},
    {0x05, "Sommer"},
    {0x11, "Kühlen"},
    {0x0B, "Automatik 1"},
    {0x0C, "Automatik 2"}
};

static const BidiMap<uint8_t, std::string> map_betriebsart {
    {0x00, "Standby"},
    {0x01, "Heizen"},
    {0x02, "Kühlen"},
    {0x03, "Abtauen"},
    {0x04, "Warmwasserbereitung"}
};

static const BidiMap<uint8_t, std::string> map_hk_function {
    {0x00, "Witterungsgeführt"},
    {0x01, "Fest"}
};

static const BidiMap<uint8_t, std::string> map_sg_mode = {
    {0x00, "Aus"},
    {0x01, "SG Modus 1"},
    {0x02, "SG Modus 2"}
};

static const BidiMap<uint8_t, std::string> map_sg = {
    {0x00, "Aus"},
    {0x01, "An"}
};

const std::vector<TRequest> entity_config = {
    { // Betriebsmodus setzen
        "operating_mode_select",
        [](auto& accessor) -> EntityBase* { return accessor.get_operating_mode_select(); },
        [](auto const& value) -> std::vector<uint8_t> {
            return {0x30, 0x00, 0xFA, 0x01, 0x12, static_cast<uint8_t>(value), 0x00};
        }
    },

    { // HK Function Einstellen
        "hk_function_select",
        [](auto& accessor) -> EntityBase* { return accessor.get_hk_function_select(); },
        [](auto const& value) -> std::vector<uint8_t> {
            return { 0x30, 0x00, 0xFA, 0x01, 0x41, 0x00, static_cast<uint8_t>(value)};
        }
    },

    { // Circulation Pump Min Set
        "circulation_pump_min_set",
        [](auto& accessor) -> EntityBase* { return accessor.get_circulation_pump_min_set(); },
        [](auto const& value) -> std::vector<uint8_t> {
            const uint16_t temp = static_cast<uint16_t>(value);
            const uint8_t hi_byte = temp >> 8;
            const uint8_t lo_byte = temp & 0xFF;
            return {0x30, 0x00, 0xFA, 0x06, 0x7F, hi_byte, lo_byte};
        }
    },

    { // Circulation Pump Max Set
        "circulation_pump_max_set",
        [](auto& accessor) -> EntityBase* { return accessor.get_circulation_pump_max_set(); },
        [](auto const& value) -> std::vector<uint8_t> {
            const uint16_t temp = static_cast<uint16_t>(value);
            const uint8_t hi_byte = temp >> 8;
            const uint8_t lo_byte = temp & 0xFF;
            return {0x30, 0x00, 0xFA, 0x06, 0x7E, hi_byte, lo_byte};
        }
    },

    { // WW Einstellen
        "target_hot_water_temperature_set",
        [](auto& accessor) -> EntityBase* { return accessor.get_target_hot_water_temperature_set(); },
        [](auto const& value) -> std::vector<uint8_t> {
            const uint16_t temp = (uint16_t)(value * 10);
            const uint8_t hi_byte = temp >> 8;
            const uint8_t lo_byte = temp & 0xFF;
            return {0x30, 0x00, 0x13, hi_byte, lo_byte, 0x00, 0x00};
        }
    },

    { // T Vorlauf Tag Einstellen
        "flow_temperature_day_set",
        [](auto& accessor) -> EntityBase* { return accessor.get_flow_temperature_day_set(); },
        [](auto const& value) -> std::vector<uint8_t> {
            const uint16_t temp = (uint16_t)(value * 10);
            const uint8_t hi_byte = temp >> 8;
            const uint8_t lo_byte = temp & 0xFF;
            return {0x30, 0x00, 0xFA,  0x01, 0x29, hi_byte, lo_byte};
        }
    },

    { // Heizkurve einstellen
        "heating_curve_set",
        [](auto& accessor) -> EntityBase* { return accessor.get_heating_curve_set(); },
        [](auto const& value) -> std::vector<uint8_t> {
            const uint16_t hk = (uint16_t)(value * 100);
            const uint8_t hi_byte = hk >> 8;
            const uint8_t lo_byte = hk & 0xFF;
            return {0x30, 0x00, 0xFA, 0x01, 0x0E, hi_byte, lo_byte};
        }
    },

    { // Min VL Einstellen
        "min_target_flow_temp_set",
        [](auto& accessor) -> EntityBase* { return accessor.get_min_target_flow_temp_set(); },
        [](auto const& value) -> std::vector<uint8_t> {
            const uint16_t temp = (uint16_t)(value * 10);
            const uint8_t hi_byte = temp >> 8;
            const uint8_t lo_byte = temp & 0xFF;
            return {0x30, 0x00, 0xFA, 0x01, 0x2B, hi_byte, lo_byte};
        }
    },

    { // Max VL Einstellen
        "max_target_flow_temp_set",
        [](auto& accessor) -> EntityBase* { return accessor.get_max_target_flow_temp_set(); },
        [](auto const& value) -> std::vector<uint8_t> {
            const uint16_t temp = static_cast<uint16_t>(value * 10);
            const uint8_t hi_byte = temp >> 8;
            const uint8_t lo_byte = temp & 0xFF;
            return {0x30, 0x00, 0x28, hi_byte, lo_byte, 0x00, 0x00};
        }
    },

    {
        "sg_mode_select",
        [](auto& accessor) -> EntityBase* { return accessor.get_sg_mode_select(); },
        [](auto const& value) -> std::vector<uint8_t> {
            return {0x30, 0x00, 0xFA, 0x06, 0x94, 0x00, static_cast<uint8_t>(value)};
        }
    },

    {
        "smart_grid_select",
        [](auto& accessor) -> EntityBase* { return accessor.get_smart_grid_select(); },
        [](auto const& value) -> std::vector<uint8_t> {
            return {0x30, 0x00, 0xFA, 0x06, 0x93, 0x00, static_cast<uint8_t>(value)};
        }
    },

    { // Raumsoll 1 Einstellen
        "target_room1_temperature_set",
        [](auto& accessor) -> EntityBase* { return accessor.get_target_room1_temperature_set(); },
        [](auto const& value) -> std::vector<uint8_t> {
            const uint16_t temp = static_cast<uint16_t>(value * 10);
            const uint8_t hi_byte = temp >> 8;
            const uint8_t lo_byte = temp & 0xFF;
            return {0x30, 0x00, 0x05, hi_byte, lo_byte, 0x00, 0x00};
        }
    },
};

DaikinRotexCanComponent::DaikinRotexCanComponent()
: m_accessor(this)
, m_data_requests(std::move(entity_config))
, m_later_calls()
, m_dhw_run_lambdas()
{
}

void DaikinRotexCanComponent::validateConfig() {
    for (auto const& pair : m_accessor.get_sensors()) {
        const std::array<uint8_t, 7> data = Utils::str_to_bytes_array8(pair.second.data);
        const std::array<uint16_t, 7> expected_response = Utils::str_to_bytes_array16(pair.second.expected_response);

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

    for (auto const& pair : m_accessor.get_binary_sensors()) {
        const std::array<uint8_t, 7> data = Utils::str_to_bytes_array8(pair.second.data);
        const std::array<uint16_t, 7> expected_response = Utils::str_to_bytes_array16(pair.second.expected_response);

        m_data_requests.add({
            pair.second.id,
            data,
            pair.second.can_id,
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
void DaikinRotexCanComponent::set_operation_mode(std::string const& mode) {
    m_data_requests.sendSet(m_accessor, m_accessor.get_operating_mode_select()->get_name(), map_betriebsmodus.getKey(mode));
    m_data_requests.sendGet(m_accessor, m_data_requests.get_entity(m_accessor, "operating_mode")->get_name());
}

void DaikinRotexCanComponent::set_hk_function(std::string const& mode) {
    m_data_requests.sendSet(m_accessor, m_accessor.get_hk_function_select()->get_name(), map_hk_function.getKey(mode));
    m_data_requests.sendGet(m_accessor, m_data_requests.get_entity(m_accessor, "hk_function")->get_name());
}

void DaikinRotexCanComponent::set_sg_mode(std::string const& mode) {
    m_data_requests.sendSet(m_accessor, m_accessor.get_sg_mode_select()->get_name(), map_sg_mode.getKey(mode));
    m_data_requests.sendGet(m_accessor, m_data_requests.get_entity(m_accessor, "sg_mode")->get_name());
}

void DaikinRotexCanComponent::set_smart_grid(std::string const& mode) {
    m_data_requests.sendSet(m_accessor, m_accessor.get_smart_grid_select()->get_name(), map_sg.getKey(mode));
    m_data_requests.sendGet(m_accessor, m_data_requests.get_entity(m_accessor, "smart_grid")->get_name());
}

///////////////// Numbers /////////////////
void DaikinRotexCanComponent::set_target_hot_water_temperature(float temperature) {
    m_data_requests.sendSet(m_accessor, m_accessor.get_target_hot_water_temperature_set()->get_name(), temperature);
    TRequest const* pRequest = m_data_requests.get("target_hot_water_temperature");
    m_data_requests.sendGet(m_accessor, pRequest->getName(m_accessor));
}

void DaikinRotexCanComponent::set_target_room1_temperature(float temperature) {
    m_data_requests.sendSet(m_accessor, m_accessor.get_target_room1_temperature_set()->get_name(), temperature);
    TRequest const* pRequest = m_data_requests.get("target_room1_temperature");
    m_data_requests.sendGet(m_accessor, pRequest->getName(m_accessor));
}

void DaikinRotexCanComponent::set_flow_temperature_day(float temperature) {
    m_data_requests.sendSet(m_accessor, m_accessor.get_flow_temperature_day_set()->get_name(), temperature);
    TRequest const* pRequest = m_data_requests.get("flow_temperature_day");
    m_data_requests.sendGet(m_accessor, pRequest->getName(m_accessor));
}

void DaikinRotexCanComponent::set_max_target_flow_temp(float temperature) {
    m_data_requests.sendSet(m_accessor, m_accessor.get_max_target_flow_temp_set()->get_name(), temperature);
    TRequest const* pRequest = m_data_requests.get("max_target_supply_temperature");
    m_data_requests.sendGet(m_accessor, pRequest->getName(m_accessor));
}

void DaikinRotexCanComponent::set_min_target_flow_temp(float temperature) {
    m_data_requests.sendSet(m_accessor, m_accessor.get_min_target_flow_temp_set()->get_name(), temperature);
    TRequest const* pRequest = m_data_requests.get("min_target_supply_temperature");
    m_data_requests.sendGet(m_accessor, pRequest->getName(m_accessor));
}

void DaikinRotexCanComponent::set_heating_curve(float heating_curve) {
    m_data_requests.sendSet(m_accessor, m_accessor.get_heating_curve_set()->get_name(), heating_curve);
    TRequest const* pRequest = m_data_requests.get("heating_curve");
    m_data_requests.sendGet(m_accessor, pRequest->getName(m_accessor));
}

void DaikinRotexCanComponent::set_circulation_pump_min(uint8_t percent) {
    m_data_requests.sendSet(m_accessor, m_accessor.get_circulation_pump_min_set()->get_name(), percent);
    TRequest const* pRequest = m_data_requests.get("circulation_pump_min");
    m_data_requests.sendGet(m_accessor, pRequest->getName(m_accessor));
}

void DaikinRotexCanComponent::set_circulation_pump_max(uint8_t percent) {
    m_data_requests.sendSet(m_accessor, m_accessor.get_circulation_pump_max_set()->get_name(), percent);
    TRequest const* pRequest = m_data_requests.get("circulation_pump_max");
    m_data_requests.sendGet(m_accessor, pRequest->getName(m_accessor));
}

///////////////// Buttons /////////////////
void DaikinRotexCanComponent::dhw_run() {
    TRequest const* pRequest = m_data_requests.get("target_hot_water_temperature");

    const float temp = pRequest->get_sensor(m_accessor)->get_raw_state();

    const std::string name = pRequest->getName(m_accessor);

    m_data_requests.sendSet(m_accessor, m_accessor.get_target_hot_water_temperature_set()->get_name(), 70);
    m_data_requests.sendGet(m_accessor, name);

    m_dhw_run_lambdas.push_back([temp, name, this]() {
        m_data_requests.sendSet(m_accessor, m_accessor.get_target_hot_water_temperature_set()->get_name(), temp);
        m_data_requests.sendGet(m_accessor, name);
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