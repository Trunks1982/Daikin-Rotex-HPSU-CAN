#include "esphome/components/daikin_rotex_can/Accessor.h"


namespace esphome {
namespace daikin_rotex_can {

std::map<uint16_t, std::string> Accessor::str_to_map(const std::string& input) {
    std::map<uint16_t, std::string> result;
    std::stringstream ss(input);
    std::string pair;

    while (std::getline(ss, pair, '|')) {
        size_t pos = pair.find(':');
        if (pos != std::string::npos) {
            std::string keyStr = pair.substr(0, pos);
            std::string value = pair.substr(pos + 1);

            uint16_t key = static_cast<uint16_t>(std::strtoul(keyStr.c_str(), nullptr, 16));

            result[key] = value;
        }
    }

    return result;
}

}
}