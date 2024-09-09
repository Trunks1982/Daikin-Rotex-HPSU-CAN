#include "esphome/components/daikin_rotex_can/utils.h"
#include <regex>

namespace esphome {
namespace daikin_rotex_can {

std::string Utils::g_log_filter = "";

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

bool Utils::is_number(const std::string& str) {
    return !str.empty() && std::find_if(
        str.begin(),
        str.end(),
        [](unsigned char chr) { return !std::isdigit(chr); }
    ) == str.end();
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

std::vector<uint8_t> Utils::str_to_bytes(const std::string& str, uint16_t value) {
    std::vector<uint8_t> result;
    std::stringstream ss(str);
    std::string byteStr;

    // High- und Low-Bytes des Werts berechnen
    uint8_t high_byte = (value >> 8) & 0xFF;  // High byte
    uint8_t low_byte = value & 0xFF;          // Low byte

    bool replaced_double_placeholder = false;

    // String in Byte-Blöcke zerlegen
    while (ss >> byteStr) {
        if (byteStr == "__") {
            if (!replaced_double_placeholder) {
                // Prüfen, ob ein weiterer "__" folgt (doppelter Platzhalter)
                std::string next_byte_str;
                std::streampos pos = ss.tellg();  // Position speichern
                if (ss >> next_byte_str && next_byte_str == "__") {
                    // Doppelte Platzhalter gefunden, durch High- und Low-Byte ersetzen
                    result.push_back(high_byte);
                    result.push_back(low_byte);
                    replaced_double_placeholder = true;
                } else {
                    // Einzelner Platzhalter, rückgängig machen und nur ein Byte ersetzen
                    result.push_back(high_byte);  // oder ein anderes byte
                    replaced_double_placeholder = true;
                    ss.seekg(pos);  // Position zurücksetzen, um den nächsten Wert zu verarbeiten
                }
            } else {
                // Einzelner Platzhalter, nur ein Byte ersetzen
                result.push_back(low_byte);
            }
        } else {
            // Konvertiere Hex-Strings in uint8_t und füge sie dem Ergebnis hinzu
            uint8_t byte = static_cast<uint8_t>(std::stoi(byteStr, nullptr, 16));
            result.push_back(byte);
        }
    }

    return result;
}

std::vector<uint8_t> Utils::replace_placeholders(const std::array<uint16_t, 7>& arr, uint16_t token, uint16_t value) {
    std::vector<uint8_t> result;

    uint8_t high_byte = (value >> 8) & 0xFF;
    uint8_t low_byte = value & 0xFF;

    for (size_t i = 0; i < arr.size(); ++i) {
        if (arr[i] == token) {
            if (i + 1 < arr.size() && arr[i + 1] == token) {
                result.push_back(high_byte);
                result.push_back(low_byte);
                ++i;
            } else {
                result.push_back(low_byte);
            }
        } else {
            result.push_back(arr[i]);
        }
    }

    return result;
}


}
}