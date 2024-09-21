#include "esphome/components/daikin_rotex_can/daikin_rotex_can.h"
#include "esphome/components/daikin_rotex_can/request.h"
#include <string>
#include <vector>

namespace esphome {
namespace daikin_rotex_can {

static const char* TAG = "daikin_rotex_can";
static const char* MODE_OF_OPERATING = "mode_of_operating";         // Betriebsart
static const char* OPERATING_MODE = "operating_mode";               // Betriebsmodus
static const char* OPTIMIZED_DEFROSTING = "optimized_defrosting";
static const uint32_t POST_SETUP_TIMOUT = 15*1000;


DaikinRotexCanComponent::DaikinRotexCanComponent()
: m_accessor(this)
, m_data_requests()
, m_later_calls()
, m_dhw_run_lambdas()
, m_optimized_defrosting(false)
, m_optimized_defrosting_pref()
, m_project_git_hash_sensor(nullptr)
, m_project_git_hash()
{
}

void DaikinRotexCanComponent::setup() {
    ESP_LOGI(TAG, "setup");

    select::Select* p_optimized_defrosting = nullptr;

    for (auto const& entity_conf : m_accessor.get_entities()) {
        const bool command_set = is_command_set(entity_conf.command);

        call_later([entity_conf, command_set](){
            ESP_LOGI("setup", "name: %s, id: %s, can_id: %s, command: %s, command_set: %d",
                entity_conf.pEntity->get_name().c_str(), entity_conf.id.c_str(),
                Utils::to_hex(entity_conf.can_id).c_str(), Utils::to_hex(entity_conf.command).c_str(), command_set);
        }, POST_SETUP_TIMOUT);

        if (entity_conf.id == OPTIMIZED_DEFROSTING) {
            p_optimized_defrosting = dynamic_cast<select::Select*>(entity_conf.pEntity);
        }

        if (!command_set) {
            continue;
        }

        m_data_requests.add({
            entity_conf.id,
            entity_conf.can_id,
            entity_conf.command,
            entity_conf.pEntity,
            [entity_conf, this](auto const& data) -> TRequest::TVariant {
                TRequest::TVariant variant;

                if (entity_conf.data_offset > 0 && (entity_conf.data_offset + entity_conf.data_size) <= 7) {
                    if (entity_conf.data_size >= 1 && entity_conf.data_size <= 2) {
                        const uint16_t value = entity_conf.handle_lambda_set ? entity_conf.handle_lambda(data) :
                                (
                                    entity_conf.data_size == 2 ?
                                    (((data[entity_conf.data_offset] << 8) + data[entity_conf.data_offset + 1])) :
                                    (data[entity_conf.data_offset])
                                );

                        if (dynamic_cast<sensor::Sensor*>(entity_conf.pEntity) != nullptr) {
                            //variant = static_cast<int16_t>(value) / entity_conf.divider;
                            variant = value / entity_conf.divider;
                            Utils::toSensor(entity_conf.pEntity)->publish_state(std::get<float>(variant));
                        } else if (dynamic_cast<binary_sensor::BinarySensor*>(entity_conf.pEntity) != nullptr) {
                            variant = value > 0;
                            Utils::toBinarySensor(entity_conf.pEntity)->publish_state(std::get<bool>(variant));
                        } else if (dynamic_cast<text_sensor::TextSensor*>(entity_conf.pEntity) != nullptr) {
                            auto it = entity_conf.map.findByKey(value);
                            const std::string str = it != entity_conf.map.end() ? it->second : Utils::format("INVALID<%f>", value);
                            Utils::toTextSensor(entity_conf.pEntity)->publish_state(str);
                            variant = str;
                        } else if (dynamic_cast<select::Select*>(entity_conf.pEntity) != nullptr) {
                            auto it = entity_conf.map.findNextByKey(value);
                            const std::string str = it != entity_conf.map.end() ? it->second : Utils::format("INVALID<%f>", value);
                            Utils::toSelect(entity_conf.pEntity)->publish_state(str);
                            variant = str;
                        } else if (dynamic_cast<number::Number*>(entity_conf.pEntity) != nullptr) {
                            //variant = static_cast<int16_t>(value) / entity_conf.divider;
                            variant = value / entity_conf.divider;
                            Utils::toNumber(entity_conf.pEntity)->publish_state(std::get<float>(variant));
                        }
                    } else {
                        call_later([entity_conf](){
                            ESP_LOGE("validateConfig", "Invalid data size: %d", entity_conf.data_size);
                        });
                    }
                } else {
                    call_later([entity_conf](){
                        ESP_LOGE("validateConfig", "Invalid data_offset: %d", entity_conf.data_offset);
                    });
                }

                if (!entity_conf.update_entity.empty()) {
                    call_later([entity_conf, this](){
                        updateState(entity_conf.update_entity);
                    });
                }

                if (entity_conf.id == "target_hot_water_temperature") {
                    call_later([this](){
                        run_dhw_lambdas();
                    });
                } else if (entity_conf.id == MODE_OF_OPERATING) {
                    call_later([this](){
                        on_mode_of_operating();
                    });
                }

                return variant;
            },
            [&entity_conf](float const& value) -> TMessage {
                TMessage message = TMessage(entity_conf.command);
                message[0] = 0x30;
                message[1] = 0x00;
                if (entity_conf.set_lambda_set) {
                    entity_conf.set_lambda(message, value);
                } else {
                    Utils::setBytes(message, value, entity_conf.data_offset, entity_conf.data_size);
                    //const int16_t ivalue = static_cast<int16_t>(value); // converte negative values. E.g. -15 to 0xFF6A
                    //const uint16_t uvalue = static_cast<int16_t>(ivalue);
                    //Utils::setBytes(message, uvalue, entity_conf.data_offset, entity_conf.data_size);
                }
                return message;
            },
            entity_conf.update_interval
        });
    }

    m_data_requests.removeInvalidRequests();
    const uint32_t size = m_data_requests.size();

    call_later([size](){
        ESP_LOGI("setup", "resquests.size: %d", size);
    }, POST_SETUP_TIMOUT);

    if (p_optimized_defrosting != nullptr) {
        m_optimized_defrosting_pref = global_preferences->make_preference<bool>(p_optimized_defrosting->get_object_id_hash());
        if (!m_optimized_defrosting_pref.load(&m_optimized_defrosting)) {
            m_optimized_defrosting = false;
        }

        auto const* pEntityConf = get_select_entity_conf(OPTIMIZED_DEFROSTING);
        if (pEntityConf != nullptr) {
            auto it = pEntityConf->map.findByKey(m_optimized_defrosting);
            if (it != pEntityConf->map.end()) {
                p_optimized_defrosting->publish_state(it->second);
            }
        }
    }

    m_project_git_hash_sensor->publish_state(m_project_git_hash);
}

void DaikinRotexCanComponent::updateState(std::string const& id) {
    if (id == "thermal_power") {
        update_thermal_power();
    }
}

void DaikinRotexCanComponent::update_thermal_power() {
    text_sensor::TextSensor* mode_of_operating = m_data_requests.get_text_sensor(MODE_OF_OPERATING);
    sensor::Sensor* thermal_power = m_accessor.get_thermal_power();

    if (mode_of_operating != nullptr && thermal_power != nullptr) {
        sensor::Sensor* flow_rate = m_data_requests.get_sensor("flow_rate");
        if (flow_rate != nullptr) {
            sensor::Sensor* tvbh = m_data_requests.get_sensor("tvbh");
            sensor::Sensor* tv = m_data_requests.get_sensor("tv");
            sensor::Sensor* tr = m_data_requests.get_sensor("tr");

            float value = 0;
            if (mode_of_operating->state == "Warmwasserbereitung" && tv != nullptr && tr != nullptr) {
                value = (tv->state - tr->state) * (4.19 * flow_rate->state) / 3600.0f;
            } else if ((mode_of_operating->state == "Heizen" || mode_of_operating->state == "Kühlen") && tvbh != nullptr && tr != nullptr) {
                value = (tvbh->state - tr->state) * (4.19 * flow_rate->state) / 3600.0f;
            }
            thermal_power->publish_state(value);
        }
    }
}

bool DaikinRotexCanComponent::on_custom_select(std::string const& id, uint8_t value) {
    if (id == OPTIMIZED_DEFROSTING) {
        Utils::log(TAG, "optimized_defrosting: %d", value);
        m_optimized_defrosting = value;
        m_optimized_defrosting_pref.save(&m_optimized_defrosting);
        return true;
    }
    return false;
}

void DaikinRotexCanComponent::on_mode_of_operating() {
    select::Select* p_operating_mode = m_data_requests.get_select(OPERATING_MODE);
    text_sensor::TextSensor* p_mode_of_operating = m_data_requests.get_text_sensor(MODE_OF_OPERATING);
    if (m_optimized_defrosting && p_operating_mode != nullptr && p_mode_of_operating != nullptr) {
        if (p_mode_of_operating->state == "Abtauen") {
            m_data_requests.sendSet(p_operating_mode->get_name(), 0x05); // Sommer
        } else if (p_mode_of_operating->state == "Heizen" && p_operating_mode->state == "Sommer") {
            m_data_requests.sendSet(p_operating_mode->get_name(), 0x03); // Heizen
        }
    }
}

///////////////// Texts /////////////////
void DaikinRotexCanComponent::custom_request(std::string const& value) {
    const uint32_t can_id = 0x680;
    const bool use_extended_id = false;

    const std::size_t pipe_pos = value.find("|");
    if (pipe_pos != std::string::npos) { // handle response
        uint16_t can_id = Utils::hex_to_uint16(value.substr(0, pipe_pos));
        std::string buffer = value.substr(pipe_pos + 1);
        const TMessage message = Utils::str_to_bytes(buffer);

        m_data_requests.handle(can_id, message);
    } else { // send custom request
        const TMessage buffer = Utils::str_to_bytes(value);
        if (is_command_set(buffer)) {
            esphome::esp32_can::ESP32Can* pCanbus = m_data_requests.getCanbus();
            pCanbus->send_data(can_id, use_extended_id, { buffer.begin(), buffer.end() });

            Utils::log("sendGet", "can_id<%s> data<%s>",
                Utils::to_hex(can_id).c_str(), value.c_str(), Utils::to_hex(buffer).c_str());
        }
    }
}

///////////////// Selects /////////////////
void DaikinRotexCanComponent::set_generic_select(std::string const& id, std::string const& state) {
    auto const* pEntityConf = get_select_entity_conf(id);
    if (pEntityConf != nullptr) {
        auto it = pEntityConf->map.findByValue(state);
        if (it != pEntityConf->map.end()) {
            const bool handled = on_custom_select(pEntityConf->id, it->first);
            if (!handled) {
                m_data_requests.sendSet(pEntityConf->pEntity->get_name(), it->first);
            }
        } else {
            ESP_LOGE(TAG, "set_generic_select(%s, %s) => Invalid value!", id.c_str(), state.c_str());
        }
    }
}

///////////////// Numbers /////////////////
void DaikinRotexCanComponent::set_generic_number(std::string const& id, float value) {
    for (auto& entity_conf : m_accessor.get_entities()) {
        if (entity_conf.id == id && dynamic_cast<number::Number*>(entity_conf.pEntity) != nullptr) {
            m_data_requests.sendSet(entity_conf.pEntity->get_name(), value * entity_conf.divider);
            break;
        }
    }
}

///////////////// Buttons /////////////////
void DaikinRotexCanComponent::dhw_run() {
    const std::string id {"target_hot_water_temperature"};
    TRequest const* pRequest = m_data_requests.get(id);
    if (pRequest != nullptr) {
        float temp1 {70};
        float temp2 {0};

        auto const* pEntityConf = get_select_entity_conf(id);
        if (pEntityConf != nullptr) {
            temp1 *= pEntityConf->divider;

            number::Number* pNumber = pRequest->get_number();
            select::Select* pSelect = pRequest->get_select();

            if (pNumber != nullptr) {
                temp1 = pNumber->state * pEntityConf->divider;
            } else if (pSelect != nullptr) {
                auto it = pEntityConf->map.findByValue(pSelect->state);
                if (it != pEntityConf->map.end()) {
                    temp2 = it->first;
                }
            }
        }

        if (temp2 > 0) {
            const std::string name = pRequest->getName();

            m_data_requests.sendSet(name,  temp1);

            call_later([name, temp2, this](){
                m_data_requests.sendSet(name, temp2);
            }, 10*1000);
        } else {
            ESP_LOGE("dhw_rum", "Request doesn't have a Number!");
        }
    } else {
        ESP_LOGE("dhw_rum", "Request couldn't be found!");
    }
}

void DaikinRotexCanComponent::dump() {
    const char* DUMP = "dump";

    ESP_LOGI(DUMP, "------------------------------------------");
    ESP_LOGI(DUMP, "------------ DUMP %d Entities ------------", m_data_requests.size());
    ESP_LOGI(DUMP, "------------------------------------------");

    for (auto index = 0; index < m_data_requests.size(); ++index) {
        TRequest const* pRequest = m_data_requests.get(index);
        if (pRequest != nullptr) {
            EntityBase* pEntity = pRequest->get_entity();
            if (sensor::Sensor* pSensor = dynamic_cast<sensor::Sensor*>(pEntity)) {
                ESP_LOGI(DUMP, "%s: %f", pSensor->get_name().c_str(), pSensor->get_state());
            } else if (binary_sensor::BinarySensor* pBinarySensor = dynamic_cast<binary_sensor::BinarySensor*>(pEntity)) {
                ESP_LOGI(DUMP, "%s: %d", pBinarySensor->get_name().c_str(), pBinarySensor->state);
            } else if (number::Number* pNumber = dynamic_cast<number::Number*>(pEntity)) {
                ESP_LOGI(DUMP, "%s: %f", pNumber->get_name().c_str(), pNumber->state);
            } else if (text_sensor::TextSensor* pTextSensor = dynamic_cast<text_sensor::TextSensor*>(pEntity)) {
                ESP_LOGI(DUMP, "%s: %s", pTextSensor->get_name().c_str(), pTextSensor->get_state().c_str());
            } else if (select::Select* pSelect = dynamic_cast<select::Select*>(pEntity)) {
                ESP_LOGI(DUMP, "%s: %s", pSelect->get_name().c_str(), pSelect->state.c_str());
            }
        } else {
            ESP_LOGE(DUMP, "Entity with index<%d> not found!", index);
        }
    }
    ESP_LOGI(DUMP, "------------------------------------------");
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
    m_data_requests.sendNextPendingGet();
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
    TMessage message;
    std::copy_n(data.begin(), 7, message.begin());
    m_data_requests.handle(can_id, message);
}

void DaikinRotexCanComponent::dump_config() {
    ESP_LOGCONFIG(TAG, "DaikinRotexCanComponent");
}

void DaikinRotexCanComponent::throwPeriodicError(std::string const& message) {
    call_later([message, this]() {
        ESP_LOGE(TAG, message.c_str());
        throwPeriodicError(message);
    }, POST_SETUP_TIMOUT);
}

bool DaikinRotexCanComponent::is_command_set(TMessage const& message) {
    for (auto& b : message) {
        if (b != 0x00) {
            return true;
        }
    }
    return false;
}

Accessor::TEntityArguments const* DaikinRotexCanComponent::get_select_entity_conf(std::string const& id) const {
    for (auto& entity_conf : m_accessor.get_entities()) {
        if (entity_conf.id == id && dynamic_cast<select::Select*>(entity_conf.pEntity) != nullptr) {
            return &entity_conf;
        }
    }
    return nullptr;
}

} // namespace daikin_rotex_can
} // namespace esphome