#pragma once

#include "esphome/components/daikin_rotex_can/types.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/select/select.h"
#include "esphome/components/number/number.h"
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

    static std::string to_hex(TMessage const& data);
    static bool find(std::string const& haystack, std::string const& needle);
    static std::vector<std::string> split(std::string const& str);
    static std::string to_hex(uint32_t value);
    static TMessage str_to_bytes(const std::string& str);
    static TMessage str_to_bytes_array8(const std::string& str);
    static void setBytes(TMessage& data, uint16_t value, uint8_t offset, uint8_t len);

    template<typename... Args>
    static void log(std::string const& tag, std::string const& str_format, Args... args);

    static sensor::Sensor* toSensor(EntityBase*);
    static text_sensor::TextSensor* toTextSensor(EntityBase*);
    static binary_sensor::BinarySensor* toBinarySensor(EntityBase*);
    static select::Select* toSelect(EntityBase*);
    static number::Number* toNumber(EntityBase*);

    static std::string g_log_filter;
};

}
}