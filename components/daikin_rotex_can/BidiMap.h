#pragma once

#include <sstream>
#include <map>

namespace esphome {
namespace daikin_rotex_can {

class BidiMap {
public:
    using Iterator = typename std::map<uint16_t, std::string>::const_iterator;

    BidiMap(std::initializer_list<std::pair<const uint16_t, std::string>> init_list)
    : key_to_value(init_list) {
        for (const auto& pair : init_list) {
            value_to_key[pair.second] = pair.first;
        }
    }

    BidiMap(const std::map<uint16_t, std::string>& map)
    : key_to_value(map)
    {
        for (const auto& pair : map) {
            value_to_key[pair.second] = pair.first;
        }
    }

    Iterator findByKey(const uint16_t& key) const {
        return key_to_value.find(key);
    }

    Iterator findNextByKey(uint16_t value) const;

    Iterator findByValue(const std::string& value) const {
        auto it = value_to_key.find(value);
        if (it != value_to_key.end()) {
            return key_to_value.find(it->second);
        }
        return key_to_value.end();
    }

    Iterator end() const {
        return key_to_value.end();
    }

    std::string string() const {
        std::stringstream ss;
        for (const auto& pair : key_to_value) {
            ss << "{" << std::to_string(pair.first) << ", " << pair.second << "} ";
        }
        return ss.str();
    }

private:
    std::map<uint16_t, std::string> key_to_value;
    std::map<std::string, uint16_t> value_to_key;
};

}
}