#include "esphome/components/daikin_rotex_can/sensors.h"
#include "esphome/components/daikin_rotex_can/request.h"

namespace esphome {
namespace daikin_rotex_can {

TRequest::TVariant GenericSensor::handleValue(uint16_t value) {
    TRequest::TVariant variant = value / m_config.divider;
    publish_state(std::get<float>(variant));
    return variant;
}

TRequest::TVariant GenericTextSensor::handleValue(uint16_t value) {
    auto it = m_config.map.findByKey(value);
    const std::string str = m_recalculate_state(m_config.pEntity, it != m_config.map.end() ? it->second : Utils::format("INVALID<%f>", value));
    publish_state(str);
    return str;
}

TRequest::TVariant GenericBinarySensor::handleValue(uint16_t value) {
    TRequest::TVariant variant = value > 0;
    publish_state(std::get<bool>(variant));
    return variant;
}

}
}