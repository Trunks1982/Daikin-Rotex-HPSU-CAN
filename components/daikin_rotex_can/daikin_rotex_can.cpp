#include "esphome/components/daikin_rotex_can/daikin_rotex_can.h"
#include "esphome/components/daikin_rotex_can/request.h"
#include "esphome/components/daikin_rotex_can/BidiMap.h"
#include <string>
#include <vector>

namespace esphome {
namespace daikin_rotex_can {

static const char* TAG = "daikin_rotex_can";

static const BidiMap<uint8_t, std::string> map_betriebsmodus {
    {0x01, "Bereitschaft"},
    {0x03, "Heizen"},
    {0x04, "Absenken"},
    {0x05, "Sommer"},
    {0x11, "Kühlen"},
    {0x0B, "Automatik 1"},
    {0x0C, "Automatik 2"}
};

static const BidiMap<uint8_t, std::string> map_betriebsart {
    {0x00, "Standby"},
    {0x01, "Heizen"},
    {0x02, "Kühlen"},
    {0x03, "Abtauen"},
    {0x04, "Warmwasserbereitung"}
};

static const BidiMap<uint8_t, std::string> map_hk_function {
    {0x00, "Witterungsgeführt"},
    {0x01, "Fest"}
};

static const BidiMap<uint8_t, std::string> map_sg_mode = {
    {0x00, "Aus"},
    {0x01, "SG Modus 1"},
    {0x02, "SG Modus 2"}
};

static const BidiMap<uint8_t, std::string> map_sg = {
    {0x00, "Aus"},
    {0x01, "An"}
};

const std::vector<TRequest> entity_config = {
    { // Betriebsmodus
        "operating_mode",
        {0x31, 0x00, 0xFA, 0x01, 0x12, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x01, 0x12,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_operating_mode(); },
        [](auto const& data, auto& accessor) -> DataType {
            const std::string mode = map_betriebsmodus.getValue(data[5]);
            if (accessor.get_operating_mode() != nullptr) {
                accessor.get_operating_mode()->publish_state(mode);
            }
            if (accessor.get_operating_mode_select() != nullptr) {
                accessor.get_operating_mode_select()->publish_state(mode);
            }
            return mode;
        }
    },
    { // Betriebsmodus setzen
        "operating_mode_select",
        [](auto& accessor) -> EntityBase* { return accessor.get_operating_mode_select(); },
        [](auto const& value) -> std::vector<uint8_t> {
            return {0x30, 0x00, 0xFA, 0x01, 0x12, static_cast<uint8_t>(value), 0x00};
        }
    },

    { // Betriebsart
        "mode_of_operating",
        {0x31, 0x00, 0xFA, 0xC0, 0xF6, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0xC0, 0xF6,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_mode_of_operating(); },
        [](auto const& data, auto& accessor) -> DataType {
            const uint32_t mode = uint32_t(data[6] + data[5]);

            const auto iter = map_betriebsart.findByKey(mode);
            const std::string str_mode = iter != map_betriebsart.end() ? iter->second : "Unknown";

            accessor.get_mode_of_operating()->publish_state(str_mode);

            accessor.getDaikinRotexCanComponent()->call_later([&accessor](){
                accessor.getDaikinRotexCanComponent()->update_thermal_power();
            });

            return str_mode;
        }
    },

    { // HK Function
        "hk_function",
        {0x31, 0x00, 0xFA, 0x01, 0x41, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x01, 0x41,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_hk_function(); },
        [](auto const& data, auto& accessor) -> DataType {
            const auto iter = map_hk_function.findByKey(data[6]);
            const std::string str_mode = iter != map_hk_function.end() ? iter->second : "Unknown";

            accessor.get_hk_function()->publish_state(str_mode);
            if (accessor.get_hk_function_select() != nullptr) {
                accessor.get_hk_function_select()->publish_state(str_mode);
            }
            return str_mode;
        }
    },
    { // HK Function Einstellen
        "hk_function_select",
        [](auto& accessor) -> EntityBase* { return accessor.get_hk_function_select(); },
        [](auto const& value) -> std::vector<uint8_t> {
            return { 0x30, 0x00, 0xFA, 0x01, 0x41, 0x00, static_cast<uint8_t>(value)};
        }
    },

    { // Status Kompressor
        "status_kompressor",
        {0xA1, 0x00, 0x61, 0x00, 0x00, 0x00, 0x00},
        0x500,
        {  DC,   DC, 0x61,   DC,   DC,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_status_kompressor(); },
        [](auto const& data, auto& accessor) -> DataType {
            const uint8_t state = data[3];
            accessor.get_status_kompressor()->publish_state(state);
            return state;
        }
    },

    { // Status Kessel
        "status_kesselpumpe",
        {0x31, 0x00, 0xFA, 0x0A, 0x8C, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x0A, 0x8C,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_status_kesselpumpe(); },
        [](auto const& data, auto& accessor) -> DataType {
            const float state = data[6];
            accessor.get_status_kesselpumpe()->publish_state(state);
            return state;
        }
    },

    { // Circulation Pump
        "circulation_pump_min",
        {0x31, 0x00, 0xFA, 0x06, 0x7F, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x06, 0x7F,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_circulation_pump_min(); },
        [](auto const& data, auto& accessor) -> DataType {
            const float percent = data[6];
            accessor.get_circulation_pump_min()->publish_state(percent);
            if (accessor.get_circulation_pump_min_set() != nullptr) {
                accessor.get_circulation_pump_min_set()->publish_state(percent);
            }
            return percent;
        }
    },
    { // Circulation Pump Min Set
        "circulation_pump_min",
        [](auto& accessor) -> EntityBase* { return accessor.get_circulation_pump_min(); },
        [](auto const& value) -> std::vector<uint8_t> {
            const uint16_t temp = static_cast<uint16_t>(value);
            const uint8_t hi_byte = temp >> 8;
            const uint8_t lo_byte = temp & 0xFF;
            return {0x30, 0x00, 0xFA, 0x06, 0x7F, hi_byte, lo_byte};
        }
    },

    { // Umwälzpumpe Max
        "circulation_pump_max",
        {0x31, 0x00, 0xFA, 0x06, 0x7E, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x06, 0x7E,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_circulation_pump_max(); },
        [](auto const& data, auto& accessor) -> DataType {
            const float percent = data[6];
            accessor.get_circulation_pump_max()->publish_state(percent);
            if (accessor.get_circulation_pump_max_set() != nullptr) {
                accessor.get_circulation_pump_max_set()->publish_state(percent);
            }
            return percent;
        }
    },
    { // Circulation Pump Max Set
        "circulation_pump_max",
        [](auto& accessor) -> EntityBase* { return accessor.get_circulation_pump_max(); },
        [](auto const& value) -> std::vector<uint8_t> {
            const uint16_t temp = static_cast<uint16_t>(value);
            const uint8_t hi_byte = temp >> 8;
            const uint8_t lo_byte = temp & 0xFF;
            return {0x30, 0x00, 0xFA, 0x06, 0x7E, hi_byte, lo_byte};
        }
    },

    { // T-WW-Soll1
        "target_hot_water_temperature",
        {0x31, 0x00, 0x13, 0x00, 0x00, 0x00, 0x00},
        {0xD2, 0x00, 0x13,   DC,   DC, 0x00, 0x00},
        [](auto& accessor) -> EntityBase* { return accessor.get_target_hot_water_temperature(); },
        [](auto const& data, auto& accessor) -> DataType {
            const float temp = ((data[3] << 8) + data[4]) / 10.0f;

            accessor.get_target_hot_water_temperature()->publish_state(temp);
            if (accessor.get_target_hot_water_temperature_set() != nullptr) {
                accessor.get_target_hot_water_temperature_set()->publish_state(temp);
            }

            accessor.getDaikinRotexCanComponent()->call_later([&](){
                accessor.getDaikinRotexCanComponent()->run_dhw_lambdas();
            });

            return temp;
        }
    },
    { // WW Einstellen
        "target_hot_water_temperature_set",
        [](auto& accessor) -> EntityBase* { return accessor.get_target_hot_water_temperature_set(); },
        [](auto const& value) -> std::vector<uint8_t> {
            const uint16_t temp = (uint16_t)(value * 10);
            const uint8_t hi_byte = temp >> 8;
            const uint8_t lo_byte = temp & 0xFF;
            return {0x30, 0x00, 0x13, hi_byte, lo_byte, 0x00, 0x00};
        }
    },

    { // T Vorlauf Tag
        "flow_temperature_day",
        {0x31, 0x00, 0xFA, 0x01, 0x29, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x01, 0x29,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_flow_temperature_day(); },
        [](auto const& data, auto& accessor) -> DataType {
            const float temp = ((data[5] << 8) + data[6]) / 10.0f;
            accessor.get_flow_temperature_day()->publish_state(temp);
            if (accessor.get_flow_temperature_day_set() != nullptr) {
                accessor.get_flow_temperature_day_set()->publish_state(temp);
            }
            return temp;
        }
    },
    { // T Vorlauf Tag Einstellen
        "flow_temperature_day_set",
        [](auto& accessor) -> EntityBase* { return accessor.get_flow_temperature_day_set(); },
        [](auto const& value) -> std::vector<uint8_t> {
            const uint16_t temp = (uint16_t)(value * 10);
            const uint8_t hi_byte = temp >> 8;
            const uint8_t lo_byte = temp & 0xFF;
            return {0x30, 0x00, 0xFA,  0x01, 0x29, hi_byte, lo_byte};
        }
    },

    { // Heizkurve
        "heating_curve",
        {0x31, 0x00, 0xFA, 0x01, 0x0E, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x01, 0x0E,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_heating_curve(); },
        [](auto const& data, auto& accessor) -> DataType {
            const float value = ((data[5] << 8) + data[6]) / 100.0f;
            accessor.get_heating_curve()->publish_state(value);
            if (accessor.get_heating_curve_set() != nullptr) {
                accessor.get_heating_curve_set()->publish_state(value);
            }
            return value;
        }
    },
    { // Heizkurve einstellen
        "heating_curve_set",
        [](auto& accessor) -> EntityBase* { return accessor.get_heating_curve_set(); },
        [](auto const& value) -> std::vector<uint8_t> {
            const uint16_t hk = (uint16_t)(value * 100);
            const uint8_t hi_byte = hk >> 8;
            const uint8_t lo_byte = hk & 0xFF;
            return {0x30, 0x00, 0xFA, 0x01, 0x0E, hi_byte, lo_byte};
        }
    },

    { // Min VL Soll
        "min_target_supply_temperature",
        {0x31, 0x00, 0xFA, 0x01, 0x2B, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x01, 0x2B,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_min_target_supply_temperature(); },
        [](auto const& data, auto& accessor) -> DataType {
            const float temp = ((data[5] << 8) + data[6]) / 10.0f;
            accessor.get_min_target_supply_temperature()->publish_state(temp);
            if (accessor.get_min_target_flow_temp_set() != nullptr) {
                accessor.get_min_target_flow_temp_set()->publish_state(temp);
            }
            return temp;
        }
    },
    { // Min VL Einstellen
        "min_target_flow_temp_set",
        [](auto& accessor) -> EntityBase* { return accessor.get_min_target_flow_temp_set(); },
        [](auto const& value) -> std::vector<uint8_t> {
            const uint16_t temp = (uint16_t)(value * 10);
            const uint8_t hi_byte = temp >> 8;
            const uint8_t lo_byte = temp & 0xFF;
            return {0x30, 0x00, 0xFA, 0x01, 0x2B, hi_byte, lo_byte};
        }
    },

    { // Max VL Soll
        "max_target_supply_temperature",
        {0x31, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00},
        {  DC,   DC, 0x28,   DC,   DC,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_max_target_supply_temperature(); },
        [](auto const& data, auto& accessor) -> DataType {
            const float temp = ((data[3] << 8) + data[4]) / 10.0f;
            accessor.get_max_target_supply_temperature()->publish_state(temp);
            if (accessor.get_max_target_flow_temp_set() != nullptr) {
                accessor.get_max_target_flow_temp_set()->publish_state(temp);
            }
            return temp;
        }
    },
    { // Max VL Einstellen
        "max_target_flow_temp_set",
        [](auto& accessor) -> EntityBase* { return accessor.get_max_target_flow_temp_set(); },
        [](auto const& value) -> std::vector<uint8_t> {
            const uint16_t temp = static_cast<uint16_t>(value * 10);
            const uint8_t hi_byte = temp >> 8;
            const uint8_t lo_byte = temp & 0xFF;
            return {0x30, 0x00, 0x28, hi_byte, lo_byte, 0x00, 0x00};
        }
    },

    { // SGModus
        "sg_mode",
        {0x31, 0x00, 0xFA, 0x06, 0x94, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x06, 0x94,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_sg_mode(); },
        [](auto const& data, auto& accessor) -> DataType {
            const std::string mode = map_sg_mode.getValue(data[6]);
            accessor.get_sg_mode()->publish_state(mode);
            if (accessor.get_sg_mode_select() != nullptr) {
                accessor.get_sg_mode_select()->publish_state(mode);
            }
            return mode;
        }
    },
    {
        "sg_mode_select",
        [](auto& accessor) -> EntityBase* { return accessor.get_sg_mode_select(); },
        [](auto const& value) -> std::vector<uint8_t> {
            return {0x30, 0x00, 0xFA, 0x06, 0x94, 0x00, static_cast<uint8_t>(value)};
        }
    },

    { // Smart Grid
        "smart_grid",
        {0x31, 0x00, 0xFA, 0x06, 0x93, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x06, 0x93,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_smart_grid(); },
        [](auto const& data, auto& accessor) -> DataType {
            const std::string state = map_sg.getValue(data[6]);
            accessor.get_smart_grid()->publish_state(state);
            if (accessor.get_smart_grid_select() != nullptr) {
                accessor.get_smart_grid_select()->publish_state(state);
            }
            return state;
        }
    },
    {
        "smart_grid_select",
        [](auto& accessor) -> EntityBase* { return accessor.get_smart_grid_select(); },
        [](auto const& value) -> std::vector<uint8_t> {
            return {0x30, 0x00, 0xFA, 0x06, 0x93, 0x00, static_cast<uint8_t>(value)};
        }
    },

    { // Raumsoll 1 Einstellen
        "target_room1_temperature_set",
        [](auto& accessor) -> EntityBase* { return accessor.get_target_room1_temperature_set(); },
        [](auto const& value) -> std::vector<uint8_t> {
            const uint16_t temp = static_cast<uint16_t>(value * 10);
            const uint8_t hi_byte = temp >> 8;
            const uint8_t lo_byte = temp & 0xFF;
            return {0x30, 0x00, 0x05, hi_byte, lo_byte, 0x00, 0x00};
        }
    },

    { // Fehlercode
        "error_code",
        {0x31, 0x00, 0xFA, 0x13, 0x88, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x13, 0x88,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_error_code(); },
        [](auto const& data, auto& accessor) -> DataType {
            const uint32_t code = (data[5] << 8) + data[6];

            const auto to_str = [](uint32_t code) -> std::string {
                switch (code)
                {
                case 0: return {"Kein Fehler"};
                case 9001: return {"E9001 Rücklauffühler"};
                case 9002: return {"E9002 Vorlauffühler"};
                case 9003: return {"E9003 Frostschutzfunktion"};
                case 9004: return {"E9004 Durchfluss"};
                case 9005: return {"E9005 Vorlauftemperaturfühler"};
                case 9006: return {"E9006 Vorlauftemperaturfühler"};
                case 9007: return {"E9007 Platine IG defekt"};
                case 9008: return {"E9008 Kältemitteltemperatur außerhalb des Bereiches"};
                case 9009: return {"E9009 STB Fehler"};
                case 9010: return {"E9010 STB Fehler"};
                case 9011: return {"E9011 Fehler Flowsensor"};
                case 9012: return {"E9012 Fehler Vorlauffühler"};
                case 9013: return {"E9013 Platine AG defekt"};
                case 9014: return {"E9014 P-Kältemittel hoch"};
                case 9015: return {"E9015 P-Kältemittel niedrig"};
                case 9016: return {"E9016 Lastschutz Verdichter"};
                case 9017: return {"E9017 Ventilator blockiert"};
                case 9018: return {"E9018 Expansionsventil"};
                case 9019: return {"E9019 Warmwassertemperatur > 85°C"};
                case 9020: return {"E9020 T-Verdampfer hoch"};
                case 9021: return {"E9021 HPS-System"};
                case 9022: return {"E9022 Fehler AT-Fühler"};
                case 9023: return {"E9023 Fehler WW-Fühler"};
                case 9024: return {"E9024 Drucksensor"};
                case 9025: return {"E9025 Fehler Rücklauffühler"};
                case 9026: return {"E9026 Drucksensor"};
                case 9027: return {"E9027 Aircoil-Fühler Defrost"};
                case 9028: return {"E9028 Aircoil-Fühler temp"};
                case 9029: return {"E9029 Fehler Kältefühler AG"};
                case 9030: return {"E9030 Defekt elektrisch"};
                case 9031: return {"E9031 Defekt elektrisch"};
                case 9032: return {"E9032 Defekt elektrisch"};
                case 9033: return {"E9033 Defekt elektrisch"};
                case 9034: return {"E9034 Defekt elektrisch"};
                case 9035: return {"E9035 Platine AG defekt"};
                case 9036: return {"E9036 Defekt elektrisch"};
                case 9037: return {"E9037 Einstellung Leistung"};
                case 9038: return {"E9038 Kältemittel Leck"};
                case 9039: return {"E9039 Unter/Überspannung"};
                case 9041: return {"E9041 Übertragungsfehler"};
                case 9042: return {"E9042 Übertragungsfehler"};
                case 9043: return {"E9043 Übertragungsfehler"};
                case 9044: return {"E9044 Übertragungsfehler"};
                case 75: return {"E75 Fehler Außentemperaturfühler"};
                case 76: return {"E76 Fehler Speichertemperaturfühler"};
                case 81: return {"E81 Kommunikationsfehler Rocon"};
                case 88: return {"E88 Kommunikationsfehler Rocon Handbuch"};
                case 91: return {"E91 Kommunikationsfehler Rocon Handbuch"};
                case 128: return {"E128 Fehler Rücklauftemperaturfühler"};
                case 129: return {"E129 Fehler Drucksensor"};
                case 198: return {"E198 Durchflussmessung nicht plausibel"};
                case 200: return {"E200 Kommunikationsfehler"};
                case 8005: return {"E8005 Wasserdruck in Heizungsanlage zu gering"};
                case 8100: return {"E8100 Kommunikation"};
                case 9000: return {"E9000 Interne vorübergehende Meldung"};
                case 8006: return {"W8006 Warnung Druckverlust"};
                case 8007: return {"W8007 Wasserdruck in Anlage zu hoch"};
                default:
                    return {"Unknown"};
                }
            };

            std::string str_code = to_str(code);

            accessor.get_error_code()->publish_state(str_code);
            return str_code;
        }
    },
};

DaikinRotexCanComponent::DaikinRotexCanComponent()
: m_accessor(this)
, m_data_requests(std::move(entity_config))
, m_later_calls()
, m_dhw_run_lambdas()
{
}

void DaikinRotexCanComponent::validateConfig() {
    for (auto const& pair : m_accessor.get_sensors()) {
        const std::array<uint8_t, 7> data = Utils::str_to_bytes_array8(pair.second.data);
        const std::array<uint16_t, 7> expected_response = Utils::str_to_bytes_array16(pair.second.expected_response);

        m_data_requests.add({
            pair.second.id,
            data,
            expected_response,
            [pair](auto& accessor) -> EntityBase* { return pair.second.pSensor; },
            [pair, this](auto const& data, auto& accessor) -> DataType {
                float value = 0;

                if (pair.second.data_offset > 0 && (pair.second.data_offset + pair.second.data_size) <= 7) {
                    switch (pair.second.data_size) {
                    case 1:
                        value = data[pair.second.data_offset] / pair.second.divider;
                        pair.second.pSensor->publish_state(value);
                        break;
                    case 2:
                        value = ((data[pair.second.data_offset] << 8) + data[pair.second.data_offset + 1]) / pair.second.divider;
                        pair.second.pSensor->publish_state(value);
                        break;
                    default:
                        call_later([pair](){
                            ESP_LOGE("validateConfig", "Invalid data size: %d", pair.second.data_size);
                        });
                    }
                } else {
                    call_later([pair](){
                        ESP_LOGE("validateConfig", "Invalid data_offset: %d", pair.second.data_offset);
                    });
                }

                call_later([pair, value, this](){
                    if (!pair.second.update_entity.empty()) {
                        updateState(pair.second.update_entity);
                    }
                });

                call_later([pair, value, &accessor, this](){
                    if (!pair.second.set_entity.empty()) {
                        TRequest const* pRequest = m_data_requests.get(pair.second.set_entity);
                        if (pRequest != nullptr) {
                            EntityBase* pEntity = pRequest->getEntity(accessor);
                            if (number::Number* pNumber = dynamic_cast<number::Number*>(pEntity)) {
                                pNumber->publish_state(value);
                            }
                        } else {
                            ESP_LOGE("handle ", "id<%s>.set_entity<%s> is invalid!", 
                                pair.second.id.c_str(), pair.second.set_entity.c_str());
                        }
                    }
                });

                return value;
            }
        });
    }

    m_data_requests.removeInvalidRequests(m_accessor);
}

void DaikinRotexCanComponent::setup() {
    ESP_LOGI(TAG, "setup");
}

void DaikinRotexCanComponent::updateState(std::string const& id) {
    if (id == "thermal_power") {
        update_thermal_power();
    }
}

void DaikinRotexCanComponent::update_thermal_power() {
    text_sensor::TextSensor* mode_of_operating = m_accessor.get_mode_of_operating();
    sensor::Sensor* thermal_power = m_accessor.get_thermal_power();

    if (mode_of_operating != nullptr && thermal_power != nullptr) {
        // TODO: Type-safe casts
        TRequest const* water_flow_request = m_data_requests.get("water_flow");
        sensor::Sensor* water_flow = static_cast<sensor::Sensor*>(water_flow_request->getEntity(m_accessor));
        sensor::Sensor* tvbh = static_cast<sensor::Sensor*>(m_data_requests.get("tvbh")->getEntity(m_accessor));
        sensor::Sensor* tv = static_cast<sensor::Sensor*>(m_data_requests.get("tv")->getEntity(m_accessor));
        sensor::Sensor* tr = static_cast<sensor::Sensor*>(m_data_requests.get("tr")->getEntity(m_accessor));

        float value = 0;
        if (mode_of_operating->state == "Warmwasserbereitung" && tv != nullptr && tr != nullptr && water_flow != nullptr) {
            value = (tv->state - tr->state) * (4.19 * water_flow->state) / 3600.0f;
        } else if (mode_of_operating->state == "Heizen" && tvbh != nullptr && tr != nullptr && water_flow != nullptr) {
            value = (tvbh->state - tr->state) * (4.19 * water_flow->state) / 3600.0f;
        } else if (mode_of_operating->state == "Kühlen" && tvbh != nullptr && tr != nullptr && water_flow != nullptr) {
            value = (tvbh->state - tr->state) * (4.19 * water_flow->state) / 3600.0f;
        }
        thermal_power->publish_state(value);
    }
}

///////////////// Texts /////////////////
void DaikinRotexCanComponent::custom_request(std::string const& value) {
    const uint32_t can_id = 0x680;
    const bool use_extended_id = false;

    const std::vector<uint8_t> buffer = Utils::str_to_bytes(value);

    if (!buffer.empty()) {
        esphome::esp32_can::ESP32Can* pCanbus = m_data_requests.getCanbus();
        pCanbus->send_data(can_id, use_extended_id, { buffer.begin(), buffer.end() });

        Utils::log("sendGet", "can_id<%s> data<%s>",
            Utils::to_hex(can_id).c_str(), value.c_str(), Utils::to_hex(buffer).c_str());
    }
}

///////////////// Selects /////////////////
void DaikinRotexCanComponent::set_operation_mode(std::string const& mode) {
    m_data_requests.sendSet(m_accessor, m_accessor.get_operating_mode_select()->get_name(), map_betriebsmodus.getKey(mode));
    m_data_requests.sendGet(m_accessor, m_accessor.get_operating_mode()->get_name());
}

void DaikinRotexCanComponent::set_hk_function(std::string const& mode) {
    m_data_requests.sendSet(m_accessor, m_accessor.get_hk_function_select()->get_name(), map_hk_function.getKey(mode));
    m_data_requests.sendGet(m_accessor, m_accessor.get_hk_function()->get_name());
}

void DaikinRotexCanComponent::set_sg_mode(std::string const& mode) {
    m_data_requests.sendSet(m_accessor, m_accessor.get_sg_mode_select()->get_name(), map_sg_mode.getKey(mode));
    m_data_requests.sendGet(m_accessor, m_accessor.get_sg_mode()->get_name());
}

void DaikinRotexCanComponent::set_smart_grid(std::string const& mode) {
    m_data_requests.sendSet(m_accessor, m_accessor.get_smart_grid_select()->get_name(), map_sg.getKey(mode));
    m_data_requests.sendGet(m_accessor, m_accessor.get_smart_grid()->get_name());
}

///////////////// Numbers /////////////////
void DaikinRotexCanComponent::set_target_hot_water_temperature(float temperature) {
    m_data_requests.sendSet(m_accessor, m_accessor.get_target_hot_water_temperature_set()->get_name(), temperature);
    m_data_requests.sendGet(m_accessor, m_accessor.get_target_hot_water_temperature()->get_name());
}

void DaikinRotexCanComponent::set_target_room1_temperature(float temperature) {
    m_data_requests.sendSet(m_accessor, m_accessor.get_target_room1_temperature_set()->get_name(), temperature);
    TRequest const* pRequest = m_data_requests.get("target_room1_temperature");
    m_data_requests.sendGet(m_accessor, pRequest->getName(m_accessor));
}

void DaikinRotexCanComponent::set_flow_temperature_day(float temperature) {
    m_data_requests.sendSet(m_accessor, m_accessor.get_flow_temperature_day_set()->get_name(), temperature);
    m_data_requests.sendGet(m_accessor, m_accessor.get_flow_temperature_day()->get_name());
}

void DaikinRotexCanComponent::set_max_target_flow_temp(float temperature) {
    m_data_requests.sendSet(m_accessor, m_accessor.get_max_target_flow_temp_set()->get_name(), temperature);
    m_data_requests.sendGet(m_accessor, m_accessor.get_max_target_supply_temperature()->get_name());
}

void DaikinRotexCanComponent::set_min_target_flow_temp(float temperature) {
    m_data_requests.sendSet(m_accessor, m_accessor.get_min_target_flow_temp_set()->get_name(), temperature);
    m_data_requests.sendGet(m_accessor, m_accessor.get_min_target_supply_temperature()->get_name());
}

void DaikinRotexCanComponent::set_heating_curve(float heating_curve) {
    m_data_requests.sendSet(m_accessor, m_accessor.get_heating_curve_set()->get_name(), heating_curve);
    m_data_requests.sendGet(m_accessor, m_accessor.get_heating_curve()->get_name());
}

void DaikinRotexCanComponent::set_circulation_pump_min(uint8_t percent) {
    m_data_requests.sendSet(m_accessor, m_accessor.get_circulation_pump_min_set()->get_name(), percent);
    m_data_requests.sendGet(m_accessor, m_accessor.get_circulation_pump_min()->get_name());
}

void DaikinRotexCanComponent::set_circulation_pump_max(uint8_t percent) {
    m_data_requests.sendSet(m_accessor, m_accessor.get_circulation_pump_max_set()->get_name(), percent);
    m_data_requests.sendGet(m_accessor, m_accessor.get_circulation_pump_max()->get_name());
}


///////////////// Buttons /////////////////
void DaikinRotexCanComponent::dhw_run() {
    const float temp = m_accessor.get_target_hot_water_temperature()->get_raw_state();

    m_data_requests.sendSet(m_accessor, m_accessor.get_target_hot_water_temperature_set()->get_name(), 70);
    m_data_requests.sendGet(m_accessor, m_accessor.get_target_hot_water_temperature()->get_name());

    m_dhw_run_lambdas.push_back([temp, this]() {
        m_data_requests.sendSet(m_accessor, m_accessor.get_target_hot_water_temperature_set()->get_name(), temp);
        m_data_requests.sendGet(m_accessor, m_accessor.get_target_hot_water_temperature()->get_name());
    });
}

void DaikinRotexCanComponent::run_dhw_lambdas() {
    if (m_accessor.getDaikinRotexCanComponent() != nullptr) {
        if (!m_dhw_run_lambdas.empty()) {
            auto& lambda = m_dhw_run_lambdas.front();
            lambda();
            m_dhw_run_lambdas.pop_front();
        }
    }
}

void DaikinRotexCanComponent::loop() {
    m_data_requests.sendNextPendingGet(m_accessor);
    for (auto it = m_later_calls.begin(); it != m_later_calls.end(); ) {
        if (millis() > it->second) {
            it->first();
            it = m_later_calls.erase(it);
        } else {
            ++it;
        }
    }
}

void DaikinRotexCanComponent::handle(uint32_t can_id, std::vector<uint8_t> const& data) {
    m_data_requests.handle(m_accessor, can_id, data);
}

void DaikinRotexCanComponent::dump_config() {
    ESP_LOGCONFIG(TAG, "DaikinRotexCanComponent");
}

} // namespace daikin_rotex_can
} // namespace esphome