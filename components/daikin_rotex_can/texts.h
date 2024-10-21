#pragma once

#include "esphome/components/daikin_rotex_can/sensor_accessor.h"
#include "esphome/components/text/text.h"

namespace esphome {
namespace daikin_rotex_can {

class LogFilterText : public text::Text, public Parented<SensorAccessor> {
public:
    LogFilterText();
protected:
    void control(const std::string &value) override;
};
class CustomRequestText : public text::Text, public Parented<SensorAccessor> {
public:
    CustomRequestText();
protected:
    void control(const std::string &value) override;
};

}
}
