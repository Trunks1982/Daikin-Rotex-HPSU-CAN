#pragma once

#include <array>
#include <string>
#include "esp_partition.h"

namespace esphome {
namespace daikin_rotex_can {

class PartitionsDumper {
public:
    static void dump();

private:
    static std::string to_hex(std::array<uint8_t, 16> const& data);
    static void print_ota_partition(esp_partition_t const* partition, std::string const& partition_getter);
};

}
}