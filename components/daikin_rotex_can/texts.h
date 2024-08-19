#pragma once

#include "esphome/components/text/text.h"
#include "esphome/components/daikin_rotex_can/daikin_rotex_can.h"

namespace esphome {
namespace daikin_rotex_can {

class LogFilterText : public text::Text, public Parented<DaikinRotexCanComponent> {
public:
    LogFilterText() = default;

protected:
    void control(const std::string &value) override;
};

}
}
