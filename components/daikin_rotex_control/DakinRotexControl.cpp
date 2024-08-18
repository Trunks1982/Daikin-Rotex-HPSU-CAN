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
        [](auto const& data) -> DataType {
            return float(((int((data[6]) + ((data[5]) << 8))) ^ 0x8000) - 0x8000)/10;
        }
    },
    {
        "Betriebsmodus",
        {0x31, 0x00, 0xFA, 0x01, 0x12, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x01, 0x12,   DC,   DC},
        [](auto const& data) -> DataType {
            return map_betriebsmodus.getValue(data[5]);
            //id(Betriebsmodus).publish_state(data[5]);
            //return Utils::setSelectOption(id(select_betriebsmodus), map_betriebsmodus, data[5]);
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
    //ESP_LOGI(TAG, "onPublish");

    if (request_name == "Aussentemperatur") {
        m_pTemperatureOutsideSensor->publish_state(std::get<float>(variant));
    } else if (request_name == "Betriebsmodus") {
        const std::string value = std::get<std::string>(variant);
        m_pOperationModeSensor->publish_state(value.c_str());
    }
}

void DakinRotexControl::loop() {
    m_data_requests.sendNextPendingGet();
}

void DakinRotexControl::handle(uint32_t can_id, std::vector<uint8_t> const& data) {
    m_data_requests.handle(can_id, data, millis());
}

void DakinRotexControl::dump_config() {
    ESP_LOGCONFIG(TAG, "DakinRotexControl");
}

} // namespace dakin_rotex_control
} // namespace esphome