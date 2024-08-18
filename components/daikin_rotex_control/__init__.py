import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, binary_sensor, text_sensor, canbus
from esphome.const import *
from esphome.components.canbus import CanbusComponent
dakin_rotex_control_ns = cg.esphome_ns.namespace('dakin_rotex_control')
DakinHPC = dakin_rotex_control_ns.class_('DakinRotexControl', cg.Component)

DEPENDENCIES = []
AUTO_LOAD = ['sensor', 'text_sensor', 'binary_sensor']

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(DakinHPC),
        cv.Required("canbus_id"): cv.use_id(CanbusComponent),
    }
).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
