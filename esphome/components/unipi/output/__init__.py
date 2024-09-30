from esphome import pins
import esphome.codegen as cg
from esphome.components import output
import esphome.config_validation as cv
from esphome.const import (
    CONF_ID,
    CONF_PIN,
)

from .. import unipi_ns

AUTO_LOAD = ["unipi"]

UnipiOutput = unipi_ns.class_("UnipiOutput", output.BinaryOutput, cg.Component)


def validate_pin(pin_str):
    pin_str = str(pin_str)
    if not pin_str.startswith("DO") and not pin_str.startswith("RO"):
        raise cv.Invalid("Output operates only on digital outs (DO), relays (RO)")

    try:
        slot, pin = (int(x) for x in pin_str[2:].split(".", 2))
    except:
        raise cv.Invalid(f"Invalid pin specification {pin_str}")

    return slot, pin


CONFIG_SCHEMA = (
    output.BINARY_OUTPUT_SCHEMA.extend(
        {
            cv.Required(CONF_ID): cv.declare_id(UnipiOutput),
            cv.Required(CONF_PIN): validate_pin,
        }
    )
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await output.register_output(var, config)

    cg.add(var.set_pin(*config[CONF_PIN]))
