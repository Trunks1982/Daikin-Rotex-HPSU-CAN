import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, binary_sensor, text_sensor, canbus
from esphome.const import *
from esphome.components.canbus import CanbusComponent
dakin_rotex_control_ns = cg.esphome_ns.namespace('dakin_rotex_control')
DakinHPC = dakin_rotex_control_ns.class_('DakinRotexControl', cg.Component)

DEPENDENCIES = []
AUTO_LOAD = ['sensor', 'text_sensor', 'binary_sensor']

CONF_CAN_ID = "canbus_id"
CONF_T_SENSORS = ["temperature_outside"]

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(DakinHPC),
        cv.Required(CONF_CAN_ID): cv.use_id(CanbusComponent),
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
    #cg.add_build_flag("-std=c++17")
    #cg.add_build_flag("-fexceptions")

    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)

#    cg.add(var.set_can_id(can_bus))

#    output_ = await cg.get_variable(config[CONF_OUTPUT])
#    cg.add(var.set_output(output_))

    if CONF_CAN_ID in config:
        cg.add_define("USE_CANBUS")
        canbus = yield cg.get_variable(config[CONF_CAN_ID])
        cg.add(var.set_canbus(canbus))

    for sens in CONF_T_SENSORS:
        if sens in config:
            conf = config[sens]
            sens = yield sensor.new_sensor(conf)
            cg.add(var.set_temperature_outside_sensor(sens))