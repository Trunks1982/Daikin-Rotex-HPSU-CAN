#include "esphome/components/daikin_rotex_can/numbers.h"

namespace esphome {
namespace daikin_rotex_can {

void TargetHotWaterTemperatureNumber::control(float value) {
    this->publish_state(value);
    this->parent_->set_target_hot_water_temperature(value);
}

}  // namespace seeed_mr24hpc1
}  // namespace esphome
