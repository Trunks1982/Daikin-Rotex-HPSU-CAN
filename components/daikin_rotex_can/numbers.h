#pragma once

#include "esphome/components/number/number.h"
#include "esphome/components/daikin_rotex_can/daikin_rotex_can.h"

namespace esphome {
namespace daikin_rotex_can {

class CanNumber : public number::Number, public TEntity, public Parented<DaikinRotexCanComponent> {
public:
    CanNumber() = default;
protected:
    void control(float value) override;
    virtual TVariant handleValue(uint16_t value) override;
};

}
}
