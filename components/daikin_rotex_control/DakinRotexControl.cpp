#include "esphome/components/daikin_rotex_control/DakinRotexControl.h"
#include "esphome/components/daikin_rotex_control/request.h"
#include "esphome/components/daikin_rotex_control/BidiMap.h"
#include <string>
#include <vector>

namespace esphome {
namespace dakin_rotex_control {

static const char *TAG = "dakin_rotex_control";

static const BidiMap<uint8_t, std::string> map_betriebsmodus {
    {0x01, "Bereitschaft"},
    {0x03, "Heizen"},
    {0x04, "Absenken"},
    {0x05, "Sommer"},
    {0x11, "Kühlen"},
    {0x0B, "Automatik 1"},
    {0x0C, "Automatik 2"}
};

const std::vector<TRequest> entity_config = {
    {
        "Aussentemperatur",
        {0x31, 0x00, 0xFA, 0xC0, 0xFF, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0xC0, 0xFF,   DC,   DC},
        [](auto const& data, auto& accessor) -> DataType {
            const float temp = float(((int((data[6]) + ((data[5]) << 8))) ^ 0x8000) - 0x8000) / 10;
            accessor.get_temperature_outside_sensor()->publish_state(temp);
            return temp;
        }
    },
    {
        "tdhw1",
        {0x31, 0x00, 0xFA, 0x00, 0x0E, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x00, 0x0E,   DC,   DC},
        [](auto const& data, auto& accessor) -> DataType {
            float temp = float((float((int((data[6]) + ((data[5]) << 8))))/10));
            accessor.get_tdhw1()->publish_state(temp);
            return temp;
        }
    },
    {
        "water pressure",
        {0x31, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00},
        {0xD2,   DC, 0x1C,   DC,   DC,   DC,   DC},
        [](auto const& data, auto& accessor) -> DataType {
            float pressure = float((float((int((data[4]) + ((data[3]) << 8))))/1000));
            accessor.get_water_pressure()->publish_state(pressure);
            return pressure;
        }
    },
    {
        "Durchfluss",
        {0x31, 0x00, 0xFA, 0x01, 0xDA, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x01, 0xDA,   DC,   DC},
        [](auto const& data, auto& accessor) -> DataType {
            float flow = float((float((int((data[6]) + ((data[5]) << 8))))));
            accessor.get_water_flow()->publish_state(flow);
            return flow;
        }
    },

    {
        "Betriebsmodus",
        {0x31, 0x00, 0xFA, 0x01, 0x12, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x01, 0x12,   DC,   DC},
        [](auto const& data, auto& accessor) -> DataType {
            const std::string mode = map_betriebsmodus.getValue(data[5]);
            accessor.get_operation_mode_sensor()->publish_state(mode);
            accessor.get_operation_mode_select()->publish_state(mode);
            return mode;
        }
    },
    {
        "Betriebsmodus setzen",
        [](auto const& value) -> std::vector<uint8_t> {
            return {0x30, 0x00, 0xFA, 0x01, 0x12, static_cast<uint8_t>(value), 0x00};
        }
    },

    {
        "Fehlercode",
        {0x31, 0x00, 0xFA, 0x13, 0x88, 0x00, 0x00},
        {  DC,   DC, 0xFA, 0x13, 0x88,   DC,   DC},
        [](auto const& data, auto& accessor) -> DataType {
            const uint32_t code = uint32_t(data[6]) + (uint32_t(data[5]) << 8);

            auto to_str = [](uint32_t code) -> std::string {
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

            accessor.get_error_code_sensor()->publish_state(str_code);
            return str_code;
        }
    },
};

DakinRotexControl::DakinRotexControl()
: m_data_requests(std::move(entity_config))
{
}

void DakinRotexControl::setup() {
    //ESP_LOGI(TAG, "setup");
}

void DakinRotexControl::set_operation_mode(std::string const& mode) {
    m_data_requests.sendSet("Betriebsmodus setzen", map_betriebsmodus.getKey(mode));
}

void DakinRotexControl::loop() {
    m_data_requests.sendNextPendingGet();
}

void DakinRotexControl::handle(uint32_t can_id, std::vector<uint8_t> const& data) {
    m_data_requests.handle(can_id, data, m_accessor);
}

void DakinRotexControl::dump_config() {
    ESP_LOGCONFIG(TAG, "DakinRotexControl");
}

} // namespace dakin_rotex_control
} // namespace esphome