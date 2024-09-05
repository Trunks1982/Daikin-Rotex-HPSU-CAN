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
    { // Aussentemperatur
        {0x31, 0x00, 0xFA, 0xC0, 0xFF, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0xC0, 0xFF,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_temperature_outside(); },
        [](auto const& data, auto& accessor) -> DataType {
            const float temp = float(((int((data[6]) + ((data[5]) << 8))) ^ 0x8000) - 0x8000) / 10;
            accessor.get_temperature_outside()->publish_state(temp);
            return temp;
        }
    },
    { // tdhw1
        {0x31, 0x00, 0xFA, 0x00, 0x0E, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x00, 0x0E,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_tdhw1(); },
        [](auto const& data, auto& accessor) -> DataType {
            const float temp = float((float((int((data[6]) + ((data[5]) << 8))))/10));
            accessor.get_tdhw1()->publish_state(temp);
            return temp;
        }
    },
    { // Vorlauftemperatur
        {0x31, 0x00, 0xFA, 0xC0, 0xFC, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0xC0, 0xFC,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_tv(); },
        [](auto const& data, auto& accessor) -> DataType {
            const float temp = float(float((int((data[6]) + ((data[5]) << 8))))/10);
            accessor.get_tv()->publish_state(temp);
            accessor.update_thermal_power();
            return temp;
        }
    },
    { // Vorlauftemperatur (TVBH)
        {0x31, 0x00, 0xFA, 0xC1, 0x02, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0xC1, 0x02,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_tvbh(); },
        [](auto const& data, auto& accessor) -> DataType {
            const float temp = float(float((int((data[6]) + ((data[5]) << 8))))/10);
            accessor.get_tvbh()->publish_state(temp);
            accessor.update_thermal_power();
            return temp;
        }
    },
    { // Rücklauftemperatur Heizung
        {0x31, 0x00, 0xFA, 0xC1, 0x00, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0xC1, 0x00,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_tr(); },
        [](auto const& data, auto& accessor) -> DataType {
            const float temp = float(float((int((data[6]) + ((data[5]) << 8))))/10);
            accessor.get_tr()->publish_state(temp);
            accessor.update_thermal_power();
            return temp;
        }
    },
    { // Water Pressure
        {0x31, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00},
        {0xD2,   DC, 0x1C,   DC,   DC,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_water_pressure(); },
        [](auto const& data, auto& accessor) -> DataType {
            const float pressure = float((float((int((data[4]) + ((data[3]) << 8))))/1000));
            accessor.get_water_pressure()->publish_state(pressure);
            return pressure;
        }
    },
    { // Durchfluss
        {0x31, 0x00, 0xFA, 0x01, 0xDA, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x01, 0xDA,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_water_flow(); },
        [](auto const& data, auto& accessor) -> DataType {
            const float flow = float((float((int((data[6]) + ((data[5]) << 8))))));
            accessor.get_water_flow()->publish_state(flow);
            accessor.update_thermal_power();
            return flow;
        }
    },

    { // Betriebsmodus
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
        [](auto& accessor) -> EntityBase* { return accessor.get_operating_mode_select(); },
        [](auto const& value) -> std::vector<uint8_t> {
            return {0x30, 0x00, 0xFA, 0x01, 0x12, static_cast<uint8_t>(value), 0x00};
        }
    },

    { // Betriebsart
        {0x31, 0x00, 0xFA, 0xC0, 0xF6, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0xC0, 0xF6,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_mode_of_operating(); },
        [](auto const& data, auto& accessor) -> DataType {
            const uint32_t mode = uint32_t(data[6] + data[5]);

            const auto iter = map_betriebsart.findByKey(mode);
            const std::string str_mode = iter != map_betriebsart.end() ? iter->second : "Unknown";

            accessor.get_mode_of_operating()->publish_state(str_mode);
            accessor.update_thermal_power();
            return str_mode;
        }
    },

    { // HK Function
        {0x31, 0x00, 0xFA, 0x01, 0x41, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x01, 0x41,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_hk_function(); },
        [](auto const& data, auto& accessor) -> DataType {
            const auto iter = map_hk_function.findByKey(data[6]);
            const std::string str_mode = iter != map_hk_function.end() ? iter->second : "Unknown";

            accessor.get_hk_function()->publish_state(str_mode);
            return str_mode;
        }
    },

    { // Status Kompressor
        {0xA1, 0x00, 0x61, 0x00, 0x00, 0x00, 0x00},
        0x500,
        {  DC,   DC, 0x61,   DC,   DC,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_status_kompressor(); },
        [](auto const& data, auto& accessor) -> DataType {
            const float state = float((float((int((data[3]) + ((data[4]) << 8))))));
            accessor.get_status_kompressor()->publish_state(state);
            return state;
        }
    },

    { // Status Kessel
        {0x31, 0x00, 0xFA, 0x0A, 0x8C, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x0A, 0x8C,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_status_kesselpumpe(); },
        [](auto const& data, auto& accessor) -> DataType {
            const float state = float((float((int((data[6]) + ((data[5]) << 8))))));
            accessor.get_status_kesselpumpe()->publish_state(state);
            return state;
        }
    },

    { // Umwälzpumpe
        {0x31, 0x00, 0xFA, 0xC0, 0xF7, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0xC0, 0xF7,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_circulation_pump(); },
        [](auto const& data, auto& accessor) -> DataType {
            const float percent = int((data[6]) + data[5]);
            accessor.get_circulation_pump()->publish_state(percent);
            return percent;
        }
    },

    { // T-WW-Soll1
        {0x31, 0x00, 0x13, 0x00, 0x00, 0x00, 0x00},
        {0xD2, 0x00, 0x13,   DC,   DC, 0x00, 0x00},
        [](auto& accessor) -> EntityBase* { return accessor.get_target_hot_water_temperature(); },
        [](auto const& data, auto& accessor) -> DataType {
            const uint16_t temperature_raw = (data[3] << 8) | data[4];
            const float temp = static_cast<float>(temperature_raw) / 10.0;
            accessor.get_target_hot_water_temperature()->publish_state(temp);
            //id(ww_soll).publish_state(temperature);
            //id(ww_soll_set).publish_state(temperature);
            return temp;
        }
    },
    { // WW Einstellen
        [](auto& accessor) -> EntityBase* { return accessor.get_target_hot_water_temperature(); }, // Falscher Sensor!!!!!?????
        [](auto const& value) -> std::vector<uint8_t> {
            uint16_t temperature = (uint16_t)(value * 10);
            uint8_t high_byte = temperature >> 8;
            uint8_t low_byte = temperature & 0xFF;
            return { 0x30, 0x00, 0x13, high_byte, low_byte, 0x00, 0x00 };
        }
    },

    { // BPV
        {0x31, 0x00, 0xFA, 0xC0, 0xFB, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0xC0, 0xFB,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_bypass_valve(); },
        [](auto const& data, auto& accessor) -> DataType {
            const uint32_t value = uint32_t((data[6]) + (data[5] << 8));
            accessor.get_bypass_valve()->publish_state(value);
            return value;
        }
    },

    { // DHW Mischer Position
        {0x31, 0x00, 0xFA, 0x06, 0x9B, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x06, 0x9B,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_dhw_mixer_position(); },
        [](auto const& data, auto& accessor) -> DataType {
            const uint32_t position = uint32_t(data[6]) + uint32_t(data[5] << 8);
            accessor.get_dhw_mixer_position()->publish_state(position);
            return position;
        }
    },

    { // T Vorlauf Tag
        {0x31, 0x00, 0xFA, 0x01, 0x29, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x01, 0x29,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_daytime_supply_temperature(); },
        [](auto const& data, auto& accessor) -> DataType {
            float temp = float((float((int((data[6]) + ((data[5]) << 8))))/10));
            accessor.get_daytime_supply_temperature()->publish_state(temp);
            //id(t_vorlauf_tag_set).publish_state(temp);
            return temp;
        }
    },

    { // VL Soll
        {0x31, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00},
        {0xD2,   DC, 0x02,   DC,   DC,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_target_supply_temperature(); },
        [](auto const& data, auto& accessor) -> DataType {
            const float temp = ((data[3] << 8) | data[4]) / 10.0f;
            accessor.get_target_supply_temperature()->publish_state(temp);
            return temp;
        }
    },

    { // Heizkurve
        {0x31, 0x00, 0xFA, 0x01, 0x0E, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x01, 0x0E,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_heating_curve(); },
        [](auto const& data, auto& accessor) -> DataType {
            const float value = (uint32_t(data[6]) + (data[5] << 8)) / 100.0f;
            accessor.get_heating_curve()->publish_state(value);
            if (accessor.get_heating_curve_set() != nullptr) {
                accessor.get_heating_curve_set()->publish_state(value);
            }
            return value;
        }
    },

    { // Heizkurve einstellen
        [](auto& accessor) -> EntityBase* { return accessor.get_heating_curve_set(); },
        [](auto const& value) -> std::vector<uint8_t> {
            const uint16_t hk = (uint16_t)(value * 100);
            const uint8_t high_byte = hk >> 8;
            const uint8_t low_byte = hk & 0xFF;

            return { 0x30, 0x00, 0xFA, 0x01, 0x0E, high_byte, low_byte };
        }
    },

    { // EHS für CH
        {0x31, 0x00, 0xFA, 0x09, 0x20, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x09, 0x20,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_ehs_for_ch(); },
        [](auto const& data, auto& accessor) -> DataType {
            const float temp = float((int((data[6]) + ((data[5]) << 8))));
            accessor.get_ehs_for_ch()->publish_state(temp);
            return temp;
        }
    },

    { // Erzeugte Energie gesamt
        {0x31, 0x00, 0xFA, 0x09, 0x30, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x09, 0x30,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_total_energy_produced(); },
        [](auto const& data, auto& accessor) -> DataType {
            float value = float((float((int((data[6]) + ((data[5]) << 8))))));
            accessor.get_total_energy_produced()->publish_state(value);
            return value;
        }
    },

    { // Laufzeit Kompressor
        {0x31, 0x00, 0xFA, 0x06, 0xA5, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x06, 0xA5,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_runtime_compressor(); },
        [](auto const& data, auto& accessor) -> DataType {
            const float hours = float((int((data[6]) + ((data[5]) << 8))));
            accessor.get_runtime_compressor()->publish_state(hours);
            return hours;
        }
    },

    { // Laufzeit Pumpe
        {0x31, 0x00, 0xFA, 0x06, 0xA4, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x06, 0xA4,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_runtime_pump(); },
        [](auto const& data, auto& accessor) -> DataType {
            const float hours = float((float((int((data[6]) + ((data[5]) << 8))))));
            accessor.get_runtime_pump()->publish_state(hours);
            return hours;
        }
    },

    { // Min VL Soll
        {0x31, 0x00, 0xFA, 0x01, 0x2B, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x01, 0x2B,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_min_target_supply_temperature(); },
        [](auto const& data, auto& accessor) -> DataType {
            const float temp = float(float((int((data[6]) + ((data[5]) << 8))))/10);
            accessor.get_min_target_supply_temperature()->publish_state(temp);
            //id(min_vl_soll_set).publish_state(temp);
            return temp;
        }
    },

    { // Max VL Soll
        {0x31, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00},
        {  DC,   DC, 0x28,   DC,   DC,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_max_target_supply_temperature(); },
        [](auto const& data, auto& accessor) -> DataType {
            const float temp = float((float((int((data[4]) + ((data[3]) << 8))))/10));
            accessor.get_max_target_supply_temperature()->publish_state(temp);
            //id(max_vl_soll_set).publish_state(temp);
            return temp;
        }
    },

    { // Spreizung MOD HZ
        {0x31, 0x00, 0xFA, 0x06, 0x83, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x06, 0x83,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_spreizung_mod_hz(); },
        [](auto const& data, auto& accessor) -> DataType {
            float temp = float((float((int((data[6]) + ((data[5]) << 8))))/10));
            accessor.get_spreizung_mod_hz()->publish_state(temp);
            return temp;
        }
    },
    { // Spreizung MOD WW
        {0x31, 0x00, 0xFA, 0x06, 0x84, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x06, 0x84,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_spreizung_mod_ww(); },
        [](auto const& data, auto& accessor) -> DataType {
            float temp = float((float((int((data[6]) + ((data[5]) << 8))))/10));
            accessor.get_spreizung_mod_ww()->publish_state(temp);
            return temp;
        }
    },

    { // SGModus
        {0x31, 0x00, 0xFA, 0x06, 0x94, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x06, 0x94,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_sg_mode(); },
        [](auto const& data, auto& accessor) -> DataType {
            const std::string mode = map_sg_mode.getValue(data[6]);
            accessor.get_sg_mode()->publish_state(mode);
            return mode;
        }
    },
    { // Smart Grid
        {0x31, 0x00, 0xFA, 0x06, 0x93, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x06, 0x93,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_smart_grid(); },
        [](auto const& data, auto& accessor) -> DataType {
            const std::string state = map_sg.getValue(data[6]);
            accessor.get_smart_grid()->publish_state(state);
            return state;
        }
    },

    { // Fehlercode
        {0x31, 0x00, 0xFA, 0x13, 0x88, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x13, 0x88,   DC,   DC},
        [](auto& accessor) -> EntityBase* { return accessor.get_error_code(); },
        [](auto const& data, auto& accessor) -> DataType {
            const uint32_t code = uint32_t(data[6]) + (uint32_t(data[5]) << 8);

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
: m_accessor()
, m_data_requests(std::move(entity_config))
{
}

void DaikinRotexCanComponent::validateConfig() {
    m_data_requests.removeInvalidRequests(m_accessor);
}

void DaikinRotexCanComponent::setup() {
    ESP_LOGI(TAG, "setup");
}

void DaikinRotexCanComponent::set_operation_mode(std::string const& mode) {
    m_data_requests.sendSet(m_accessor, m_accessor.get_operating_mode_select()->get_name(), map_betriebsmodus.getKey(mode));
}

void DaikinRotexCanComponent::set_target_hot_water_temperature(float temperature) {
    m_data_requests.sendSet(m_accessor, m_accessor.get_target_hot_water_temperature_set()->get_name(), temperature);
}

void DaikinRotexCanComponent::set_heating_curve(float heating_curve) {
    m_data_requests.sendSet(m_accessor, m_accessor.get_heating_curve_set()->get_name(), heating_curve);
}

void DaikinRotexCanComponent::loop() {
    m_data_requests.sendNextPendingGet(m_accessor);
}

void DaikinRotexCanComponent::handle(uint32_t can_id, std::vector<uint8_t> const& data) {
    m_data_requests.handle(m_accessor, can_id, data);
}

void DaikinRotexCanComponent::dump_config() {
    ESP_LOGCONFIG(TAG, "DaikinRotexCanComponent");
}

} // namespace daikin_rotex_can
} // namespace esphome