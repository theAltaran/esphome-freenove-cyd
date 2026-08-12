import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.core import CORE
from esphome.components import uart
from esphome.const import CONF_ID

CODEOWNERS = ["@theAltaran"]
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

    # TFT_eSPI and SPI are Arduino libraries that are disabled by default
    # since ESPHome 2026.2.0 (PR #13623). They must be enabled explicitly.
    # Safe on all ESPHome versions (no-op if already enabled).
    if CORE.is_esp32 and CORE.using_arduino:
        cg.add_library("TFT_eSPI", None, "Bodmer/TFT_eSPI @ ^2.5.30")
        cg.add_library("SPI", None)

    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
