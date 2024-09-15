#include "esphome/components/daikin_rotex_can/selects.h"

namespace esphome {
namespace daikin_rotex_can {

void GenericSelect::control(const std::string &value) {
    this->publish_state(value);
    this->parent_->set_generic_select(m_id, state);
}

}  // namespace daikin_rotex_can
}  // namespace esphome
