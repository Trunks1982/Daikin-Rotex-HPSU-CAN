#include "esphome/components/daikin_rotex_can/BidiMap.h"
#include "esphome/core/log.h"

namespace esphome {
namespace daikin_rotex_can {

BidiMap::Iterator BidiMap::findNextByKey(uint16_t value) const {
    if (key_to_value.empty()) {
        return key_to_value.end();
    }

    Iterator closest = key_to_value.end();
    float closest_distance = std::numeric_limits<float>::max();

    for (auto it = key_to_value.begin(); it != key_to_value.end(); ++it) {
        float distance = std::abs(it->first - value);

        if (distance < closest_distance) {
            closest_distance = distance;
            closest = it;
        }
    }

    return closest;
}

}
}