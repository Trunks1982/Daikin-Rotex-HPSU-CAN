#pragma once

#include <string>

namespace esphome {
namespace daikin_rotex_can {

class SensorAccessor {
public:
    virtual void dhw_run() = 0;
    virtual void dump() = 0;

    virtual void custom_request(std::string const& value) = 0;
};

}
}