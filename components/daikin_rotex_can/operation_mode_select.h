#pragma once

#include "esphome/components/select/select.h"
#include "esphome/components/daikin_rotex_can/daikin_rotex_can.h"

namespace esphome {
namespace daikin_rotex_can {

class OperationModeSelect : public select::Select, public Parented<DaikinRotexCanComponent> {
public:
    OperationModeSelect() = default;

protected:
    void control(const std::string &value) override;
};

}  // namespace ld2410
}  // namespace esphome
