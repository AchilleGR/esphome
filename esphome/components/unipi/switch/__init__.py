import esphome.codegen as cg
from esphome.components import switch
import esphome.config_validation as cv
from esphome.const import CONF_PIN

from .. import unipi_ns

AUTO_LOAD = ["unipi"]

UnipiSwitch = unipi_ns.class_("UnipiSwitch", switch.Switch, cg.PollingComponent)


def validate_pin(pin_str):
    pin_str = str(pin_str)
    if not pin_str.startswith("DO") and not pin_str.startswith("RO"):
        raise cv.Invalid("Switch operates only on digital outs (DO), relays (RO)")
    return pin_str


CONFIG_SCHEMA = (
    switch.switch_schema(UnipiSwitch)
    .extend(
        {
            cv.Required(CONF_PIN): validate_pin,
        }
    )
    .extend(cv.polling_component_schema("40ms"))
)


async def to_code(config):
    var = await switch.new_switch(config)
    await cg.register_component(var, config)

    cg.add(var.set_pin(config[CONF_PIN]))
