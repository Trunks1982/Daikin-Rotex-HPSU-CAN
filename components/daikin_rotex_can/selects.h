#pragma once

#include "esphome/components/select/select.h"
#include "esphome/components/daikin_rotex_can/daikin_rotex_can.h"

namespace esphome {
namespace daikin_rotex_can {

class GenericSelect : public select::Select, public Parented<DaikinRotexCanComponent> {
public:
    GenericSelect() = default;
    void set_id(std::string const& id) { m_id = id; }
protected:
    void control(const std::string &value) override;
private:
    std::string m_id;
};

}  // namespace ld2410
}  // namespace esphome
