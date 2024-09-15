#include "esphome/components/daikin_rotex_can/numbers.h"

namespace esphome {
namespace daikin_rotex_can {

void GenericNumber::control(float value) {
    this->publish_state(value);
    this->parent_->set_generic_number(m_id, value);
}

}  // namespace seeed_mr24hpc1
}  // namespace esphome
