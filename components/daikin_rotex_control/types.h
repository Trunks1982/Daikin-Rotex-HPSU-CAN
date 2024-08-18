#pragma once

#include <variant>
#include <string>
#include <stdint.h>

namespace esphome {
namespace dakin_rotex_control {

using DataType = std::variant<uint32_t, uint8_t, float, std::string>;

}
}