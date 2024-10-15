import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv
from esphome.const import CONF_PIN

from .. import unipi_ns

AUTO_LOAD = ["unipi"]

UnipiBinarySensor = unipi_ns.class_(
    "UnipiBinarySensor", binary_sensor.BinarySensor, cg.PollingComponent
)


def validate_pin(pin_str):
    pin_str = str(pin_str)
    if not pin_str.startswith("DI"):
        raise cv.Invalid("Binary Sensor operates only on digital inputs (DI)")
    return pin_str


CONFIG_SCHEMA = (
    binary_sensor.binary_sensor_schema(UnipiBinarySensor)
    .extend(
        {
            cv.Required(CONF_PIN): validate_pin,
        }
    )
    .extend(cv.polling_component_schema("40ms"))
)


async def to_code(config):
    var = await binary_sensor.new_binary_sensor(config)
    await cg.register_component(var, config)

    cg.add(var.set_pin(config[CONF_PIN]))
