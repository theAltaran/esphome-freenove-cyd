import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

CODEOWNERS = ["@altaran"]
DEPENDENCIES = ["spi"]

CONF_FREENOVE_CYD_DISPLAY = "freenove_cyd_display"

freenove_cyd_ns = cg.esphome_ns.namespace("freenove_cyd")
FreenoveCYDDisplay = freenove_cyd_ns.class_(
    "FreenoveCYDDisplay", cg.Component
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(FreenoveCYDDisplay),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    cg.add_global(cg.RawExpression('#include "freenove_cyd_display.h"'))
    
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
