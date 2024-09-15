#pragma once

#include "esphome/components/number/number.h"
#include "esphome/components/daikin_rotex_can/daikin_rotex_can.h"

namespace esphome {
namespace daikin_rotex_can {

class GenericNumber : public number::Number, public Parented<DaikinRotexCanComponent> {
public:
    GenericNumber() = default;
    void set_id(std::string const& id) { m_id = id; }
protected:
    void control(float value) override;
private:
    std::string m_id;
};

}
}
