#pragma once

#include "esphome/components/select/select.h"
#include "DakinRotexControl.h"

namespace esphome {
namespace dakin_rotex_control {

class OperationModeSelect : public select::Select, public Parented<DakinRotexControl> {
public:
    OperationModeSelect() = default;

protected:
    void control(const std::string &value) override;
};

}  // namespace ld2410
}  // namespace esphome
