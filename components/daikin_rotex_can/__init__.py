import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, binary_sensor, number, select, text_sensor, canbus, text
from esphome.const import *
from esphome.components.canbus import CanbusComponent
#from esphome.const import (
#    ENTITY_CATEGORY_CONFIG,
#)

daikin_rotex_can_ns = cg.esphome_ns.namespace('daikin_rotex_can')
DaikinRotexCanComponent = daikin_rotex_can_ns.class_('DaikinRotexCanComponent', cg.Component)
OperationModeSelect = daikin_rotex_can_ns.class_("OperationModeSelect", select.Select)
TargetHotWaterTemperatureNumber = daikin_rotex_can_ns.class_("TargetHotWaterTemperatureNumber", number.Number)
LogFilterText = daikin_rotex_can_ns.class_("LogFilterText", text.Text)

DEPENDENCIES = []

UNIT_BAR = "bar"
UNIT_LITER_PER_HOUR = "L/h"

AUTO_LOAD = ['binary_sensor', 'number', 'sensor', 'select', 'text', 'text_sensor']

CONF_CAN_ID = "canbus_id"
CONF_LOG_FILTER_TEXT = "log_filter"

########## Sensors ##########

CONF_TEMPERATURE_OUTSIDE = "temperature_outside"    # External temperature
CONF_TDHW1 = "tdhw1"
CONF_TARGET_HOT_WATER_TEMPERATURE = "target_hot_water_temperature"
CONF_TV = "tv"
CONF_TVBH = "tvbh"
CONF_TR = "tr"
CONF_WATER_PRESSURE = "water_pressure"
CONF_WATER_FLOW = "water_flow"
CONF_CIRCULATION_PUMP = "circulation_pump"
CONF_BYPASS_VALVE = "bypass_valve"
CONF_DHW_MIXER_POSITION = "dhw_mixer_position"
CONF_TARGET_SUPPLY_TEMPERATURE = "target_supply_temperature" # Vorlauf Soll

########## Text Sensors ##########

CONF_OPERATING_MODE = "operating_mode"
CONF_MODE_OF_OPERATING = "mode_of_operation"
CONF_ERROR_CODE = "error_code"

CONF_STATUS_KOMPRESSOR = "status_kompressor"
CONF_STATUS_KESSELPUMPE = "status_kesselpumpe"

CONF_OPERATING_MODE_SELECT = "operating_mode_select"

CONF_TARGET_HOT_WATER_TEMPERATURE_SET = "target_hot_water_temperature_set"

ICON_SUN_SNOWFLAKE_VARIANT = "mdi:sun-snowflake-variant"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(DaikinRotexCanComponent),
        cv.Required(CONF_CAN_ID): cv.use_id(CanbusComponent),

        ########## Texts ##########

        cv.Optional(CONF_LOG_FILTER_TEXT): 
        text.TEXT_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(LogFilterText),
                cv.Optional(CONF_MODE, default="TEXT"): cv.enum(text.TEXT_MODES, upper=True),
            }
        ),

        ########## Sensors ##########

        cv.Optional(CONF_TEMPERATURE_OUTSIDE): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT
        ).extend(),
        cv.Optional(CONF_TDHW1): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT
        ).extend(),
        cv.Optional(CONF_TARGET_HOT_WATER_TEMPERATURE): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT
        ).extend(),
        cv.Optional(CONF_TV): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT
        ).extend(),
        cv.Optional(CONF_TVBH): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT
        ).extend(),
        cv.Optional(CONF_TR): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT
        ).extend(),
        cv.Optional(CONF_WATER_PRESSURE): sensor.sensor_schema(
            device_class=DEVICE_CLASS_PRESSURE,
            unit_of_measurement=UNIT_BAR,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT
        ).extend(),
        cv.Optional(CONF_WATER_FLOW): sensor.sensor_schema(
            device_class=DEVICE_CLASS_WATER,
            unit_of_measurement=UNIT_LITER_PER_HOUR,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT
        ).extend(),
        cv.Optional(CONF_CIRCULATION_PUMP): sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLUME_FLOW_RATE,
            unit_of_measurement=UNIT_PERCENT,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
            icon="mdi:pump"
        ).extend(),
        cv.Optional(CONF_BYPASS_VALVE): sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLUME_FLOW_RATE,
            unit_of_measurement=UNIT_PERCENT,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
            icon="mdi:waves-arrow-left"
        ).extend(),
        cv.Optional(CONF_DHW_MIXER_POSITION): sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLUME_FLOW_RATE,
            unit_of_measurement=UNIT_PERCENT,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
            icon="mdi:waves-arrow-left"
        ).extend(),
        cv.Optional(CONF_TARGET_SUPPLY_TEMPERATURE): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT
        ).extend(),

        ######## Text Sensors ########

        cv.Optional(CONF_OPERATING_MODE): text_sensor.text_sensor_schema(
            icon=ICON_SUN_SNOWFLAKE_VARIANT
        ).extend(),
        cv.Optional(CONF_MODE_OF_OPERATING): text_sensor.text_sensor_schema(
            icon=ICON_SUN_SNOWFLAKE_VARIANT
        ).extend(),
        cv.Optional(CONF_ERROR_CODE): text_sensor.text_sensor_schema(
            icon="mdi:alert"
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

        ########## Number ##########

        cv.Optional(CONF_TARGET_HOT_WATER_TEMPERATURE_SET): number.number_schema(
            TargetHotWaterTemperatureNumber,
            entity_category=ENTITY_CATEGORY_CONFIG,
            icon=ICON_SUN_SNOWFLAKE_VARIANT
        ).extend(),
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

    if log_filter := config.get(CONF_LOG_FILTER_TEXT):
        te = yield text.new_text(log_filter)
        cg.add(var.getAccessor().set_log_filter(te))

    ########## Sensors ##########

    if temperature_outside := config.get(CONF_TEMPERATURE_OUTSIDE):
        sens = yield sensor.new_sensor(temperature_outside)
        cg.add(var.getAccessor().set_temperature_outside(sens))

    if tdhw1 := config.get(CONF_TDHW1):
        sens = yield sensor.new_sensor(tdhw1)
        cg.add(var.getAccessor().set_tdhw1(sens))

    if target_hot_water_temperature := config.get(CONF_TARGET_HOT_WATER_TEMPERATURE):
        sens = yield sensor.new_sensor(target_hot_water_temperature)
        cg.add(var.getAccessor().set_target_hot_water_temperature(sens))

    if tv := config.get(CONF_TV):
        sens = yield sensor.new_sensor(tv)
        cg.add(var.getAccessor().set_tv(sens))

    if tvbh := config.get(CONF_TVBH):
        sens = yield sensor.new_sensor(tvbh)
        cg.add(var.getAccessor().set_tvbh(sens))

    if tr := config.get(CONF_TR):
        sens = yield sensor.new_sensor(tr)
        cg.add(var.getAccessor().set_tr(sens))

    if water_pressure := config.get(CONF_WATER_PRESSURE):
        sens = yield sensor.new_sensor(water_pressure)
        cg.add(var.getAccessor().set_water_pressure(sens))

    if water_flow := config.get(CONF_WATER_FLOW):
        sens = yield sensor.new_sensor(water_flow)
        cg.add(var.getAccessor().set_water_flow(sens))

    if circulation_pump := config.get(CONF_CIRCULATION_PUMP):
        sens = yield sensor.new_sensor(circulation_pump)
        cg.add(var.getAccessor().set_circulation_pump(sens))

    if bypass_valve := config.get(CONF_BYPASS_VALVE):
        sens = yield sensor.new_sensor(bypass_valve)
        cg.add(var.getAccessor().set_bypass_valve(sens))

    if dhw_mixer_position := config.get(CONF_DHW_MIXER_POSITION):
        sens = yield sensor.new_sensor(dhw_mixer_position)
        cg.add(var.getAccessor().set_dhw_mixer_position(sens))

    if target_supply_temperature := config.get(CONF_TARGET_SUPPLY_TEMPERATURE):
        sens = yield sensor.new_sensor(target_supply_temperature)
        cg.add(var.getAccessor().set_target_supply_temperature(sens))

    ######## Text Sensors ########

    if operating_mode := config.get(CONF_OPERATING_MODE):
        sens = yield text_sensor.new_text_sensor(operating_mode)
        cg.add(var.getAccessor().set_operating_mode(sens))

    if mode_of_operating := config.get(CONF_MODE_OF_OPERATING):
        sens = yield text_sensor.new_text_sensor(mode_of_operating)
        cg.add(var.getAccessor().set_mode_of_operating(sens))

    if error_code := config.get(CONF_ERROR_CODE):
        sens = yield text_sensor.new_text_sensor(error_code)
        cg.add(var.getAccessor().set_error_code(sens))

    ######## Binary Sensors ########

    if status_kompressor := config.get(CONF_STATUS_KOMPRESSOR):
        sens = yield binary_sensor.new_binary_sensor(status_kompressor)
        cg.add(var.getAccessor().set_status_kompressor(sens))

    if status_kesselpumpe := config.get(CONF_STATUS_KESSELPUMPE):
        sens = yield binary_sensor.new_binary_sensor(status_kesselpumpe)
        cg.add(var.getAccessor().set_status_kesselpumpe(sens))

    ########## Selects ##########

    operating_mode_options = ["Bereitschaft", "Heizen", "Absenken", "Sommer", "Kühlen", "Automatik 1", "Automatik 2"]

    if operating_mode_select := config.get(CONF_OPERATING_MODE_SELECT):
        s = yield select.new_select(operating_mode_select, options = operating_mode_options)
        yield cg.register_parented(s, var)
        cg.add(var.getAccessor().set_operating_mode_select(s))

    ########## Selects ##########
    if target_hot_water_temperature_set := config.get(CONF_TARGET_HOT_WATER_TEMPERATURE_SET):
        num = yield number.new_number(
            target_hot_water_temperature_set,
            min_value=35,
            max_value=70,
            step=1
        )
        yield cg.register_parented(num, var)
        cg.add(var.getAccessor().set_target_hot_water_temperature_set(num))

    cg.add(var.validateConfig())
