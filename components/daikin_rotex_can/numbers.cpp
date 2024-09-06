#include "esphome/components/daikin_rotex_can/numbers.h"

namespace esphome {
namespace daikin_rotex_can {

void TargetHotWaterTemperatureNumber::control(float value) {
    this->publish_state(value);
    this->parent_->set_target_hot_water_temperature(value);
}

void TargetRoom1TemperatureNumber::control(float value) {
    this->publish_state(value);
    this->parent_->set_target_room1_temperature(value);
}

void FlowTemperatureDayNumber::control(float value) {
    this->publish_state(value);
    this->parent_->set_flow_temperature_day(value);
}

void MaxTargetFlowTempNumber::control(float value) {
    this->publish_state(value);
    this->parent_->set_max_target_flow_temp(value);
}

void MinTargetFlowTempNumber::control(float value) {
    this->publish_state(value);
    this->parent_->set_min_target_flow_temp(value);
}

void HeatingCurveNumber::control(float value) {
    this->publish_state(value);
    this->parent_->set_heating_curve(value);
}

}  // namespace seeed_mr24hpc1
}  // namespace esphome
