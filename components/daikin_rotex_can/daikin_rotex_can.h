#pragma once

#include "esphome/components/daikin_rotex_can/requests.h"
#include "esphome/components/daikin_rotex_can/Accessor.h"
#include "esphome/components/esp32_can/esp32_can.h"
#include "esphome/core/component.h"

namespace esphome {
namespace daikin_rotex_can {

class DaikinRotexCanComponent: public Component {
public:
    DaikinRotexCanComponent();
    void setup() override;
    void loop() override;
    void dump_config() override;

    void set_canbus(esphome::esp32_can::ESP32Can* pCanbus);

    // Selects
    void set_operation_mode(std::string const& state);
    void set_hk_function(std::string const& state);
    void set_sg_mode(std::string const& state);
    void set_smart_grid(std::string const& state);

    // Numbers
    void set_target_hot_water_temperature(float temperature);
    void set_target_room1_temperature(float temperature);
    void set_flow_temperature_day(float temperature);
    void set_max_target_flow_temp(float temperature);
    void set_min_target_flow_temp(float temperature);
    void set_heating_curve(float heating_curve);

    void validateConfig();

    Accessor& getAccessor() { return m_accessor; }

    void handle(uint32_t can_id, std::vector<uint8_t> const& data);

private:

    using TCanbusAutomation = esphome::Automation<std::vector<uint8_t>, uint32_t, bool>;
    using TCanbusAction = esphome::Action<std::vector<uint8_t>, uint32_t, bool>;

    class MyAction : public TCanbusAction {
    public:
        MyAction(DaikinRotexCanComponent* pParent): m_pParent(pParent) {}
    protected:
        virtual void play(std::vector<uint8_t> data, uint32_t can_id, bool remote_transmission_request) override {
            m_pParent->handle(can_id, data);
        }
    private:
        DaikinRotexCanComponent* m_pParent;
    };

    Accessor m_accessor;
    TRequests m_data_requests;

    std::shared_ptr<esphome::canbus::CanbusTrigger> m_canbus_trigger;
    std::shared_ptr<TCanbusAutomation> m_canbus_automation;
    std::shared_ptr<MyAction> m_canbus_action;
};

inline void DaikinRotexCanComponent::set_canbus(esphome::esp32_can::ESP32Can* pCanbus) {
    m_data_requests.setCanbus(pCanbus);

    m_canbus_trigger = std::make_shared<esphome::canbus::CanbusTrigger>(pCanbus, 0, 0, false); // Listen to all can messages
    m_canbus_automation = std::make_shared<TCanbusAutomation>(m_canbus_trigger.get());
    m_canbus_action = std::make_shared<MyAction>(this);
    m_canbus_automation->add_action(m_canbus_action.get());
    pCanbus->add_trigger(m_canbus_trigger.get());
}

} // namespace daikin_rotex_can
} // namespace esphome