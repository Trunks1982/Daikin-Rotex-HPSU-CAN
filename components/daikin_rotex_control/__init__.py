import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, binary_sensor, text_sensor, canbus
from esphome.const import *
from esphome.components.canbus import CanbusComponent
dakin_rotex_control_ns = cg.esphome_ns.namespace('dakin_rotex_control')
DakinHPC = dakin_rotex_control_ns.class_('DakinRotexControl', cg.Component)

DEPENDENCIES = []
AUTO_LOAD = ['sensor', 'text_sensor', 'binary_sensor']

CONF_T_SENSORS = ["temperature_outside"]

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(DakinHPC),
        cv.Required("canbus_id"): cv.use_id(CanbusComponent),
        cv.Optional(CONF_T_SENSORS[0]):
            sensor.sensor_schema(
                device_class=DEVICE_CLASS_TEMPERATURE,
                unit_of_measurement=UNIT_CELSIUS,
                accuracy_decimals=1,
                state_class=STATE_CLASS_MEASUREMENT
            ).extend(),
    }
).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)

    for sens in CONF_T_SENSORS:
        if sens in config:
            conf = config[sens]
            sens = yield sensor.new_sensor(conf)
            cg.add(var.set_temperature_outside_sensor(sens))