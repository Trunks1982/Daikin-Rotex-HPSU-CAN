#pragma once

#include "esphome/core/log.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <vector>
#include <variant>
#include <memory>
#include <map>

namespace esphome {
namespace daikin_rotex_can {

class Utils {
public:
    template<typename... Args>
    static std::string format(const std::string& format, Args... args) {
        const auto size = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
        const auto buffer = std::make_unique<char[]>(size);

        std::snprintf(buffer.get(), size, format.c_str(), args...);

        return std::string(buffer.get(), buffer.get() + size - 1);
    }

    static bool find(std::string const& haystack, std::string const& needle) {
        auto it = std::search(
            haystack.begin(), haystack.end(),
            needle.begin(), needle.end(),
            [](unsigned char ch1, unsigned char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
        );
        return (it != haystack.end());
    }

    static std::vector<std::string> split(std::string const& str) {
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

    static bool is_number(const std::string& str) {
        return !str.empty() && std::find_if(
            str.begin(),
            str.end(),
            [](unsigned char chr) { return !std::isdigit(chr); }
        ) == str.end();
    }

    static std::string to_hex(uint32_t value) {
        char hex_string[20];
        sprintf(hex_string, "0x%02X", value);
        return std::string(hex_string);
    }

    template<typename T>
    static std::string to_hex(const T& data) {
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
            str << "0x" << std::setw(2) << (unsigned short)(std::byte)chr;
        }
        return str.str();
    }

    static std::vector<uint8_t> str_to_bytes(const std::string& str) {
        std::vector<uint8_t> bytes;
        std::stringstream ss(str);
        std::string byteStr;

        while (ss >> byteStr) {
            const uint8_t byte = static_cast<uint8_t>(std::stoi(byteStr, nullptr, 16));
            bytes.push_back(byte);
        }

        return bytes;
    }

    static std::array<uint8_t, 7> str_to_bytes_array8(const std::string& str) {
        std::array<uint8_t, 7> bytes;
        std::stringstream ss(str);
        std::string byteStr;

        uint8_t index = 0;
        while (ss >> byteStr && index < 7) {
            const uint8_t byte = static_cast<uint8_t>(std::stoi(byteStr, nullptr, 16));
            bytes[index++] = byte;
        }

        return bytes;
    }

    static std::array<uint16_t, 7> str_to_bytes_array16(const std::string& str) {
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

    static std::map<uint8_t, std::string> str_to_map(const std::string& input) {
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

    template<typename... Args>
    static void log(std::string const& tag, std::string const& str_format, Args... args) {
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

    static std::string g_log_filter;
};

}
}