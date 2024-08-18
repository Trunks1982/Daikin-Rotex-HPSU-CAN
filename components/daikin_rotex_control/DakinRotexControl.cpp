#include "esphome/components/daikin_rotex_control/DakinRotexControl.h"
#include "esphome/components/daikin_rotex_control/request.h"
#include "esphome/components/daikin_rotex_control/BidiMap.h"
#include <string>
#include <vector>

namespace esphome {
namespace dakin_rotex_control {

static const char *TAG = "dakin_rotex_control";

static const BidiMap<uint8_t, std::string> map_betriebsmodus {
    {0x01, "Bereitschaft"},
    {0x03, "Heizen"},
    {0x04, "Absenken"},
    {0x05, "Sommer"},
    {0x11, "Kühlen"},
    {0x0B, "Automatik 1"},
    {0x0C, "Automatik 2"}
};

const std::vector<TRequest> entity_config = {
    {
        "Aussentemperatur",
        {0x31, 0x00, 0xFA, 0xC0, 0xFF, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0xC0, 0xFF,   DC,   DC},
        [](auto const& data, auto& accessor) -> DataType {
            const float temp = float(((int((data[6]) + ((data[5]) << 8))) ^ 0x8000) - 0x8000) / 10;
            accessor.get_temperature_outside_sensor()->publish_state(temp);
            return temp;
        }
    },
    {
        "Betriebsmodus",
        {0x31, 0x00, 0xFA, 0x01, 0x12, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x01, 0x12,   DC,   DC},
        [](auto const& data, auto& accessor) -> DataType {
            const std::string mode = map_betriebsmodus.getValue(data[5]);
            accessor.get_operation_mode_sensor()->publish_state(mode);
            accessor.get_operation_mode_select()->publish_state(mode);
            return mode;
        }
    },
    {
        "Betriebsmodus setzen",
        [](auto const& value) -> std::vector<uint8_t> {
            return {0x30, 0x00, 0xFA, 0x01, 0x12, static_cast<uint8_t>(value), 0x00};
        }
    }
};

DakinRotexControl::DakinRotexControl()
: m_data_requests(this, std::move(entity_config))
{
}

void DakinRotexControl::setup() {
    //ESP_LOGI(TAG, "setup");
}

void DakinRotexControl::onPublish(std::string const& request_name, DataType const& variant) {
}

void DakinRotexControl::set_operation_mode(std::string const& mode) {
    m_data_requests.sendSet("Betriebsmodus setzen", map_betriebsmodus.getKey(mode));
    //m_data_requests.sendGet("Betriebsmodus");
}

void DakinRotexControl::loop() {
    m_data_requests.sendNextPendingGet();
}

void DakinRotexControl::handle(uint32_t can_id, std::vector<uint8_t> const& data) {
    m_data_requests.handle(can_id, data, m_accessor);
}

void DakinRotexControl::dump_config() {
    ESP_LOGCONFIG(TAG, "DakinRotexControl");
}

} // namespace dakin_rotex_control
} // namespace esphome