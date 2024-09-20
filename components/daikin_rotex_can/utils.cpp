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

    while (std::getline(iss, segment, '|')) {
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

std::string Utils::to_hex(TMessage const& data) {
    std::stringstream str;
    str.setf(std::ios_base::hex, std::ios::basefield);
    str.setf(std::ios_base::uppercase);
    str.fill('0');

    bool first = true;
    for (uint8_t chr : data)
    {
        if (first) {
            first = false;
        } else {
            str << " ";
        }
        str << std::setw(2) << (unsigned short)(std::byte)chr;
    }
    return str.str();
}

TMessage Utils::str_to_bytes(const std::string& str) {
    TMessage bytes;
    std::stringstream ss(str);
    std::string byteStr;

    uint8_t index = 0;
    while (ss >> byteStr) {
        const uint8_t byte = static_cast<uint8_t>(std::stoi(byteStr, nullptr, 16));
        bytes[index++] = byte;
    }

    return bytes;
}

TMessage Utils::str_to_bytes_array8(const std::string& str) {
    TMessage byte_array{};

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

uint16_t Utils::hex_to_uint16(const std::string& hexStr) {
    uint16_t result;
    std::stringstream ss;

    ss << std::hex << hexStr;
    ss >> result;

    return result;
}

void Utils::setBytes(TMessage& data, uint16_t value, uint8_t offset, uint8_t len) {
    if (len == 1) {
        data[offset] = value & 0xFF;
    } else if (len == 2) {
        data[offset] = (value >> 8) & 0xFF;
        data[offset + 1] = value & 0xFF;
    } else {
        ESP_LOGE("write", "Invalid len: %d", len);
    }
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

template<typename... Args>
void Utils::log(std::string const& tag, std::string const& str_format, Args... args) {
    const std::string formated = Utils::format(str_format, args...);
    const std::string log_filter = g_log_filter;
    bool found = log_filter.empty();
    if (!found) {
        for (auto segment : Utils::split(log_filter)) {
            if (Utils::find(tag, segment) || Utils::find(formated, segment)) {
                found = true;
                break;
            }
        }
    }
    if (found) {
        ESP_LOGI(tag.c_str(), formated.c_str(), "");
    }
}

template void Utils::log<char const*, char const*, char const*, char const*>(std::string const& tag, std::string const& str_format, char const* arg1, char const* arg2, char const* arg3, char const* arg4);
template void Utils::log<char const*, float, char const*, char const*>(std::string const& tag, std::string const& str_format, char const* arg1, float arg2, char const* arg3, char const* arg4);
template void Utils::log<char const*, char const*, char const*>(std::string const& tag, std::string const& str_format, char const* arg1, char const* arg2, char const* arg3);
template void Utils::log<char const*, char const*>(std::string const& tag, std::string const& str_format, char const* arg1, char const* arg2);
template void Utils::log<unsigned char>(std::string const& tag, std::string const& str_format, unsigned char arg);
template void Utils::log<int>(std::string const& tag, std::string const& str_format, int arg);
template void Utils::log<>(std::string const& tag, std::string const& str_format);
template void Utils::log<char const*>(std::string const& tag, std::string const& str_format, char const* arg);
template void Utils::log<void*, void*>(std::string const& tag, std::string const& str_format, void* arg1, void* arg2);

}
}