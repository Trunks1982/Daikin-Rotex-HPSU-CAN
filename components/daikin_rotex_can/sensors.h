#pragma once

#include "esphome/components/daikin_rotex_can/sensor_accessor.h"
#include "esphome/components/daikin_rotex_can/BidiMap.h"
#include "esphome/components/daikin_rotex_can/entity.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/number/number.h"
#include "esphome/components/select/select.h"

namespace esphome {
namespace daikin_rotex_can {

/////////////////////// CanSensor ///////////////////////

class CanSensor : public sensor::Sensor, public TEntity, public Parented<SensorAccessor> {
public:
    CanSensor() = default;

protected:
    virtual TVariant handleValue(uint16_t value) override;
};

/////////////////////// CanTextSensor ///////////////////////

class CanTextSensor : public text_sensor::TextSensor, public TEntity, public Parented<SensorAccessor> {
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

/////////////////////// CanBinarySensor ///////////////////////

class CanBinarySensor : public binary_sensor::BinarySensor, public TEntity, public Parented<SensorAccessor> {
public:
    CanBinarySensor() = default;

protected:
    virtual TVariant handleValue(uint16_t value) override;
};

/////////////////////// CanNumber ///////////////////////

class CanNumber : public number::Number, public TEntity, public Parented<SensorAccessor> {
public:
    CanNumber() = default;
protected:
    void control(float value) override;
    virtual TVariant handleValue(uint16_t value) override;
};

/////////////////////// CanSelect ///////////////////////

class CanSelect : public select::Select, public TEntity, public Parented<SensorAccessor> {
    using TCustomSelectLambda = std::function<bool(std::string const& id, uint16_t key)>;
public:
    CanSelect() = default;
    void set_map(std::string const& str_map) { m_map = Utils::str_to_map(str_map); }
    void set_custom_select_lambda(TCustomSelectLambda&& lambda) { m_custom_select_lambda = std::move(lambda); }

    std::string findNextByKey(uint16_t value, std::string const& fallback) const;
    uint16_t getKey(std::string const& value) const;

    void publish_select_key(uint16_t key);
protected:
    void control(const std::string &value) override;
    virtual TVariant handleValue(uint16_t value) override;

private:
    BidiMap m_map;
    TCustomSelectLambda m_custom_select_lambda;
};

inline std::string CanSelect::findNextByKey(uint16_t value, std::string const& fallback) const { 
    auto it = m_map.findNextByKey(value);
    return it != m_map.end() ? it->second : Utils::format("INVALID<%f>", value);
};

}  // namespace ld2410
}  // namespace esphome