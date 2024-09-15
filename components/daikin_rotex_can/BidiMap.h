#pragma once

#include <sstream>  
#include <map>

template<typename KeyType, typename ValueType>
class BidiMap {
public:
    using Iterator = typename std::map<KeyType, ValueType>::const_iterator;

    BidiMap(std::initializer_list<std::pair<const KeyType, ValueType>> init_list)
    : key_to_value(init_list) {
        for (const auto& pair : init_list) {
            value_to_key[pair.second] = pair.first;
        }
    }

    BidiMap(const std::map<KeyType, ValueType>& map)
    : key_to_value(map) {
        for (const auto& pair : map) {
            value_to_key[pair.second] = pair.first;
        }
    }

    Iterator findByKey(const KeyType& key) const {
        return key_to_value.find(key);
    }

    Iterator findByValue(const ValueType& value) const {
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
    std::map<KeyType, ValueType> key_to_value;
    std::map<ValueType, KeyType> value_to_key;
};
