#include "esphome/components/daikin_rotex_can/buttons.h"

namespace esphome {
namespace daikin_rotex_can {

void DHWRunButton::press_action() {
    this->parent_->dhw_run();
}

}  // namespace seeed_mr24hpc1
}  // namespace esphome
