#include "esphome/components/daikin_rotex_can/sensors.h"
#include "esphome/components/daikin_rotex_can/entity.h"

namespace esphome {
namespace daikin_rotex_can {

static const char* TAG = "daikin_rotex_can";

/////////////////////// CanSensor ///////////////////////

bool CanSensor::handleValue(uint16_t value, TEntity::TVariant& variant) {
    variant = value / m_config.divider;
    const float float_value = std::get<float>(variant);
    const bool valid = !m_range.required() || (float_value >= m_range.min && float_value <= m_range.max);
    if (valid) {
        publish_state(float_value);
    } else {
        ESP_LOGE(TAG, "CanSensor::handleValue() => Sensor<%s> value<%f> out of range[%f, %f]",
            get_id().c_str(), float_value, m_range.min, m_range.max);
    }

    return valid;
}

/////////////////////// CanTextSensor ///////////////////////

bool CanTextSensor::handleValue(uint16_t value, TEntity::TVariant& variant) {
    auto it = m_map.findByKey(value);
    variant = m_recalculate_state(m_config.pEntity, it != m_map.end() ? it->second : Utils::format("INVALID<%f>", value));
    publish_state(std::get<std::string>(variant));
    return true;
}

/////////////////////// CanBinarySensor ///////////////////////

bool CanBinarySensor::handleValue(uint16_t value, TEntity::TVariant& variant) {
    variant = value > 0;
    publish_state(std::get<bool>(variant));
    return true;
}

/////////////////////// CanNumber ///////////////////////

void CanNumber::control(float value) {
    this->publish_state(value);
    sendSet(m_pCanbus, value * get_config().divider);
}

bool CanNumber::handleValue(uint16_t value, TEntity::TVariant& variant) {
    variant = value / m_config.divider;
    publish_state(std::get<float>(variant));
    return true;
}

/////////////////////// CanSelect ///////////////////////

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

bool CanSelect::handleValue(uint16_t value, TEntity::TVariant& variant) {
    variant = findNextByKey(value, Utils::format("INVALID<%f>", value));
    publish_state(std::get<std::string>(variant));
    return true;
}

}
}