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

OperationModeSelect = daikin_rotex_can_ns.class_("OperationModeSelect", select.Select)
HKFunctionSelect = daikin_rotex_can_ns.class_("HKFunctionSelect", select.Select)
SGModeSelect = daikin_rotex_can_ns.class_("SGModeSelect", select.Select)
SmartGridSelect = daikin_rotex_can_ns.class_("SmartGridSelect", select.Select)

TargetHotWaterTemperatureNumber = daikin_rotex_can_ns.class_("TargetHotWaterTemperatureNumber", number.Number)
TargetRoom1TemperatureNumber =  daikin_rotex_can_ns.class_("TargetRoom1TemperatureNumber", number.Number)
FlowTemperatureDayNumber = daikin_rotex_can_ns.class_("FlowTemperatureDayNumber", number.Number)
MaxTargetFlowTempNumber = daikin_rotex_can_ns.class_("MaxTargetFlowTempNumber", number.Number)
MinTargetFlowTempNumber = daikin_rotex_can_ns.class_("MinTargetFlowTempNumber", number.Number)
HeatingCurveNumber = daikin_rotex_can_ns.class_("HeatingCurveNumber", number.Number)
CirculationPumpMinNumber = daikin_rotex_can_ns.class_("CirculationPumpMinNumber", number.Number)
CirculationPumpMaxNumber = daikin_rotex_can_ns.class_("CirculationPumpMaxNumber", number.Number)

LogFilterText = daikin_rotex_can_ns.class_("LogFilterText", text.Text)
CustomRequestText = daikin_rotex_can_ns.class_("CustomRequestText", text.Text)

DHWRunButton = daikin_rotex_can_ns.class_("DHWRunButton", button.Button)

UNIT_BAR = "bar"
UNIT_LITER_PER_HOUR = "L/h"

sensor_configuration = [
    {
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
        "name": "circulation_pump_min",
        "device_class": DEVICE_CLASS_VOLUME_FLOW_RATE,
        "unit_of_measurement": UNIT_PERCENT,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:waves-arrow-left",
        "data": "31 00 FA 06 7F 00 00",
        "expected_reponse": "__ __ FA 06 7F __ __",
        "data_offset": 6,
        "data_size": 1,
        "divider": 1,
        "set_entity": "circulation_pump_min_set"
    },
    {
        "name": "circulation_pump_max",
        "device_class": DEVICE_CLASS_VOLUME_FLOW_RATE,
        "unit_of_measurement": UNIT_PERCENT,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:waves-arrow-right",
        "data": "31 00 FA 06 7E 00 00",
        "expected_reponse": "__ __ FA 06 7E __ __",
        "data_offset": 6,
        "data_size": 1,
        "divider": 1,
        "set_entity": "circulation_pump_max_set"
    },
    {
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
        "name": "ehs_for_ch",
        "device_class": DEVICE_CLASS_ENERGY_STORAGE,
        "unit_of_measurement": UNIT_KILOWATT_HOURS,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:thermometer-lines",
        "data": "31 00 FA 09 20 00 00",
        "expected_reponse": "__ __ FA 09 20 __ __",
        "data_offset": 5,
        "data_size": 2,
        "divider": 1
    },
    {
        "name": "total_energy_produced",
        "device_class": DEVICE_CLASS_ENERGY_STORAGE,
        "unit_of_measurement": UNIT_KILOWATT_HOURS,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:thermometer-lines",
        "data": "31 00 FA 09 30 00 00",
        "expected_reponse": "__ __ FA 09 30 __ __",
        "data_offset": 5,
        "data_size": 2,
        "divider": 1
    },
    {
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
        "name": "target_room1_temperature",
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "unit_of_measurement": UNIT_CELSIUS,
        "accuracy_decimals": 1,
        "state_class": STATE_CLASS_MEASUREMENT,
        "icon": "mdi:thermometer-lines",
        "data": "31 00 05 00 00 00 00",
        "expected_reponse": "D2 00 05 __ __ 00 __",
        "data_offset": 3,
        "data_size": 2,
        "divider": 10.0,
        "set_entity": "target_room1_temperature_set"
    }
]

DEPENDENCIES = []

AUTO_LOAD = ['binary_sensor', 'button', 'number', 'sensor', 'select', 'text', 'text_sensor']

CONF_CAN_ID = "canbus_id"
CONF_LOG_FILTER_TEXT = "log_filter"
CONF_CUSTOM_REQUEST_TEXT = "custom_request"
CONF_ENTITIES = "entities"

########## Sensors ##########

CONF_TARGET_HOT_WATER_TEMPERATURE = "target_hot_water_temperature"
CONF_FLOW_TEMPERATURE_DAY = "flow_temperature_day" # Temperatur Vorlauf Tag
CONF_THERMAL_POWER = "thermal_power" # Thermische Leistung
CONF_HEATING_CURVE = "heating_curve" # Heizkurve
CONF_MIN_TARGET_SUPPLY_TEMPERATURE = "min_target_supply_temperature" # Min Vorlauf Soll
CONF_MAX_TARGET_SUPPLY_TEMPERATURE = "max_target_supply_temperature" # Max Vorlauf Soll

########## Text Sensors ##########

CONF_OPERATING_MODE = "operating_mode"
CONF_MODE_OF_OPERATING = "mode_of_operation"
CONF_HK_FUNCTION = "hk_function"
CONF_SG_MODE = "sg_mode"
CONF_SMART_GRID = "smart_grid"
CONF_ERROR_CODE = "error_code"

########## Binary Sensors ##########

CONF_STATUS_KOMPRESSOR = "status_kompressor"
CONF_STATUS_KESSELPUMPE = "status_kesselpumpe"

########## Selects ##########

CONF_OPERATING_MODE_SELECT = "operating_mode_select"
CONF_HK_FUNCTION_SELECT = "hk_function_select"
CONF_SG_MODE_SELECT = "sg_mode_select"
CONF_SMART_GRID_SELECT = "smart_grid_select"

########## Numbers ##########

CONF_TARGET_HOT_WATER_TEMPERATURE_SET = "target_hot_water_temperature_set"
CONF_TARGET_ROOM1_TEMPERATURE_SET = "target_room1_temperature_set"
CONF_FLOW_TEMPERATURE_DAY_SET = "flow_temperature_day_set"
CONF_MAX_TARGET_FLOW_TEMP_SET = "max_target_flow_temp_set"
CONF_MIN_TARGET_FLOW_TEMP_SET = "min_target_flow_temp_set"
CONF_HEATING_CURVE_SET = "heating_curve_set" # Heizkurve setzen
CONF_CIRCULATION_PUMP_MIN_SET = "circulation_pump_min_set"
CONF_CIRCULATION_PUMP_MAX_SET = "circulation_pump_max_set"

########## Numbers ##########

CONF_DHW_RUN = "dhw_run"

########## Icons ##########

ICON_SUN_SNOWFLAKE_VARIANT = "mdi:sun-snowflake-variant"

schemas = {}

for sensor_conf in sensor_configuration:
    sens = {
        cv.Optional(sensor_conf.get("name")): sensor.sensor_schema(
            device_class=(sensor_conf.get("device_class") if sensor_conf.get("device_class") != None else sensor._UNDEF),
            unit_of_measurement=sensor_conf.get("unit_of_measurement"),
            accuracy_decimals=sensor_conf.get("accuracy_decimals"),
            state_class=sensor_conf.get("state_class"),
            icon=(sensor_conf.get("icon") if sensor_conf.get("icon") != None else sensor._UNDEF)
        ).extend()
    }
    schemas.update(sens)

entity_schemas = {
                ########## Sensors ##########

                cv.Optional(CONF_TARGET_HOT_WATER_TEMPERATURE): sensor.sensor_schema(
                    device_class=DEVICE_CLASS_TEMPERATURE,
                    unit_of_measurement=UNIT_CELSIUS,
                    accuracy_decimals=1,
                    state_class=STATE_CLASS_MEASUREMENT
                ).extend(),
                cv.Optional(CONF_FLOW_TEMPERATURE_DAY): sensor.sensor_schema(
                    device_class=DEVICE_CLASS_TEMPERATURE,
                    unit_of_measurement=UNIT_CELSIUS,
                    accuracy_decimals=1,
                    state_class=STATE_CLASS_MEASUREMENT
                ).extend(),
                cv.Optional(CONF_THERMAL_POWER): sensor.sensor_schema(
                    device_class=DEVICE_CLASS_POWER,
                    unit_of_measurement=UNIT_KILOWATT,
                    accuracy_decimals=2,
                    state_class=STATE_CLASS_MEASUREMENT
                ).extend(),
                cv.Optional(CONF_HEATING_CURVE): sensor.sensor_schema(
                    device_class=DEVICE_CLASS_TEMPERATURE,
                    icon="mdi:thermometer-lines",
                    accuracy_decimals=2,
                    state_class=STATE_CLASS_MEASUREMENT
                ).extend(),
                cv.Optional(CONF_MIN_TARGET_SUPPLY_TEMPERATURE): sensor.sensor_schema(
                    device_class=DEVICE_CLASS_TEMPERATURE,
                    unit_of_measurement=UNIT_CELSIUS,
                    accuracy_decimals=0,
                    icon="mdi:waves-arrow-left",
                ).extend(),
                cv.Optional(CONF_MAX_TARGET_SUPPLY_TEMPERATURE): sensor.sensor_schema(
                    device_class=DEVICE_CLASS_TEMPERATURE,
                    unit_of_measurement=UNIT_CELSIUS,
                    accuracy_decimals=0,
                    icon="mdi:waves-arrow-right",
                ).extend(),

                ######## Text Sensors ########

                cv.Optional(CONF_OPERATING_MODE): text_sensor.text_sensor_schema(
                    icon=ICON_SUN_SNOWFLAKE_VARIANT
                ).extend(),
                cv.Optional(CONF_MODE_OF_OPERATING): text_sensor.text_sensor_schema(
                    icon=ICON_SUN_SNOWFLAKE_VARIANT
                ).extend(),
                cv.Optional(CONF_HK_FUNCTION): text_sensor.text_sensor_schema(
                    icon="mdi:weather-partly-cloudy"
                ).extend(),
                cv.Optional(CONF_ERROR_CODE): text_sensor.text_sensor_schema(
                    icon="mdi:alert"
                ).extend(),
                cv.Optional(CONF_SG_MODE): text_sensor.text_sensor_schema(
                    icon="mdi:thermometer-lines",
                ).extend(),
                cv.Optional(CONF_SMART_GRID): text_sensor.text_sensor_schema(
                    icon="mdi:thermometer-lines",
                ).extend(),

                ########## Binary Sensors ##########

                cv.Optional(CONF_STATUS_KOMPRESSOR): binary_sensor.binary_sensor_schema(
                    icon="mdi:pump"
                ).extend(),
                cv.Optional(CONF_STATUS_KESSELPUMPE): binary_sensor.binary_sensor_schema(
                    icon="mdi:pump"
                ).extend(),

                ########## Selects ##########

                cv.Optional(CONF_OPERATING_MODE_SELECT): select.select_schema(
                    OperationModeSelect,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICON_SUN_SNOWFLAKE_VARIANT
                ).extend(),
                cv.Optional(CONF_HK_FUNCTION_SELECT): select.select_schema(
                    HKFunctionSelect,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon="mdi:weather-partly-snowy"
                ).extend(),
                cv.Optional(CONF_SG_MODE_SELECT): select.select_schema(
                    SGModeSelect,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon="mdi:weather-partly-snowy"
                ).extend(),
                cv.Optional(CONF_SMART_GRID_SELECT): select.select_schema(
                    SmartGridSelect,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon="mdi:weather-partly-snowy"
                ).extend(),

                ########## Number ##########

                cv.Optional(CONF_TARGET_HOT_WATER_TEMPERATURE_SET): number.number_schema(
                    TargetHotWaterTemperatureNumber,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICON_SUN_SNOWFLAKE_VARIANT
                ).extend(),
                cv.Optional(CONF_TARGET_ROOM1_TEMPERATURE_SET): number.number_schema(
                    TargetRoom1TemperatureNumber,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICON_SUN_SNOWFLAKE_VARIANT
                ).extend(),
                cv.Optional(CONF_FLOW_TEMPERATURE_DAY_SET): number.number_schema(
                    FlowTemperatureDayNumber,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICON_SUN_SNOWFLAKE_VARIANT
                ).extend(),
                cv.Optional(CONF_MAX_TARGET_FLOW_TEMP_SET): number.number_schema(
                    MaxTargetFlowTempNumber,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICON_SUN_SNOWFLAKE_VARIANT
                ).extend(),
                cv.Optional(CONF_MIN_TARGET_FLOW_TEMP_SET): number.number_schema(
                    MinTargetFlowTempNumber,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICON_SUN_SNOWFLAKE_VARIANT
                ).extend(),
                cv.Optional(CONF_HEATING_CURVE_SET): number.number_schema(
                    HeatingCurveNumber,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICON_SUN_SNOWFLAKE_VARIANT
                ).extend(),
                cv.Optional(CONF_CIRCULATION_PUMP_MIN_SET): number.number_schema(
                    CirculationPumpMinNumber,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICON_SUN_SNOWFLAKE_VARIANT
                ).extend(),
                cv.Optional(CONF_CIRCULATION_PUMP_MAX_SET): number.number_schema(
                    CirculationPumpMaxNumber,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICON_SUN_SNOWFLAKE_VARIANT
                ).extend(),

                ########## Buttons ##########

                cv.Optional(CONF_DHW_RUN): button.button_schema(
                    DHWRunButton,
                    entity_category=ENTITY_CATEGORY_CONFIG,
                    icon=ICON_SUN_SNOWFLAKE_VARIANT
                ).extend(),
            }

entity_schemas.update(schemas)


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
        ########## Sensors ##########

        for sens_conf in sensor_configuration:
            if sensor_conf := entities.get(sens_conf.get("name")):
                sens = yield sensor.new_sensor(sensor_conf)

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

        if sensor_conf := entities.get(CONF_TARGET_HOT_WATER_TEMPERATURE):
            sens = yield sensor.new_sensor(sensor_conf)
            cg.add(var.getAccessor().set_target_hot_water_temperature(sens))

        if sensor_conf := entities.get(CONF_FLOW_TEMPERATURE_DAY):
            sens = yield sensor.new_sensor(sensor_conf)
            cg.add(var.getAccessor().set_flow_temperature_day(sens))

        if sensor_conf := entities.get(CONF_THERMAL_POWER):
            sens = yield sensor.new_sensor(sensor_conf)
            cg.add(var.getAccessor().set_thermal_power(sens))

        if sensor_conf := entities.get(CONF_HEATING_CURVE):
            sens = yield sensor.new_sensor(sensor_conf)
            cg.add(var.getAccessor().set_heating_curve(sens))

        if sensor_conf := entities.get(CONF_MIN_TARGET_SUPPLY_TEMPERATURE):
            sens = yield sensor.new_sensor(sensor_conf)
            cg.add(var.getAccessor().set_min_target_supply_temperature(sens))

        if sensor_conf := entities.get(CONF_MAX_TARGET_SUPPLY_TEMPERATURE):
            sens = yield sensor.new_sensor(sensor_conf)
            cg.add(var.getAccessor().set_max_target_supply_temperature(sens))

        ######## Text Sensors ########

        if sensor_conf := entities.get(CONF_OPERATING_MODE):
            sens = yield text_sensor.new_text_sensor(sensor_conf)
            cg.add(var.getAccessor().set_operating_mode(sens))

        if sensor_conf := entities.get(CONF_MODE_OF_OPERATING):
            sens = yield text_sensor.new_text_sensor(sensor_conf)
            cg.add(var.getAccessor().set_mode_of_operating(sens))

        if sensor_conf := entities.get(CONF_HK_FUNCTION):
            sens = yield text_sensor.new_text_sensor(sensor_conf)
            cg.add(var.getAccessor().set_hk_function(sens))

        if sensor_conf := entities.get(CONF_ERROR_CODE):
            sens = yield text_sensor.new_text_sensor(sensor_conf)
            cg.add(var.getAccessor().set_error_code(sens))

        if sensor_conf := entities.get(CONF_SG_MODE):
            sens = yield text_sensor.new_text_sensor(sensor_conf)
            cg.add(var.getAccessor().set_sg_mode(sens))

        if sensor_conf := entities.get(CONF_SMART_GRID):
            sens = yield text_sensor.new_text_sensor(sensor_conf)
            cg.add(var.getAccessor().set_smart_grid(sens))

        ######## Binary Sensors ########

        if sensor_conf := entities.get(CONF_STATUS_KOMPRESSOR):
            sens = yield binary_sensor.new_binary_sensor(sensor_conf)
            cg.add(var.getAccessor().set_status_kompressor(sens))

        if sensor_conf := entities.get(CONF_STATUS_KESSELPUMPE):
            sens = yield binary_sensor.new_binary_sensor(sensor_conf)
            cg.add(var.getAccessor().set_status_kesselpumpe(sens))

        ########## Selects ##########

        if select_conf := entities.get(CONF_OPERATING_MODE_SELECT):
            options = ["Bereitschaft", "Heizen", "Absenken", "Sommer", "Kühlen", "Automatik 1", "Automatik 2"]
            sel = yield select.new_select(select_conf, options = options)
            yield cg.register_parented(sel, var)
            cg.add(var.getAccessor().set_operating_mode_select(sel))

        if select_conf := entities.get(CONF_HK_FUNCTION_SELECT):
            options = ["Witterungsgeführt", "Fest"]
            sel = yield select.new_select(select_conf, options = options)
            yield cg.register_parented(sel, var)
            cg.add(var.getAccessor().set_hk_function_select(sel))

        if select_conf := entities.get(CONF_SG_MODE_SELECT):
            options = ["Aus", "SG Modus 1", "SG Modus 2"]
            sel = yield select.new_select(select_conf, options = options)
            yield cg.register_parented(sel, var)
            cg.add(var.getAccessor().set_sg_mode_select(sel))

        if select_conf := entities.get(CONF_SMART_GRID_SELECT):
            options = ["Aus", "An"]
            sel = yield select.new_select(select_conf, options = options)
            yield cg.register_parented(sel, var)
            cg.add(var.getAccessor().set_smart_grid_select(sel))

        ########## Numbers ##########

        if number_conf := entities.get(CONF_TARGET_HOT_WATER_TEMPERATURE_SET):
            num = yield number.new_number(
                number_conf,
                min_value=35,
                max_value=70,
                step=1
            )
            yield cg.register_parented(num, var)
            cg.add(var.getAccessor().set_target_hot_water_temperature_set(num))

        if number_conf := entities.get(CONF_TARGET_ROOM1_TEMPERATURE_SET):
            num = yield number.new_number(
                number_conf,
                min_value=15,
                max_value=25,
                step=0.1
            )
            yield cg.register_parented(num, var)
            cg.add(var.getAccessor().set_target_room1_temperature_set(num))

        if number_conf := entities.get(CONF_FLOW_TEMPERATURE_DAY_SET):
            num = yield number.new_number(
                number_conf,
                min_value=25,
                max_value=60,
                step=1
            )
            yield cg.register_parented(num, var)
            cg.add(var.getAccessor().set_flow_temperature_day_set(num))

        if number_conf := entities.get(CONF_MAX_TARGET_FLOW_TEMP_SET):
            num = yield number.new_number(
                number_conf,
                min_value=25,
                max_value=60,
                step=1
            )
            yield cg.register_parented(num, var)
            cg.add(var.getAccessor().set_max_target_flow_temp_set(num))

        if number_conf := entities.get(CONF_MIN_TARGET_FLOW_TEMP_SET):
            num = yield number.new_number(
                number_conf,
                min_value=25,
                max_value=40,
                step=1
            )
            yield cg.register_parented(num, var)
            cg.add(var.getAccessor().set_min_target_flow_temp_set(num))

        if number_conf := entities.get(CONF_HEATING_CURVE_SET):
            num = yield number.new_number(
                number_conf,
                min_value=0,
                max_value=2.55,
                step=0.01
            )
            yield cg.register_parented(num, var)
            cg.add(var.getAccessor().set_heating_curve_set(num))

        if number_conf := entities.get(CONF_CIRCULATION_PUMP_MIN_SET):
            num = yield number.new_number(
                number_conf,
                min_value=40,
                max_value=100,
                step=1
            )
            yield cg.register_parented(num, var)
            cg.add(var.getAccessor().set_circulation_pump_min_set(num))

        if number_conf := entities.get(CONF_CIRCULATION_PUMP_MAX_SET):
            num = yield number.new_number(
                number_conf,
                min_value=60,
                max_value=100,
                step=1
            )
            yield cg.register_parented(num, var)
            cg.add(var.getAccessor().set_circulation_pump_max_set(num))

        ########## Numbers ##########

        if button_conf := entities.get(CONF_DHW_RUN):
            but = yield button.new_button(button_conf)
            yield cg.register_parented(but, var)

    cg.add(var.validateConfig())
