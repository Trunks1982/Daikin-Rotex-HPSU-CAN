#pragma once

#include "esphome/components/number/number.h"
#include "esphome/components/daikin_rotex_can/daikin_rotex_can.h"

namespace esphome {
namespace daikin_rotex_can {

class TargetHotWaterTemperatureNumber : public number::Number, public Parented<DaikinRotexCanComponent> {
public:
    TargetHotWaterTemperatureNumber() = default;

protected:
    void control(float value) override;
};

class FlowTemperatureDayNumber : public number::Number, public Parented<DaikinRotexCanComponent> {
public:
    FlowTemperatureDayNumber() = default;

protected:
    void control(float value) override;
};

class MaxTargetFlowTempNumber : public number::Number, public Parented<DaikinRotexCanComponent> {
public:
    MaxTargetFlowTempNumber() = default;

protected:
    void control(float value) override;
};

class MinTargetFlowTempNumber : public number::Number, public Parented<DaikinRotexCanComponent> {
public:
    MinTargetFlowTempNumber() = default;

protected:
    void control(float value) override;
};

class HeatingCurveNumber : public number::Number, public Parented<DaikinRotexCanComponent> {
public:
    HeatingCurveNumber() = default;

protected:
    void control(float value) override;
};

}
}
