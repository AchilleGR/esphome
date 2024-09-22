from esphome import pins
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import (
    CONF_ALLOW_OTHER_USES,
    CONF_ID,
    CONF_INPUT,
    CONF_MODE,
    CONF_NUMBER,
    CONF_OUTPUT,
)

CONF_UNIPI = "unipi"
CONF_SLOT = "slot"
CONF_RELAY = "relay"

unipi_ns = cg.esphome_ns.namespace(CONF_UNIPI)

UnipiComponent = unipi_ns.class_("UnipiComponent", cg.Component)
UnipiGPIOPin = unipi_ns.class_("UnipiGPIOPin", cg.GPIOPin)
IoMode = unipi_ns.enum("IoMode")

IO_MODES = {
    CONF_INPUT: IoMode.DI,
    CONF_OUTPUT: IoMode.DO,
    CONF_RELAY: IoMode.RO,
}

MULTI_CONF = True
CONFIG_SCHEMA = (
    cv.Schema({cv.Required(CONF_ID): cv.declare_id(UnipiComponent)})
    .extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)


def validate_mode(mode):
    if not (mode[CONF_INPUT] or mode[CONF_OUTPUT] or mode[CONF_RELAY]):
        raise cv.Invalid("Mode must be input, output or relay")
    if mode[CONF_INPUT] and mode[CONF_RELAY]:
        raise cv.Invalid("Mode cannot be input and relay at the same tiem")
    return mode


Unipi_PIN_SCHEMA = cv.All(
    {
        cv.GenerateID(): cv.declare_id(UnipiGPIOPin),
        cv.Required(CONF_UNIPI): cv.use_id(UnipiComponent),
        cv.Required(CONF_SLOT): cv.All(cv.int_range(min=1, max=3)),
        cv.Required(CONF_NUMBER): cv.All(cv.int_range(min=0, max=16)),
        cv.Optional(CONF_MODE, default={}): cv.All(
            {
                cv.Optional(CONF_INPUT, default=False): cv.boolean,
                cv.Optional(CONF_OUTPUT, default=False): cv.boolean,
                cv.Optional(CONF_RELAY, default=False): cv.boolean,
            },
            validate_mode,
        ),
    }
)


def mode_expr(mode):
    if mode.get(CONF_RELAY):
        return IoMode.RO
    elif mode.get(CONF_OUTPUT):
        return IoMode.DO
    elif mode.get(CONF_INPUT):
        return IoMode.DI


def unipi_pin_final_validate_config(pin_config, parent_config):
    # PinRegistry checks for uniqueness of CONF_NUMBER, but for us uniqueness
    # is (CONF_SLOT, CONF_NUMBER), i.e. 1.5 and 2.5 may both exist.
    pin_config[CONF_ALLOW_OTHER_USES] = True


@pins.PIN_SCHEMA_REGISTRY.register(CONF_UNIPI, Unipi_PIN_SCHEMA, unipi_pin_final_validate_config)
async def unipi_pin_to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    parent = await cg.get_variable(config[CONF_UNIPI])

    cg.add(var.set_parent(parent))
    cg.add(var.set_slot(config[CONF_SLOT]))
    cg.add(var.set_pin(config[CONF_NUMBER]))
    cg.add(var.set_mode(mode_expr(config[CONF_MODE])))

    return var
