import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, binary_sensor, text_sensor, canbus, text
from esphome.const import *
from esphome.components.canbus import CanbusComponent

timer_ns = cg.esphome_ns.namespace("timer")
dakin_rotex_control_ns = cg.esphome_ns.namespace('dakin_rotex_control')
DakinHPC = dakin_rotex_control_ns.class_('DakinRotexControl', cg.Component)
TimerText = timer_ns.class_("TimerText", text.Text, cg.Component)

DEPENDENCIES = []
AUTO_LOAD = ['sensor', 'text_sensor', 'binary_sensor']

CONF_CAN_ID = "canbus_id"
CONF_LOG_FILTER_TEXT = "log_filter_text"
CONF_T_SENSORS = ["temperature_outside", "operation_mode"]

CONF_TEMPERATURE_OUTSIDE = "temperature_outside"
CONF_OPERATION_MODE = "operation_mode"

ICON_SUN_SNOWFLAKE_VARIANT = "mdi:sun-snowflake-variant"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(DakinHPC),
        cv.Required(CONF_CAN_ID): cv.use_id(CanbusComponent),
        cv.Optional(CONF_LOG_FILTER_TEXT): text.TEXT_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(TimerText),
                cv.Optional(CONF_MODE, default="TEXT"): cv.enum(text.TEXT_MODES, upper=True),
            }
        ),
        cv.Optional(CONF_TEMPERATURE_OUTSIDE): sensor.sensor_schema(
            device_class=DEVICE_CLASS_TEMPERATURE,
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT
        ).extend(),
        cv.Optional(CONF_OPERATION_MODE): text_sensor.text_sensor_schema(
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

    if temperature_outside := config.get(CONF_TEMPERATURE_OUTSIDE):
        sens = yield sensor.new_sensor(temperature_outside)
        cg.add(var.set_temperature_outside_sensor(sens))

    if operation_mode := config.get(CONF_OPERATION_MODE):
        sens = yield text_sensor.new_text_sensor(operation_mode)
        cg.add(var.set_operation_mode_sensor(sens))
