from esphome import pins
import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv
from esphome.const import CONF_PIN

from .. import unipi_ns

UnipiBinarySensor = unipi_ns.class_("UnipiBinarySensor", binary_sensor.BinarySensor, cg.PollingComponent)

CONFIG_SCHEMA = (
    binary_sensor.binary_sensor_schema(UnipiBinarySensor)
    .extend(
        {
            cv.Required(CONF_PIN): pins.gpio_input_pin_schema,
        }
    )
    .extend(cv.polling_component_schema("100ms"))
)


async def to_code(config):
    var = await binary_sensor.new_binary_sensor(config)
    await cg.register_component(var, config)

    pin = await cg.gpio_pin_expression(config[CONF_PIN])
    cg.add(var.set_pin(pin))
