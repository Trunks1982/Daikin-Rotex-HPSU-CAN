#pragma once

#include <array>
#include <variant>
#include <string>

namespace esphome {
namespace daikin_rotex_can {

using TMessage = std::array<uint8_t, 7>;

}
}