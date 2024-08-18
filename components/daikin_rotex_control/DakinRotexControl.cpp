#include "esphome/components/daikin_rotex_control/DakinRotexControl.h"
#include "esphome/components/daikin_rotex_control/request.h"
#include <string>
#include <vector>

namespace esphome {
namespace dakin_rotex_control {

static const char *TAG = "dakin_rotex_control";

const std::vector<TRequest> entity_config = {
    {
        "Aussentemperatur",
        {0x31, 0x00, 0xFA, 0xC0, 0xFF, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0xC0, 0xFF,   DC,   DC},
        [](auto const& data) -> DataType {
            float temperature = float(((int((data[6]) + ((data[5]) << 8))) ^ 0x8000) - 0x8000)/10;
            //id(temperature_outside).publish_state(temperature); 
            return temperature;
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

    //ESP_LOGI(TAG, "bbb: %d", 234);

    if (request_name == std::string("Aussentemperatur")) {
        ESP_LOGI(TAG, "onPublish -> Aussentemperatur");
        float value = std::get<float>(variant);
        m_pTemperatureOutsideSensor->publish_state(value);
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