#include "partitions_dumper.h"
#include <array>
#include <sstream>
#include <iomanip>
#include <esp_ota_ops.h>
#include "esphome/core/log.h"
#include "esp_partition.h"

namespace esphome {

std::string to_hex(std::array<uint8_t, 16> const& data) {
    std::stringstream str;
    str.setf(std::ios_base::hex, std::ios::basefield);
    str.setf(std::ios_base::uppercase);
    str.fill('0');

    bool first = true;
    for (uint8_t chr : data)
    {
        if (first) {
            first = false;
        } else {
            str << " ";
        }
        str << std::setw(2) << (unsigned short)(std::byte)chr;
    }
    return str.str();
}

void print_ota_partition(esp_partition_t const* partition, std::string const& partition_getter) {
  std::string label;
  esp_err_t err_state;
  esp_ota_img_states_t ota_state;
  std::string data;

  if (partition) {
    label = partition->label;
    err_state = esp_ota_get_state_partition(partition, &ota_state);

    uint8_t buf[16];
    const auto state = esp_partition_read(partition, 0, buf, sizeof(buf));

    std::array<uint8_t, 16> buf_array;
    std::copy(std::begin(buf), std::end(buf), buf_array.begin());

    data = to_hex(buf_array);
  } else {
    label = "nullptr";
  }
  ESP_LOGE("TEST", "%s: %s, err_state: %d, ota_state:%d, data: %s",
    partition_getter.c_str(), label.c_str(), err_state, ota_state, data.c_str());
}

void dump_partitions() {
  ESP_LOGE("TEST", "esp_ota_get_app_partition_count: %d",
    esp_ota_get_app_partition_count());

  print_ota_partition(esp_ota_get_boot_partition(), "esp_ota_get_boot_partition");
  print_ota_partition(esp_ota_get_last_invalid_partition(), "esp_ota_get_last_invalid_partition");
  print_ota_partition(esp_ota_get_running_partition(), "esp_ota_get_running_partition");
  print_ota_partition(esp_ota_get_next_update_partition(nullptr), "esp_ota_get_next_update_partition");
}

}