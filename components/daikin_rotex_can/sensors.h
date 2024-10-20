#pragma once

#include "esphome/components/daikin_rotex_can/daikin_rotex_can.h"
#include "esphome/components/daikin_rotex_can/BidiMap.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace daikin_rotex_can {

class CanSensor : public sensor::Sensor, public TEntity, public Parented<DaikinRotexCanComponent> {
public:
    CanSensor() = default;

protected:
    virtual TVariant handleValue(uint16_t value) override;
};

class CanTextSensor : public text_sensor::TextSensor, public TEntity, public Parented<DaikinRotexCanComponent> {
public:
    using TRecalculateState = std::function<std::string(EntityBase*, std::string const&)>;

    CanTextSensor() = default;
    void set_map(std::string const& str_map) { m_map = Utils::str_to_map(str_map); }
    void set_recalculate_state(TRecalculateState&& lambda) { m_recalculate_state = std::move(lambda); }
protected:
    virtual TVariant handleValue(uint16_t value) override;
private:
    BidiMap m_map;
    TRecalculateState m_recalculate_state;
};

class CanBinarySensor : public binary_sensor::BinarySensor, public TEntity, public Parented<DaikinRotexCanComponent> {
public:
    CanBinarySensor() = default;

protected:
    virtual TVariant handleValue(uint16_t value) override;
};

}  // namespace ld2410
}  // namespace esphome
