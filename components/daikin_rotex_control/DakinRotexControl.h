#pragma once

#include "esphome/components/daikin_rotex_control/IPublisher.h"
#include "esphome/components/daikin_rotex_control/requests.h"
#include "esphome/components/esp32_can/esp32_can.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"

namespace esphome {
namespace dakin_rotex_control {

class DakinRotexControl: public Component, public IPublisher {
public:
    DakinRotexControl();
    void setup() override;
    void loop() override;
    void dump_config() override;
    void onPublish(std::string const& request_name, DataType const& variant) override;

    void set_canbus(esphome::esp32_can::ESP32Can* pCanbus);
    void set_temperature_outside_sensor(sensor::Sensor* pSensor);

    void handle(uint32_t can_id, std::vector<uint8_t> const& data);

protected:

    sensor::Sensor* m_pTemperatureOutsideSensor;

    TRequests m_data_requests;
private:

    using TCanbusAutomation = esphome::Automation<std::vector<uint8_t>, uint32_t, bool>;
    using TCanbusAction = esphome::Action<std::vector<uint8_t>, uint32_t, bool>;

    class MyAction : public TCanbusAction {
    public:
        MyAction(DakinRotexControl* pParent): m_pParent(pParent) {}
    protected:
        virtual void play(std::vector<uint8_t> data, uint32_t can_id, bool remote_transmission_request) override {
            m_pParent->handle(can_id, data);
        }
    private:
        DakinRotexControl* m_pParent;
    };

    std::shared_ptr<esphome::canbus::CanbusTrigger> m_canbus_trigger;
    std::shared_ptr<TCanbusAutomation> m_canbus_automation;
    std::shared_ptr<MyAction> m_canbus_action;
};

inline void DakinRotexControl::set_canbus(esphome::esp32_can::ESP32Can* pCanbus) {
    m_data_requests.setCanbus(pCanbus);

    m_canbus_trigger = std::make_shared<esphome::canbus::CanbusTrigger>(pCanbus, 0, 0, false); // Listen to all can messages
    m_canbus_automation = std::make_shared<TCanbusAutomation>(m_canbus_trigger.get());
    m_canbus_action = std::make_shared<MyAction>(this);
    m_canbus_automation->add_action(m_canbus_action.get());
    pCanbus->add_trigger(m_canbus_trigger.get());
}

inline void DakinRotexControl::set_temperature_outside_sensor(sensor::Sensor* pSensor) {
    m_pTemperatureOutsideSensor = pSensor;
}

} // namespace dakin_rotex_control
} // namespace esphome