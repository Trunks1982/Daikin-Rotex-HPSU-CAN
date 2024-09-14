import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, binary_sensor, button, number, select, text_sensor, canbus, text
from esphome.const import *
from esphome.core import Lambda
from esphome.cpp_generator import MockObj
from esphome.cpp_types import std_ns
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
DumpButton = daikin_rotex_can_ns.class_("DumpButton", button.Button)

UNIT_BAR = "bar"
UNIT_LITER_PER_HOUR = "L/h"

########## Icons ##########
ICON_SUN_SNOWFLAKE_VARIANT = "mdi:sun-snowflake-variant"

########## Configuration of Sensors, TextSensors, BinarySensors, Selects and Numbers ##########

sensor_configuration = [
   {
        "type": "select",
        "name": "outdoor_unit" ,
        "icon": ICON_SUN_SNOWFLAKE_VARIANT,
        "command": "31 00 FA 06 9A 00 00",
        "data_offset": 6,
        "data_size": 1,
        "map": {
            0x00: "--",
            0x01: "4",
            0x02: "6",
            0x03: "8",
            0x04: "11",
            0x05: "14",
            0x06: "16"
        }
    },
    {
        "type": "select",
        "name": "indoor_unit" ,
        "icon": ICON_SUN_SNOWFLAKE_VARIANT,
        "command": "31 00 FA 06 99 00 00",
        "data_offset": 6,
        "data_size": 1,
        "map": {
            0x00: "--",
            0x01: "304",
            0x02: "308",
            0x03: "508",
            0x04: "516"
        }
    },
    {
        "type": "number",
        "name": "antileg_temp",
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "unit_of_measurement": UNIT_CELSIUS,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "min_value": 60,
        "max_value": 75,
        "step": 1,
        "command": "31 00 FA 05 87 00 00",
        "data_offset": 5,
        "data_size": 2,
        "divider": 10.0
    },
    {
        "type": "select",
        "name": "antileg_day" ,
        "icon": ICON_SUN_SNOWFLAKE_VARIANT,
        "command": "31 00 FA 01 01 00 00",
        "data_offset": 5,
        "data_size": 1,
        "map": {
            0x00: "Aus",
            0x01: "Mo",
            0x02: "Di",
            0x03: "Mi",
            0x04: "Do",
            0x05: "Fr",
            0x06: "Sa",
            0x07: "So",
            0x08: "Mo-So"
        }
    },
    {
        "type": "number",
        "name": "circulation_interval_on",
        "unit_of_measurement": UNIT_MINUTE,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "min_value": 0,
        "max_value": 15,
        "step": 1,
        "command": "31 00 FA 06 5E 00 00",
        "data_offset": 5,
        "data_size": 2,
        "divider": 1
    },
    {
        "type": "number",
        "name": "circulation_interval_off",
        "unit_of_measurement": UNIT_MINUTE,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "min_value": 0,
        "max_value": 15,
        "step": 1,
        "command": "31 00 FA 06 5F 00 00",
        "data_offset": 5,
        "data_size": 2,
        "divider": 1
    },
    {
        "type": "select",
        "name": "circulation_with_dhw_program" ,
        "icon": ICON_SUN_SNOWFLAKE_VARIANT,
        "command": "31 00 FA 01 82 00 00",
        "data_offset": 6,
        "data_size": 1,
        "map": {
            0x00: "Aus",
            0x01: "An"
        }
    },
    {
        "type": "number",
        "name": "t_dhw_1_min",
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "unit_of_measurement": UNIT_CELSIUS,
        "accuracy_decimals": 1,
        "state_class": STATE_CLASS_MEASUREMENT,
        "min_value": 20,
        "max_value": 85,
        "step": 1,
        "command": "31 00 FA 06 73 00 00",
        "data_offset": 5,
        "data_size": 2,
        "divider": 10.0
    },
    {
        "type": "number",
        "name": "max_dhw_loading",
        "unit_of_measurement": UNIT_MINUTE,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "min_value": 10,
        "max_value": 240,
        "step": 1,
        "command": "31 00 FA 01 80 00 00",
        "data_offset": 5,
        "data_size": 2,
        "divider": 1
    },
    {
        "type": "number",
        "name": "dhw_off_time",
        "unit_of_measurement": UNIT_MINUTE,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "min_value": 0,
        "max_value": 180,
        "step": 1,
        "command": "31 00 FA 4E 3F 00 00",
        "data_offset": 5,
        "data_size": 2,
        "divider": 1
    },
    {
        "type": "number",
        "name": "tdiff_dhw_ch",
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "unit_of_measurement": UNIT_KELVIN,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "min_value": 2,
        "max_value": 15,
        "step": 1,
        "command": "31 00 FA 06 6D 00 00",
        "data_offset": 5,
        "data_size": 2,
        "divider": 10.0
    },
    {
        "type": "sensor",
        "name": "t_hs",
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "unit_of_measurement": UNIT_CELSIUS,
        "accuracy_decimals": 1,
        "state_class": STATE_CLASS_MEASUREMENT,
        "command": "31 00 FA 01 D6 00 00",
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
        "command": "31 00 FA C0 FF 00 00",
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
        "command": "61 00 FA 0A 0C 00 00",
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
        "command": "31 00 FA 00 0E 00 00",
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
        "command": "31 00 1C 00 00 00 00",
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
        "command": "31 00 FA C0 F7 00 00",
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
        "command": "31 00 FA 06 7F 00 00",
        "data_offset": 6,
        "data_size": 1,
        "divider": 1
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
        "command": "31 00 FA 06 7E 00 00",
        "data_offset": 6,
        "data_size": 1,
        "divider": 1
    },
    {
        "type": "sensor",
        "name": "bypass_valve",
        "device_class": DEVICE_CLASS_VOLUME_FLOW_RATE,
        "unit_of_measurement": UNIT_PERCENT,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:waves-arrow-left",
        "command": "31 00 FA C0 FB 00 00",
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
        "command": "31 00 FA 06 9B 00 00",
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
        "command": "31 00 02 00 00 00 00",
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
        "command": "31 00 FA 09 20 00 00",
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
        "command": "31 00 FA 06 A7 00 00",
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
        "command": "31 00 FA 09 1C 00 00",
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
        "command": "31 00 FA 09 2C 00 00",
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
        "command": "31 00 FA 09 30 00 00",
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
        "command": "31 00 FA 06 A5 00 00",
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
        "command": "31 00 FA 06 A4 00 00",
        "data_offset": 5,
        "data_size": 2,
        "divider": 1
    },
    {
        "type": "sensor",
        "name": "delta_temp_ch",
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "unit_of_measurement": UNIT_KELVIN,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:thermometer-lines",
        "command": "31 00 FA 06 83 00 00",
        "data_offset": 5,
        "data_size": 2,
        "divider": 10.0
    },
    {
        "type": "sensor",
        "name": "delta_temp_dhw",
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "unit_of_measurement": UNIT_KELVIN,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:thermometer-lines",
        "command": "31 00 FA 06 84 00 00",
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
        "command": "31 00 FA C0 FC 00 00",
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
        "command": "31 00 FA C1 02 00 00",
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
        "command": "31 00 FA C1 00 00 00",
        "data_offset": 5,
        "data_size": 2,
        "divider": 10.0,
        "update_entity": "thermal_power"
    },
    {
        "type": "sensor",
        "name": "flow_rate",
        "device_class": DEVICE_CLASS_WATER,
        "unit_of_measurement": UNIT_LITER_PER_HOUR,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:thermometer-lines",
        "command": "31 00 FA 01 DA 00 00",
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
        "command": "31 00 05 00 00 00 00",
        "data_offset": 3,
        "data_size": 2,
        "divider": 10.0
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
        "command": "31 00 FA 01 29 00 00",
        "data_offset": 5,
        "data_size": 2,
        "divider": 10.0
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
        "command": "31 00 FA 01 0E 00 00",
        "data_offset": 5,
        "data_size": 2,
        "divider": 100.0
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
        "command": "31 00 FA 01 2B 00 00",
        "data_offset": 5,
        "data_size": 2,
        "divider": 10.0
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
        "command": "31 00 28 00 00 00 00",
        "data_offset": 3,
        "data_size": 2,
        "divider": 10.0
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
        "command": "31 00 13 00 00 00 00",
        "data_offset": 3,
        "data_size": 2,
        "divider": 10.0
    },
    {
        "type": "text_sensor",
        "name": "mode_of_operating" ,
        "icon": ICON_SUN_SNOWFLAKE_VARIANT,
        "command": "31 00 FA C0 F6 00 00",
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
        "command": "31 00 FA 06 96",
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
        "command": "31 00 FA 13 88 00 00",
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
    {
        "type": "binary_sensor",
        "name": "status_kompressor" ,
        "icon": "mdi:pump",
        "command": "A1 00 61 00 00 00 00",
        "data_offset": 3,
        "data_size": 1
    },
    {
        "type": "binary_sensor",
        "name": "status_kesselpumpe" ,
        "icon": "mdi:pump",
        "command": "31 00 FA 0A 8C 00 00",
        "data_offset": 6,
        "data_size": 1
    },
    {
        "type": "select",
        "name": "operating_mode" ,
        "icon": ICON_SUN_SNOWFLAKE_VARIANT,
        "command": "31 00 FA 01 12 00 00",
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
        }
    },
    {
        "type": "select",
        "name": "hk_function" ,
        "icon": "mdi:weather-partly-cloudy",
        "command": "31 00 FA 01 41 00 00",
        "data_offset": 6,
        "data_size": 1,
        "map": {
            0x00: "Witterungsgeführt",
            0x01: "Fest"
        }
    },
    {
        "type": "select",
        "name": "sg_mode" ,
        "icon": "mdi:weather-partly-cloudy",
        "command": "31 00 FA 06 94 00 00",
        "data_offset": 6,
        "data_size": 1,
        "map": {
            0x00: "Aus",
            0x01: "SG Modus 1",
            0x02: "SG Modus 2"
        }
    },
    {
        "type": "select",
        "name": "smart_grid" ,
        "icon": "mdi:weather-partly-cloudy",
        "command": "31 00 FA 06 93 00 00",
        "data_offset": 6,
        "data_size": 1,
        "map": {
            0x00: "Aus",
            0x01: "An"
        }
    },
    {
        "type": "select",
        "name": "function_ehs" ,
        "icon": ICON_SUN_SNOWFLAKE_VARIANT,
        "command": "31 00 FA 06 D2 00 00",
        "data_offset": 6,
        "data_size": 1,
        "map": {
            0x00: "Kein zusätzlicher Wärmeerzeuger",
            0x01: "Optionaler Backup-Heater",
            0x02: "WEZ für WW und HZ",
            0x03: "WEZ1 für WW - WEZ2 für HZ"
        }
    },
    {
        "type": "select",
        "name": "ch_support" ,
        "icon": ICON_SUN_SNOWFLAKE_VARIANT,
        "command": "31 00 FA 06 6C 00 00",
        "data_offset": 6,
        "data_size": 1,
        "map": {
            0x00: "Aus",
            0x01: "An"
        }
    },
    {
        "type": "number",
        "name": "power_dhw",
        "device_class": DEVICE_CLASS_POWER,
        "unit_of_measurement": UNIT_KILOWATT,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:waves-arrow-left",
        "min_value": 1,
        "max_value": 40,
        "step": 1,
        "command": "31 00 FA 06 68 00 00",
        "handle_lambda": """
            return ((data[5] << 8) | data[6]) / 0x64;
        """,
        "set_lambda": """
            const uint16_t u16val = value * 0x64;
            data[5] = (u16val >> 8) & 0xFF;
            data[6] = u16val & 0xFF;
        """
    },
    {
        "type": "number",
        "name": "power_ehs_1",
        "device_class": DEVICE_CLASS_POWER,
        "unit_of_measurement": UNIT_KILOWATT,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:waves-arrow-left",
        "min_value": 1,
        "max_value": 40,
        "step": 1,
        "command": "31 00 FA 06 69 00 00",
       "handle_lambda": """
            return ((data[5] << 8) | data[6]) / 0x64;
        """,
        "set_lambda": """
            const uint16_t u16val = value * 0x64;
            data[5] = (u16val >> 8) & 0xFF;
            data[6] = u16val & 0xFF;
        """
    },
    {
        "type": "number",
        "name": "power_ehs_2",
        "device_class": DEVICE_CLASS_POWER,
        "unit_of_measurement": UNIT_KILOWATT,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:waves-arrow-left",
        "min_value": 1,
        "max_value": 40,
        "step": 1,
        "command": "31 00 FA 06 6A 00 00",
       "handle_lambda": """
            return ((data[5] << 8) | data[6]) / 0x64;
        """,
        "set_lambda": """
            const uint16_t u16val = value * 0x64;
            data[5] = (u16val >> 8) & 0xFF;
            data[6] = u16val & 0xFF;
        """
    },
    {
        "type": "number",
        "name": "power_biv",
        "device_class": DEVICE_CLASS_POWER,
        "unit_of_measurement": UNIT_KILOWATT,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:waves-arrow-left",
        "min_value": 3,
        "max_value": 40,
        "step": 1,
        "command": "31 00 FA 06 6B 00 00",
       "handle_lambda": """
            return ((data[5] << 8) | data[6]) / 0x64;
        """,
        "set_lambda": """
            const uint16_t u16val = value * 0x64;
            data[5] = (u16val >> 8) & 0xFF;
            data[6] = u16val & 0xFF;
        """
    },
    {
        "type": "select",
        "name": "electric_heater",
        "device_class": DEVICE_CLASS_POWER,
        "unit_of_measurement": UNIT_KILOWATT,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:induction",
        "command": "31 00 FA 0A 20 00 00",
        "data_offset": 5,
        "data_size": 2,
        "map": {
            0x00: "Aus",
            0x03: "3 kW",
            0x06: "6 kW",
            0x09: "9 kW"
        },
        "handle_lambda": """
            return
                bool(data[5] & 0b00001000) * 3 +
                bool(data[5] & 0b00000100) * 3 +
                bool(data[5] & 0b00000010) * 3;
        """,
        "set_lambda": """
            data[5] = 0b00000001;
            if (value >= 3) data[5] |= 0b00001000;
            if (value >= 6) data[5] |= 0b00000100;
            if (value >= 9) data[5] |= 0b00000010;
        """
    },
    {
        "type": "text_sensor",
        "name": "ext",
        "accuracy_decimals": 0,
        "icon": "mdi:transmission-tower-import",
        "command": "31 00 FA C0 F8 00 00",
        "data_offset": 6,
        "data_size": 1,
        "map": {
            0x03: "SGN - Normaler Modus",
            0x04: "SG1 - WW & HZ ausgeschalten",
            0x05: "SG2 - WW & HZ + 5°C",
            0x06: "SG3 - WW 70°C"
        }
    }
]

DEPENDENCIES = []

AUTO_LOAD = ['binary_sensor', 'button', 'number', 'sensor', 'select', 'text', 'text_sensor']

CONF_CAN_ID = "canbus_id"
CONF_UPDATE_INTERVAL = "update_interval"
CONF_LOG_FILTER_TEXT = "log_filter"
CONF_CUSTOM_REQUEST_TEXT = "custom_request"
CONF_ENTITIES = "entities"

########## Sensors ##########

CONF_THERMAL_POWER = "thermal_power" # Thermische Leistung

CONF_DUMP = "dump"
CONF_DHW_RUN = "dhw_run"

DEFAULT_UPDATE_INTERVAL = 30 # seconds

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
                ).extend({cv.Optional(CONF_UPDATE_INTERVAL): cv.uint16_t}),
            })
        case "text_sensor":
            entity_schemas.update({
                cv.Optional(sensor_conf.get("name")): text_sensor.text_sensor_schema(
                    icon=sensor_conf.get("icon")
                ).extend({cv.Optional(CONF_UPDATE_INTERVAL): cv.uint16_t}),
            })
        case "binary_sensor":
            entity_schemas.update({
                cv.Optional(sensor_conf.get("name")): binary_sensor.binary_sensor_schema(
                    icon=(sensor_conf.get("icon") if sensor_conf.get("icon") != None else sensor._UNDEF)
                ).extend({cv.Optional(CONF_UPDATE_INTERVAL): cv.uint16_t}),
            })
        case "select":
            entity_schemas.update({
                cv.Optional(sensor_conf.get("name")): select.select_schema(
                    GenericSelect,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=sensor_conf.get("icon")
                ).extend({cv.Optional(CONF_UPDATE_INTERVAL): cv.uint16_t}),
            })
        case "number":
            entity_schemas.update({
                cv.Optional(sensor_conf.get("name")): number.number_schema(
                    GenericNumber,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICON_SUN_SNOWFLAKE_VARIANT
                ).extend({
                    cv.Optional(CONF_UPDATE_INTERVAL): cv.uint16_t,
                    cv.Optional(CONF_MODE, default="BOX"): cv.enum(number.NUMBER_MODES, upper=True)
                })
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
        cv.Optional(CONF_UPDATE_INTERVAL, default=DEFAULT_UPDATE_INTERVAL): cv.uint16_t,

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
        cv.Optional(CONF_DUMP): button.button_schema(
            DumpButton,
            entity_category=ENTITY_CATEGORY_CONFIG,
            icon=ICON_SUN_SNOWFLAKE_VARIANT
        ).extend(),

        cv.Required(CONF_ENTITIES): cv.Schema(
            entity_schemas
        ),
    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    global_ns = MockObj("", "")
    std_array_u8_7_const_ref = std_ns.class_("array<uint8_t, 7> const&")
    std_array_u8_7_ref = std_ns.class_("array<uint8_t, 7>&")

    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    if CONF_CAN_ID in config:
        cg.add_define("USE_CANBUS")
        canbus = await cg.get_variable(config[CONF_CAN_ID])
        cg.add(var.set_canbus(canbus))

    ########## Texts ##########

    if text_conf := config.get(CONF_LOG_FILTER_TEXT):
        t = await text.new_text(text_conf)
        cg.add(var.getAccessor().set_log_filter(t))

    if text_conf := config.get(CONF_CUSTOM_REQUEST_TEXT):
        t = await text.new_text(text_conf)
        await cg.register_parented(t, var)
        cg.add(var.getAccessor().set_custom_request_text(t))

    if button_conf := config.get(CONF_DUMP):
        but = await button.new_button(button_conf)
        await cg.register_parented(but, var)

    if entities := config.get(CONF_ENTITIES):
        for sens_conf in sensor_configuration:
            if yaml_sensor_conf := entities.get(sens_conf.get("name")):
                entity = None
                match sens_conf.get("type"):
                    case "sensor":
                        entity = await sensor.new_sensor(yaml_sensor_conf)
                    case "text_sensor":
                        entity = await text_sensor.new_text_sensor(yaml_sensor_conf)
                    case "binary_sensor":
                        entity = await binary_sensor.new_binary_sensor(yaml_sensor_conf)
                    case "select":
                        entity = await select.new_select(yaml_sensor_conf, options = list(sens_conf.get("map").values()))
                        cg.add(entity.set_id(sens_conf.get("name")))
                        await cg.register_parented(entity, var)
                    case "number":
                        if "min_value" not in sens_conf:
                            raise Exception("min_value is required for number: " + sens_conf.get("name"))
                        if "max_value" not in sens_conf:
                            raise Exception("max_value is required for number: " + sens_conf.get("name"))
                        if "step" not in sens_conf:
                            raise Exception("step is required for number: " + sens_conf.get("name"))
                        entity = await number.new_number(
                            yaml_sensor_conf,
                            min_value=sens_conf.get("min_value"),
                            max_value=sens_conf.get("max_value"),
                            step=sens_conf.get("step")
                        )
                        cg.add(entity.set_id(sens_conf.get("name")))

                        await cg.register_parented(entity, var)
                    case _:
                        raise Exception("Unknown type: " + sens_conf.get("type"))

                update_interval = yaml_sensor_conf.get(CONF_UPDATE_INTERVAL, -1)
                if update_interval < 0:
                    update_interval = config[CONF_UPDATE_INTERVAL]

                if "command" not in sens_conf:
                    raise Exception("command is required for number: " + sens_conf.get("name"))

                async def handle_lambda():
                    lamb = str(sens_conf.get("handle_lambda")) if "handle_lambda" in sens_conf else "return 0;"
                    return await cg.process_lambda(
                        Lambda(lamb),
                        [(std_array_u8_7_const_ref, "data")],
                        return_type=cg.float_,
                    )

                async def set_lambda():
                    lamb = str(sens_conf.get("set_lambda")) if "set_lambda" in sens_conf else ""
                    return await cg.process_lambda(
                        Lambda(lamb),
                        [(std_array_u8_7_ref, "data"), (float, "value")],
                        return_type=cg.void,
                    )

                cg.add(var.getAccessor().set_entity(sens_conf.get("name"), [
                    entity,
                    sens_conf.get("name"),
                    sens_conf.get("command"),
                    sens_conf.get("data_offset", 5),
                    sens_conf.get("data_size", 1),
                    sens_conf.get("divider", 1.0),
                    "|".join([f"0x{key:02X}:{value}" for key, value in sens_conf.get("map", {}).items()]), # map
                    sens_conf.get("update_entity", ""),
                    update_interval,
                    await handle_lambda(),
                    await set_lambda(),
                    "handle_lambda" in sens_conf,
                    "set_lambda" in sens_conf
                ]))

        ########## Sensors ##########

        if yaml_sensor_conf := entities.get(CONF_THERMAL_POWER):
            sens = await sensor.new_sensor(yaml_sensor_conf)
            cg.add(var.getAccessor().set_thermal_power(sens))

        ########## Buttons ##########

        if button_conf := entities.get(CONF_DHW_RUN):
            but = await button.new_button(button_conf)
            await cg.register_parented(but, var)
