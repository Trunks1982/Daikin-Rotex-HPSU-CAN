#include "esphome/components/daikin_rotex_can/sensors.h"
#include "esphome/components/daikin_rotex_can/entity.h"

namespace esphome {
namespace daikin_rotex_can {

TEntity::TVariant CanSensor::handleValue(uint16_t value) {
    TEntity::TVariant variant = value / m_config.divider;
    publish_state(std::get<float>(variant));
    return variant;
}

TEntity::TVariant CanTextSensor::handleValue(uint16_t value) {
    auto it = m_config.map.findByKey(value);
    const std::string str = m_recalculate_state(m_config.pEntity, it != m_config.map.end() ? it->second : Utils::format("INVALID<%f>", value));
    publish_state(str);
    return str;
}

TEntity::TVariant CanBinarySensor::handleValue(uint16_t value) {
    TEntity::TVariant variant = value > 0;
    publish_state(std::get<bool>(variant));
    return variant;
}

}
}