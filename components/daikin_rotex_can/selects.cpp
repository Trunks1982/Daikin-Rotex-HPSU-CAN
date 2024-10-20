#include "esphome/components/daikin_rotex_can/selects.h"

namespace esphome {
namespace daikin_rotex_can {

static const char* TAG = "daikin_rotex_can";

void CanSelect::control(const std::string &value) {
    this->publish_state(value);
    const uint16_t key = getKey(state);
    const bool handled = m_custom_select_lambda(get_id(), key);
    if (!handled) {
        sendSet(m_pCanbus, key);
    }
}

uint16_t CanSelect::getKey(std::string const& value) const {
    auto it = m_map.findByValue(value);
    if (it != m_map.end()) {
        return it->first;
    } else {
        ESP_LOGE(TAG, "CanSelect::getValue(%s) => Value not found!", value.c_str());
    }
    return 0;
}

void CanSelect::publish_select_key(uint16_t key) {
    auto it = m_map.findByKey(key);
    if (it != m_map.end()) {
        publish_state(it->second);
    } else {
        ESP_LOGE(TAG, "CanSelect::publish_select_key(%s) => Key not found!", key);
    }
}

TEntity::TVariant CanSelect::handleValue(uint16_t value) {
    const std::string str = findNextByKey(value, Utils::format("INVALID<%f>", value));
    publish_state(str);
    return str;
}


}  // namespace daikin_rotex_can
}  // namespace esphome
