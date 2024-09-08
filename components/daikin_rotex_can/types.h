#pragma once

#include <variant>
#include <string>
#include <stdint.h>

namespace esphome {
namespace daikin_rotex_can {

using DataType = std::variant<uint32_t, uint8_t, float, bool, std::string>;

}
}