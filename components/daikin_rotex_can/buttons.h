#pragma once

#include "esphome/components/button/button.h"
#include "esphome/components/daikin_rotex_can/daikin_rotex_can.h"

namespace esphome {
namespace daikin_rotex_can {

class DHWRunButton : public button::Button, public Parented<DaikinRotexCanComponent> {
public:
    DHWRunButton() = default;
protected:
    void press_action() override;
};

class DumpButton : public button::Button, public Parented<DaikinRotexCanComponent> {
public:
    DumpButton() = default;
protected:
    void press_action() override;
};

}
}
