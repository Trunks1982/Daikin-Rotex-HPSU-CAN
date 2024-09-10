#include "esphome/components/daikin_rotex_can/utils.h"
#include <regex>

namespace esphome {
namespace daikin_rotex_can {

std::string Utils::g_log_filter = "";
static const char* TAG = "Utils";

bool Utils::find(std::string const& haystack, std::string const& needle) {
    auto it = std::search(
        haystack.begin(), haystack.end(),
        needle.begin(), needle.end(),
        [](unsigned char ch1, unsigned char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
    );
    return (it != haystack.end());
}

std::vector<std::string> Utils::split(std::string const& str) {
    std::string segment;
    std::istringstream iss(str);
    std::vector<std::string> result;

    while (std::getline(iss, segment, '|'))
    {
        if (!segment.empty()) {
            result.push_back(segment);
        }
    }

    return result;
}

std::string Utils::to_hex(uint32_t value) {
    char hex_string[20];
    sprintf(hex_string, "0x%02X", value);
    return std::string(hex_string);
}

std::vector<uint8_t> Utils::str_to_bytes(const std::string& str) {
    std::vector<uint8_t> bytes;
    std::stringstream ss(str);
    std::string byteStr;

    while (ss >> byteStr) {
        const uint8_t byte = static_cast<uint8_t>(std::stoi(byteStr, nullptr, 16));
        bytes.push_back(byte);
    }

    return bytes;
}

std::array<uint8_t, 7> Utils::str_to_bytes_array8(const std::string& str) {
    std::array<uint8_t, 7> byte_array{};

    std::string cleaned_str = std::regex_replace(str, std::regex("[^0-9A-Fa-f\\s]+"), "");
    cleaned_str = std::regex_replace(cleaned_str, std::regex("\\s+"), " ");

    std::stringstream ss(cleaned_str);
    std::string byte_str;
    size_t index = 0;

    while (ss >> byte_str && index < byte_array.size()) {
        byte_array[index++] = static_cast<uint8_t>(std::stoul(byte_str, nullptr, 16));
    }

    return byte_array;
}

std::array<uint16_t, 7> Utils::str_to_bytes_array16(const std::string& str) {
    std::array<uint16_t, 7> words;
    std::stringstream ss(str);
    std::string wordStr;
    const uint16_t DC = 0xFFFF;

    uint8_t index = 0;
    while (ss >> wordStr && index < 7) {
        const uint16_t word = (wordStr == "__") ? DC : static_cast<uint16_t>(std::stoi(wordStr, nullptr, 16));
        words[index++] = word;
    }

    return words;
}

std::map<uint8_t, std::string> Utils::str_to_map(const std::string& input) {
    std::map<uint8_t, std::string> result;
    std::stringstream ss(input);
    std::string pair;

    while (std::getline(ss, pair, '|')) {
        size_t pos = pair.find(':');
        if (pos != std::string::npos) {
            std::string keyStr = pair.substr(0, pos);
            std::string value = pair.substr(pos + 1);

            uint8_t key = static_cast<uint8_t>(std::strtoul(keyStr.c_str(), nullptr, 16));

            result[key] = value;
        }
    }

    return result;
}

std::vector<uint8_t> Utils::str_to_bytes(const std::string& input, uint16_t value) {
    std::vector<uint8_t> result;
    std::istringstream stream(input);
    std::string byteStr;

    uint8_t hiByte = (value >> 8) & 0xFF;
    uint8_t loByte = value & 0xFF;

    int placeholderCount = 0;

    while (stream >> byteStr) {
        if (byteStr == "__") {
            ++placeholderCount;
            if (placeholderCount == 2) {
                result.push_back(hiByte);
                result.push_back(loByte);
                placeholderCount = 0;
            }
        } else {
            if (placeholderCount == 1) {
                result.push_back(loByte);
                placeholderCount = 0;
            }
            result.push_back(static_cast<uint8_t>(std::stoi(byteStr, nullptr, 16)));
        }
    }

    if (placeholderCount == 1) {
        result.push_back(loByte);
    }

    return result;
}

sensor::Sensor* Utils::toSensor(EntityBase* pEntity) {
    if (sensor::Sensor* pSensor = dynamic_cast<sensor::Sensor*>(pEntity)) {
        return pSensor;
    } else {
        ESP_LOGE(TAG, "Entity is not a sensor: %s", pEntity->get_name().c_str());
        return nullptr;
    }
}

text_sensor::TextSensor* Utils::toTextSensor(EntityBase* pEntity) {
    if (text_sensor::TextSensor* pTextSensor = dynamic_cast<text_sensor::TextSensor*>(pEntity)) {
        return pTextSensor;
    } else {
        ESP_LOGE(TAG, "Entity is not a text sensor: %s", pEntity->get_name().c_str());
        return nullptr;
    }
}

binary_sensor::BinarySensor* Utils::toBinarySensor(EntityBase* pEntity) {
    if (binary_sensor::BinarySensor* pBinarySensor = dynamic_cast<binary_sensor::BinarySensor*>(pEntity)) {
        return pBinarySensor;
    } else {
        ESP_LOGE(TAG, "Entity is not a binary sensor: %s", pEntity->get_name().c_str());
        return nullptr;
    }
}

select::Select* Utils::toSelect(EntityBase* pEntity) {
    if (select::Select* pSelect = dynamic_cast<select::Select*>(pEntity)) {
        return pSelect;
    } else {
        ESP_LOGE(TAG, "Entity is not a select: %s", pEntity->get_name().c_str());
        return nullptr;
    }
}

number::Number* Utils::toNumber(EntityBase* pEntity) {
    if (number::Number* pNumber = dynamic_cast<number::Number*>(pEntity)) {
        return pNumber;
    } else {
        ESP_LOGE(TAG, "Entity is not a number: %s", pEntity->get_name().c_str());
        return nullptr;
    }
}


}
}