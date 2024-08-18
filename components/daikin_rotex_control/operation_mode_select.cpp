#include "operation_mode_select.h"

namespace esphome {
namespace dakin_rotex_control {

void OperationModeSelect::control(const std::string &value) {
    this->publish_state(value);
    this->parent_->set_operation_mode(state);
}

}  // namespace dakin_rotex_control
}  // namespace esphome
