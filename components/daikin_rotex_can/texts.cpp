#include "esphome/components/daikin_rotex_can/texts.h"

namespace esphome {
namespace daikin_rotex_can {

void LogFilterText::control(const std::string &value) {
    this->publish_state(value);
    Utils::g_log_filter = value;
}

}  // namespace seeed_mr24hpc1
}  // namespace esphome
