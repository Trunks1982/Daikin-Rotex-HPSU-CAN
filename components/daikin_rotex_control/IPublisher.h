#pragma once

#include "esphome/components/daikin_rotex_control/types.h"

namespace esphome {
namespace dakin_rotex_control {

class IPublisher {
public:
    virtual void onPublish(std::string const& request_name, DataType const& variant);
};

}
}