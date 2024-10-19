#pragma once

#include "esphome/components/daikin_rotex_can/daikin_rotex_can.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace daikin_rotex_can {

class GenericSensor : public sensor::Sensor, public TRequest, public Parented<DaikinRotexCanComponent> {
public:
    GenericSensor() = default;
private:
};

class GenericTextSensor : public text_sensor::TextSensor, public TRequest, public Parented<DaikinRotexCanComponent> {
public:
    GenericTextSensor() = default;
private:
};

class GenericBinarySensor : public binary_sensor::BinarySensor, public TRequest, public Parented<DaikinRotexCanComponent> {
public:
    GenericBinarySensor() = default;
private:
};

}  // namespace ld2410
}  // namespace esphome
