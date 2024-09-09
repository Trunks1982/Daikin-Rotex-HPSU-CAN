#pragma once

#include "esphome/core/log.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <variant>
#include <vector>
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

    static bool find(std::string const& haystack, std::string const& needle);
    static std::vector<std::string> split(std::string const& str);
    static bool is_number(const std::string& str);
    static std::string to_hex(uint32_t value);
    static std::vector<uint8_t> str_to_bytes(const std::string& str);
    static std::array<uint8_t, 7> str_to_bytes_array8(const std::string& str);
    static std::array<uint16_t, 7> str_to_bytes_array16(const std::string& str);
    static std::map<uint8_t, std::string> str_to_map(const std::string& input);
    static std::vector<uint8_t> str_to_bytes(const std::string& str, uint16_t value);
    static std::vector<uint8_t> replace_placeholders(const std::array<uint16_t, 7>& arr, uint16_t token, uint16_t value);

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