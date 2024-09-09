import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, binary_sensor, button, number, select, text_sensor, canbus, text
from esphome.const import *
from esphome.components.canbus import CanbusComponent
#from esphome.const import (
#    ENTITY_CATEGORY_CONFIG,
#)

daikin_rotex_can_ns = cg.esphome_ns.namespace('daikin_rotex_can')
DaikinRotexCanComponent = daikin_rotex_can_ns.class_('DaikinRotexCanComponent', cg.Component)

GenericSelect = daikin_rotex_can_ns.class_("GenericSelect", select.Select)
GenericNumber = daikin_rotex_can_ns.class_("GenericNumber", number.Number)

LogFilterText = daikin_rotex_can_ns.class_("LogFilterText", text.Text)
CustomRequestText = daikin_rotex_can_ns.class_("CustomRequestText", text.Text)

DHWRunButton = daikin_rotex_can_ns.class_("DHWRunButton", button.Button)

UNIT_BAR = "bar"
UNIT_LITER_PER_HOUR = "L/h"

########## Icons ##########
ICON_SUN_SNOWFLAKE_VARIANT = "mdi:sun-snowflake-variant"

########## Configuration of Sensors, TextSensors, BinarySensors, Selects and Numbers ##########

sensor_configuration = [
    ####################### Sensors #######################
    {
        "type": "sensor",
        "name": "t_hs",
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "unit_of_measurement": UNIT_CELSIUS,
        "accuracy_decimals": 1,
        "state_class": STATE_CLASS_MEASUREMENT,
        "data": "31 00 FA 01 D6 00 00",
        "expected_reponse": "__ __ FA 01 D6 __ __",
        "data_offset": 5,
        "data_size": 2,
        "divider": 10.0
    },
    {
        "type": "sensor",
        "name": "temperature_outside",
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "unit_of_measurement": UNIT_CELSIUS,
        "accuracy_decimals": 1,
        "state_class": STATE_CLASS_MEASUREMENT,
        "data": "31 00 FA C0 FF 00 00",
        "expected_reponse": "__ __ FA C0 FF __ __",
        "data_offset": 5,
        "data_size": 2,
        "divider": 10.0
    },
    {
        "type": "sensor",
        "name": "t_ext",
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "unit_of_measurement": UNIT_CELSIUS,
        "accuracy_decimals": 1,
        "state_class": STATE_CLASS_MEASUREMENT,
        "data": "61 00 FA 0A 0C 00 00",
        "expected_reponse": "__ __ FA 0A 0C __ __",
        "data_offset": 5,
        "data_size": 2,
        "divider": 10.0
    },
    {
        "type": "sensor",
        "name": "tdhw1",
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "unit_of_measurement": UNIT_CELSIUS,
        "accuracy_decimals": 1,
        "state_class": STATE_CLASS_MEASUREMENT,
        "data": "31 00 FA 00 0E 00 00",
        "expected_reponse": "__ __ FA, 00, 0E __ __",
        "data_offset": 5,
        "data_size": 2,
        "divider": 10.0
    },
    {
        "type": "sensor",
        "name": "water_pressure",
        "device_class": DEVICE_CLASS_PRESSURE,
        "unit_of_measurement": UNIT_BAR,
        "accuracy_decimals": 2,
        "state_class": STATE_CLASS_MEASUREMENT,
        "data": "31 00 1C 00 00 00 00",
        "expected_reponse": "D2 __ 1C __ __ __ __",
        "data_offset": 3,
        "data_size": 2,
        "divider": 1000.0
    },
    {
        "type": "sensor",
        "name": "circulation_pump",
        "device_class": DEVICE_CLASS_VOLUME_FLOW_RATE,
        "unit_of_measurement": UNIT_PERCENT,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:pump",
        "data": "31 00 FA C0 F7 00 00",
        "expected_reponse": "__ __ FA C0 F7 __ __",
        "data_offset": 6,
        "data_size": 1,
        "divider": 1
    },
    {
        "type": "number",
        "name": "circulation_pump_min",
        "device_class": DEVICE_CLASS_VOLUME_FLOW_RATE,
        "unit_of_measurement": UNIT_PERCENT,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:waves-arrow-left",
        "min_value": 40,
        "max_value": 100,
        "step": 1,
        "data": "31 00 FA 06 7F 00 00",
        "expected_reponse": "__ __ FA 06 7F __ __",
        "data_offset": 6,
        "data_size": 1,
        "divider": 1,
        "set": "30 00 FA 06 7F __ __"
    },
    {
        "type": "number",
        "name": "circulation_pump_max",
        "device_class": DEVICE_CLASS_VOLUME_FLOW_RATE,
        "unit_of_measurement": UNIT_PERCENT,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:waves-arrow-right",
        "min_value": 60,
        "max_value": 100,
        "step": 1,
        "data": "31 00 FA 06 7E 00 00",
        "expected_reponse": "__ __ FA 06 7E __ __",
        "data_offset": 6,
        "data_size": 1,
        "divider": 1,
        "set": "30 00 FA 06 7E __ __"
    },
    {
        "type": "sensor",
        "name": "bypass_valve",
        "device_class": DEVICE_CLASS_VOLUME_FLOW_RATE,
        "unit_of_measurement": UNIT_PERCENT,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:waves-arrow-left",
        "data": "31 00 FA C0 FB 00 00",
        "expected_reponse": "__ __ FA C0 FB __ __",
        "data_offset": 5,
        "data_size": 2,
        "divider": 1
    },
    {
        "type": "sensor",
        "name": "dhw_mixer_position",
        "device_class": DEVICE_CLASS_VOLUME_FLOW_RATE,
        "unit_of_measurement": UNIT_PERCENT,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:waves-arrow-left",
        "data": "31 00 FA 06 9B 00 00",
        "expected_reponse": "__ __ FA 06 9B __ __",
        "data_offset": 5,
        "data_size": 2,
        "divider": 1
    },
    {
        "type": "sensor",
        "name": "target_supply_temperature",
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "unit_of_measurement": UNIT_CELSIUS,
        "accuracy_decimals": 1,
        "state_class": STATE_CLASS_MEASUREMENT,
        "data": "31 00 02 00 00 00 00",
        "expected_reponse": "D2 __ 02 __ __ __ __",
        "data_offset": 3,
        "data_size": 2,
        "divider": 10.0
    },
    {
        "type": "sensor",
        "name": "ehs_for_ch",
        "device_class": DEVICE_CLASS_ENERGY_STORAGE,
        "unit_of_measurement": UNIT_KILOWATT_HOURS,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:transmission-tower",
        "data": "31 00 FA 09 20 00 00",
        "expected_reponse": "__ __ FA 09 20 __ __",
        "data_offset": 5,
        "data_size": 2,
        "divider": 1
    },
    {
        "type": "sensor",
        "name": "qch",
        "device_class": DEVICE_CLASS_ENERGY_STORAGE,
        "unit_of_measurement": UNIT_KILOWATT_HOURS,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:transmission-tower",
        "data": "31 00 FA 06 A7 00 00",
        "expected_reponse": "__ __ FA 06 A7 __ __",
        "data_offset": 5,
        "data_size": 2,
        "divider": 1
    },
    {
        "type": "sensor",
        "name": "qboh",
        "device_class": DEVICE_CLASS_ENERGY_STORAGE,
        "unit_of_measurement": UNIT_KILOWATT_HOURS,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:transmission-tower",
        "data": "31 00 FA 09 1C 00 00",
        "expected_reponse": "__ __ FA 09 1C __ __",
        "data_offset": 5,
        "data_size": 2,
        "divider": 1
    },
    {
        "type": "sensor",
        "name": "qdhw",
        "device_class": DEVICE_CLASS_ENERGY_STORAGE,
        "unit_of_measurement": UNIT_KILOWATT_HOURS,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:transmission-tower",
        "data": "31 00 FA 09 2C 00 00",
        "expected_reponse": "__ __ FA 09 2C __ __",
        "data_offset": 5,
        "data_size": 2,
        "divider": 1
    },
    {
        "type": "sensor",
        "name": "total_energy_produced",
        "device_class": DEVICE_CLASS_ENERGY_STORAGE,
        "unit_of_measurement": UNIT_KILOWATT_HOURS,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:transmission-tower",
        "data": "31 00 FA 09 30 00 00",
        "expected_reponse": "__ __ FA 09 30 __ __",
        "data_offset": 5,
        "data_size": 2,
        "divider": 1
    },
    {
        "type": "sensor",
        "name": "runtime_compressor",
        "unit_of_measurement": UNIT_HOUR,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:clock-time-two-outline",
        "data": "31 00 FA 06 A5 00 00",
        "expected_reponse": "__ __ FA 06 A5 __ __",
        "data_offset": 5,
        "data_size": 2,
        "divider": 1
    },
    {
        "type": "sensor",
        "name": "runtime_pump",
        "unit_of_measurement": UNIT_HOUR,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:clock-time-two-outline",
        "data": "31 00 FA 06 A4 00 00",
        "expected_reponse": "__ __ FA 06 A4 __ __",
        "data_offset": 5,
        "data_size": 2,
        "divider": 1
    },
    {
        "type": "sensor",
        "name": "spreizung_mod_hz",
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "unit_of_measurement": UNIT_KELVIN,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:thermometer-lines",
        "data": "31 00 FA 06 83 00 00",
        "expected_reponse": "__ __ FA 06 83 __ __",
        "data_offset": 5,
        "data_size": 2,
        "divider": 10.0
    },
    {
        "type": "sensor",
        "name": "spreizung_mod_ww",
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "unit_of_measurement": UNIT_KELVIN,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:thermometer-lines",
        "data": "31 00 FA 06 84 00 00",
        "expected_reponse": "__ __ FA 06 84 __ __",
        "data_offset": 5,
        "data_size": 2,
        "divider": 10.0
    },
    {
        "type": "sensor",
        "name": "tv",
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "unit_of_measurement": UNIT_CELSIUS,
        "accuracy_decimals": 1,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:thermometer-lines",
        "data": "31 00 FA C0 FC 00 00",
        "expected_reponse": "__ __ FA C0 FC __ __",
        "data_offset": 5,
        "data_size": 2,
        "divider": 10.0,
        "update_entity": "thermal_power"
    },
    {
        "type": "sensor",
        "name": "tvbh",
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "unit_of_measurement": UNIT_CELSIUS,
        "accuracy_decimals": 1,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:thermometer-lines",
        "data": "31 00 FA C1 02 00 00",
        "expected_reponse": "__ __ FA C1 02 __ __",
        "data_offset": 5,
        "data_size": 2,
        "divider": 10.0,
        "update_entity": "thermal_power"
    },
    {
        "type": "sensor",
        "name": "tr",
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "unit_of_measurement": UNIT_CELSIUS,
        "accuracy_decimals": 1,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:thermometer-lines",
        "data": "31 00 FA C1 00 00 00",
        "expected_reponse": "__ __ FA C1 00 __ __",
        "data_offset": 5,
        "data_size": 2,
        "divider": 10.0,
        "update_entity": "thermal_power"
    },
    {
        "type": "sensor",
        "name": "water_flow",
        "device_class": DEVICE_CLASS_WATER,
        "unit_of_measurement": UNIT_LITER_PER_HOUR,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:thermometer-lines",
        "data": "31 00 FA 01 DA 00 00",
        "expected_reponse": "__ __ FA 01 DA __ __",
        "data_offset": 5,
        "data_size": 2,
        "divider": 1,
        "update_entity": "thermal_power"
    },
    {
        "type": "number",
        "name": "target_room1_temperature",
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "unit_of_measurement": UNIT_CELSIUS,
        "accuracy_decimals": 1,
        "state_class": STATE_CLASS_MEASUREMENT,
        "min_value": 15,
        "max_value": 25,
        "step": 0.1,
        "data": "31 00 05 00 00 00 00",
        "expected_reponse": "D2 00 05 __ __ 00 __",
        "data_offset": 3,
        "data_size": 2,
        "divider": 10.0,
        "set": "30 00 05 __ __ 00 00"
    },
    {
        "type": "number",
        "name": "flow_temperature_day",
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "unit_of_measurement": UNIT_CELSIUS,
        "accuracy_decimals": 1,
        "state_class": STATE_CLASS_MEASUREMENT,
        "min_value": 25,
        "max_value": 60,
        "step": 1,
        "data": "31 00 FA 01 29 00 00",
        "expected_reponse": "__ __ FA 01 29 __ __",
        "data_offset": 5,
        "data_size": 2,
        "divider": 10.0,
        "set": "30 00 FA 01 29 __ __"
    },
    {
        "type": "number",
        "name": "heating_curve",
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "unit_of_measurement": UNIT_CELSIUS,
        "accuracy_decimals": 2,
        "state_class": STATE_CLASS_MEASUREMENT,
        "min_value": 0,
        "max_value": 2.55,
        "step": 0.01,
        "data": "31 00 FA 01 0E 00 00",
        "expected_reponse": "__ __ FA 01 0E __ __",
        "data_offset": 5,
        "data_size": 2,
        "divider": 100.0,
        "set": "30 00 FA 01 0E __ __"
    },
    {
        "type": "number",
        "name": "min_target_flow_temp",
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "unit_of_measurement": UNIT_CELSIUS,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:waves-arrow-left",
        "min_value": 25,
        "max_value": 40,
        "step": 1,
        "data": "31 00 FA 01 2B 00 00",
        "expected_reponse": "__ __ FA 01 2B __ __",
        "data_offset": 5,
        "data_size": 2,
        "divider": 10.0,
        "set": "30 00 FA 01 2B __ __"
    },
    {
        "type": "number",
        "name": "max_target_flow_temp",
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "unit_of_measurement": UNIT_CELSIUS,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:waves-arrow-right",
        "min_value": 25,
        "max_value": 60,
        "step": 1,
        "data": "31 00 28 00 00 00 00",
        "expected_reponse": "__ __ 28 __ __ __ __",
        "data_offset": 3,
        "data_size": 2,
        "divider": 10.0,
        "set": "30 00 28 __ __ 00 00"
    },
    {
        "type": "number",
        "name": "target_hot_water_temperature",
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "unit_of_measurement": UNIT_CELSIUS,
        "accuracy_decimals": 1,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:waves-arrow-right",
        "min_value": 35,
        "max_value": 70,
        "step": 1,
        "data": "31 00 13 00 00 00 00",
        "expected_reponse": "D2 00 13 __ __ 00 00",
        "data_offset": 3,
        "data_size": 2,
        "divider": 10.0,
        "set": "30 00 13 __ __ 00 00"
    },
    ####################### Text sensors #######################
    {
        "type": "text_sensor",
        "name": "mode_of_operating" ,
        "icon": ICON_SUN_SNOWFLAKE_VARIANT,
        "data": "31 00 FA C0 F6 00 00",
        "expected_reponse": "__ __ FA C0 F6 __ __",
        "data_offset": 5,
        "data_size": 2,
        "map": {
            0x00: "Standby",
            0x01: "Heizen",
            0x02: "Kühlen",
            0x03: "Abtauen",
            0x04: "Warmwasserbereitung"
        },
        "update_entity": "thermal_power"
    },
    {
        "type": "text_sensor",
        "name": "quiet" ,
        "icon": "mdi:weather-partly-cloudy",
        "data": "31 00 FA 06 96",
        "expected_reponse": "__ __ FA 06 96 __ __",
        "data_offset": 6,
        "data_size": 1,
        "map": {
            0x00: "Aus",
            0x01: "An",
            0x02: "Nur bei Nacht"
        }
    },
    {
        "type": "text_sensor",
        "name": "error_code" ,
        "icon": "mdi:alert",
        "data": "31 00 FA 13 88 00 00",
        "expected_reponse": "__ __ FA 13 88 __ __",
        "data_offset": 5,
        "data_size": 2,
        "map": {
            0: "Kein Fehler",
            9001: "E9001 Rücklauffühler",
            9002: "E9002 Vorlauffühler",
            9003: "E9003 Frostschutzfunktion",
            9004: "E9004 Durchfluss",
            9005: "E9005 Vorlauftemperaturfühler",
            9006: "E9006 Vorlauftemperaturfühler",
            9007: "E9007 Platine IG defekt",
            9008: "E9008 Kältemitteltemperatur außerhalb des Bereiches",
            9009: "E9009 STB Fehler",
            9010: "E9010 STB Fehler",
            9011: "E9011 Fehler Flowsensor",
            9012: "E9012 Fehler Vorlauffühler",
            9013: "E9013 Platine AG defekt",
            9014: "E9014 P-Kältemittel hoch",
            9015: "E9015 P-Kältemittel niedrig",
            9016: "E9016 Lastschutz Verdichter",
            9017: "E9017 Ventilator blockiert",
            9018: "E9018 Expansionsventil",
            9019: "E9019 Warmwassertemperatur > 85°C",
            9020: "E9020 T-Verdampfer hoch",
            9021: "E9021 HPS-System",
            9022: "E9022 Fehler AT-Fühler",
            9023: "E9023 Fehler WW-Fühler",
            9024: "E9024 Drucksensor",
            9025: "E9025 Fehler Rücklauffühler",
            9026: "E9026 Drucksensor",
            9027: "E9027 Aircoil-Fühler Defrost",
            9028: "E9028 Aircoil-Fühler temp",
            9029: "E9029 Fehler Kältefühler AG",
            9030: "E9030 Defekt elektrisch",
            9031: "E9031 Defekt elektrisch",
            9032: "E9032 Defekt elektrisch",
            9033: "E9033 Defekt elektrisch",
            9034: "E9034 Defekt elektrisch",
            9035: "E9035 Platine AG defekt",
            9036: "E9036 Defekt elektrisch",
            9037: "E9037 Einstellung Leistung",
            9038: "E9038 Kältemittel Leck",
            9039: "E9039 Unter/Überspannung",
            9041: "E9041 Übertragungsfehler",
            9042: "E9042 Übertragungsfehler",
            9043: "E9043 Übertragungsfehler",
            9044: "E9044 Übertragungsfehler",
            75: "E75 Fehler Außentemperaturfühler",
            76: "E76 Fehler Speichertemperaturfühler",
            81: "E81 Kommunikationsfehler Rocon",
            88: "E88 Kommunikationsfehler Rocon Handbuch",
            91: "E91 Kommunikationsfehler Rocon Handbuch",
            128: "E128 Fehler Rücklauftemperaturfühler",
            129: "E129 Fehler Drucksensor",
            198: "E198 Durchflussmessung nicht plausibel",
            200: "E200 Kommunikationsfehler",
            8005: "E8005 Wasserdruck in Heizungsanlage zu gering",
            8100: "E8100 Kommunikation",
            9000: "E9000 Interne vorübergehende Meldung",
            8006: "W8006 Warnung Druckverlust",
            8007: "W8007 Wasserdruck in Anlage zu hoch"
        }
    },
    ####################### Binary sensors #######################
    {
        "type": "binary_sensor",
        "name": "status_kompressor" ,
        "icon": "mdi:pump",
        "data": "A1 00 61 00 00 00 00",
        "expected_reponse": "__ __ 61 __ __ __ __",
        "data_offset": 3,
        "data_size": 1
    },
    {
        "type": "binary_sensor",
        "name": "status_kesselpumpe" ,
        "icon": "mdi:pump",
        "data": "31 00 FA 0A 8C 00 00",
        "expected_reponse": "__ __ FA 0A 8C __ __",
        "data_offset": 6,
        "data_size": 1
    },
    ####################### Selects #######################
    {
        "type": "select",
        "name": "operating_mode" ,
        "icon": ICON_SUN_SNOWFLAKE_VARIANT,
        "data": "31 00 FA 01 12 00 00",
        "expected_reponse": "__ __ FA 01 12 __ __",
        "data_offset": 5,
        "data_size": 1,
        "map": {
            0x01: "Bereitschaft",
            0x03: "Heizen",
            0x04: "Absenken",
            0x05: "Sommer",
            0x11: "Kühlen",
            0x0B: "Automatik 1",
            0x0C: "Automatik 2"
        },
        "set": "30 00 FA 01 12 __ 00"
    },
    {
        "type": "select",
        "name": "hk_function" ,
        "icon": "mdi:weather-partly-cloudy",
        "data": "31 00 FA 01 41 00 00",
        "expected_reponse": "__ __ FA 01 41 __ __",
        "data_offset": 6,
        "data_size": 1,
        "map": {
            0x00: "Witterungsgeführt",
            0x01: "Fest"
        },
        "set": "30 00 FA 01 41 00 __"
    },
    {
        "type": "select",
        "name": "sg_mode" ,
        "icon": "mdi:weather-partly-cloudy",
        "data": "31 00 FA 06 94 00 00",
        "expected_reponse": "__ __ FA 06 94 __ __",
        "data_offset": 6,
        "data_size": 1,
        "map": {
            0x00: "Aus",
            0x01: "SG Modus 1",
            0x02: "SG Modus 2"
        },
        "set": "30 00 FA 06 94 00 __"
    },
    {
        "type": "select",
        "name": "smart_grid" ,
        "icon": "mdi:weather-partly-cloudy",
        "data": "31 00 FA 06 93 00 00",
        "expected_reponse": "__ __ FA 06 93 __ __",
        "data_offset": 6,
        "data_size": 1,
        "map": {
            0x00: "Aus",
            0x01: "An"
        },
        "set": "30 00 FA 06 93 00, __"
    }
]

DEPENDENCIES = []

AUTO_LOAD = ['binary_sensor', 'button', 'number', 'sensor', 'select', 'text', 'text_sensor']

CONF_CAN_ID = "canbus_id"
CONF_LOG_FILTER_TEXT = "log_filter"
CONF_CUSTOM_REQUEST_TEXT = "custom_request"
CONF_ENTITIES = "entities"

########## Sensors ##########

CONF_THERMAL_POWER = "thermal_power" # Thermische Leistung

CONF_DHW_RUN = "dhw_run"


entity_schemas = {}
for sensor_conf in sensor_configuration:
    match sensor_conf.get("type"):
        case "sensor":
            entity_schemas.update({
                cv.Optional(sensor_conf.get("name")): sensor.sensor_schema(
                    device_class=(sensor_conf.get("device_class") if sensor_conf.get("device_class") != None else sensor._UNDEF),
                    unit_of_measurement=sensor_conf.get("unit_of_measurement"),
                    accuracy_decimals=sensor_conf.get("accuracy_decimals"),
                    state_class=sensor_conf.get("state_class"),
                    icon=(sensor_conf.get("icon") if sensor_conf.get("icon") != None else sensor._UNDEF)
                ).extend()
            })
        case "text_sensor":
            entity_schemas.update({
                cv.Optional(sensor_conf.get("name")): text_sensor.text_sensor_schema(
                    icon=sensor_conf.get("icon")
                ).extend()
            })
        case "binary_sensor":
            entity_schemas.update({
                cv.Optional(sensor_conf.get("name")): binary_sensor.binary_sensor_schema(
                    icon=(sensor_conf.get("icon") if sensor_conf.get("icon") != None else sensor._UNDEF)
                ).extend()
            })
        case "select":
            entity_schemas.update({
                cv.Optional(sensor_conf.get("name")): select.select_schema(
                    GenericSelect,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=sensor_conf.get("icon")
                ).extend()
            })
        case "number":
            entity_schemas.update({
                cv.Optional(sensor_conf.get("name")): number.number_schema(
                    GenericNumber,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICON_SUN_SNOWFLAKE_VARIANT
                ).extend(),
            })


entity_schemas.update({
                ########## Sensors ##########

                cv.Optional(CONF_THERMAL_POWER): sensor.sensor_schema(
                    device_class=DEVICE_CLASS_POWER,
                    unit_of_measurement=UNIT_KILOWATT,
                    accuracy_decimals=2,
                    state_class=STATE_CLASS_MEASUREMENT
                ).extend(),

                ########## Buttons ##########

                cv.Optional(CONF_DHW_RUN): button.button_schema(
                    DHWRunButton,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICON_SUN_SNOWFLAKE_VARIANT
                ).extend(),
            })

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(DaikinRotexCanComponent),
        cv.Required(CONF_CAN_ID): cv.use_id(CanbusComponent),

        ########## Texts ##########

        cv.Optional(CONF_LOG_FILTER_TEXT): text.TEXT_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(LogFilterText),
                cv.Optional(CONF_MODE, default="TEXT"): cv.enum(text.TEXT_MODES, upper=True),
            }
        ),
        cv.Optional(CONF_CUSTOM_REQUEST_TEXT): text.TEXT_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(CustomRequestText),
                cv.Optional(CONF_MODE, default="TEXT"): cv.enum(text.TEXT_MODES, upper=True),
            }
        ),

        cv.Required(CONF_ENTITIES): cv.Schema(
            entity_schemas
        ),
    }
).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)

    if CONF_CAN_ID in config:
        cg.add_define("USE_CANBUS")
        canbus = yield cg.get_variable(config[CONF_CAN_ID])
        cg.add(var.set_canbus(canbus))

    ########## Texts ##########

    if text_conf := config.get(CONF_LOG_FILTER_TEXT):
        t = yield text.new_text(text_conf)
        cg.add(var.getAccessor().set_log_filter(t))

    if text_conf := config.get(CONF_CUSTOM_REQUEST_TEXT):
        t = yield text.new_text(text_conf)
        yield cg.register_parented(t, var)
        cg.add(var.getAccessor().set_custom_request_text(t))

    if entities := config.get(CONF_ENTITIES):
        for sens_conf in sensor_configuration:
            if yaml_sensor_conf := entities.get(sens_conf.get("name")):
                match sens_conf.get("type"):
                    case "sensor":
                        sens = yield sensor.new_sensor(yaml_sensor_conf)

                        cg.add(var.getAccessor().set_sensor(
                            sens_conf.get("name"),
                            [
                                sens,
                                sens_conf.get("name"),
                                sens_conf.get("data"),
                                sens_conf.get("expected_reponse"),
                                sens_conf.get("data_offset"),
                                sens_conf.get("data_size"),
                                sens_conf.get("divider"),
                                sens_conf.get("update_entity", ""),
                                sens_conf.get("set_entity", "")
                            ]
                        ))
                    case "text_sensor":
                        sens = yield text_sensor.new_text_sensor(yaml_sensor_conf)

                        cg.add(var.getAccessor().set_text_sensor(
                            sens_conf.get("name"),
                            [
                                sens,
                                sens_conf.get("name"),
                                sens_conf.get("data"),
                                sens_conf.get("expected_reponse"),
                                sens_conf.get("data_offset"),
                                sens_conf.get("data_size"),
                                "|".join([f"0x{key:02X}:{value}" for key, value in sens_conf.get("map").items()]),
                                sens_conf.get("update_entity", ""),
                                sens_conf.get("set_entity", "")
                            ]
                        ))
                    case "binary_sensor":
                        sens = yield binary_sensor.new_binary_sensor(yaml_sensor_conf)

                        cg.add(var.getAccessor().set_binary_sensor(
                            sens_conf.get("name"),
                            [
                                sens,
                                sens_conf.get("name"),
                                sens_conf.get("data"),
                                sens_conf.get("expected_reponse"),
                                sens_conf.get("data_offset"),
                                sens_conf.get("data_size"),
                                sens_conf.get("update_entity", ""),
                                sens_conf.get("set_entity", "")
                            ]
                        ))
                    case "select":
                        sel = yield select.new_select(yaml_sensor_conf, options = list(sens_conf.get("map").values()))
                        cg.add(sel.set_id(sens_conf.get("name")))
                        yield cg.register_parented(sel, var)

                        cg.add(var.getAccessor().set_select(
                            sens_conf.get("name"),
                            [
                                sel,
                                sens_conf.get("name"),
                                sens_conf.get("data"),
                                sens_conf.get("expected_reponse"),
                                sens_conf.get("data_offset"),
                                sens_conf.get("data_size"),
                                "|".join([f"0x{key:02X}:{value}" for key, value in sens_conf.get("map").items()]),
                                sens_conf.get("set")
                            ]
                        ))
                    case "number":
                        num = yield number.new_number(
                            yaml_sensor_conf,
                            min_value=sens_conf.get("min_value"),
                            max_value=sens_conf.get("max_value"),
                            step=sens_conf.get("step"),
                        )
                        yield cg.register_parented(num, var)
                        cg.add(var.getAccessor().set_number(
                            sens_conf.get("name"),
                            [
                                num,
                                sens_conf.get("name"),
                                sens_conf.get("data"),
                                sens_conf.get("expected_reponse"),
                                sens_conf.get("data_offset"),
                                sens_conf.get("data_size"),
                                sens_conf.get("divider"),
                                sens_conf.get("set")
                            ]
                        ))

        ########## Sensors ##########

        if yaml_sensor_conf := entities.get(CONF_THERMAL_POWER):
            sens = yield sensor.new_sensor(yaml_sensor_conf)
            cg.add(var.getAccessor().set_thermal_power(sens))

        ########## Buttons ##########

        if button_conf := entities.get(CONF_DHW_RUN):
            but = yield button.new_button(button_conf)
            yield cg.register_parented(but, var)

    cg.add(var.validateConfig())
