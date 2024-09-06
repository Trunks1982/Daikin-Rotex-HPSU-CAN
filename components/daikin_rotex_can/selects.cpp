#include "esphome/components/daikin_rotex_can/selects.h"

namespace esphome {
namespace daikin_rotex_can {

void OperationModeSelect::control(const std::string &value) {
    this->publish_state(value);
    this->parent_->set_operation_mode(state);
}

void HKFunctionSelect::control(const std::string &value) {
    this->publish_state(value);
    this->parent_->set_hk_function(state);
}

void SGModeSelect::control(const std::string &value) {
    this->publish_state(value);
    this->parent_->set_sg_mode(state);
}

void SmartGridSelect::control(const std::string &value) {
    this->publish_state(value);
    this->parent_->set_smart_grid(state);
}

}  // namespace daikin_rotex_can
}  // namespace esphome
