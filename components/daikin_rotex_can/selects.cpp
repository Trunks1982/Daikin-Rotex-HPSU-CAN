#include "esphome/components/daikin_rotex_can/selects.h"

namespace esphome {
namespace daikin_rotex_can {

static const char* TAG = "daikin_rotex_can";

void GenericSelect::control(const std::string &value) {
    this->publish_state(value);
    this->parent_->set_generic_select(m_id, state);
}

void GenericSelect::set_map(std::string const& str_map) {
    m_map = Utils::str_to_map(str_map);
}

uint16_t GenericSelect::getKey(std::string const& value) const {
    auto it = m_map.findByValue(value);
    if (it != m_map.end()) {
        return it->first;
    } else {
        ESP_LOGE(TAG, "GenericSelect::getValue(%s) => Value not found!", value.c_str());
    }
    return 0;
}

void GenericSelect::publish_select_key(uint16_t key) {
    auto it = m_map.findByKey(key);
    if (it != m_map.end()) {
        publish_state(it->second);
    } else {
        ESP_LOGE(TAG, "GenericSelect::publish_select_key(%s) => Key not found!", key);
    }
}

TRequest::TVariant GenericSelect::handleValue(uint16_t value) {
    const std::string str = findNextByKey(value, Utils::format("INVALID<%f>", value));
    publish_state(str);
    return str;
}


}  // namespace daikin_rotex_can
}  // namespace esphome
