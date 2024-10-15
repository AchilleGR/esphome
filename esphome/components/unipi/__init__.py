import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

CONF_UNIPI = "unipi"

unipi_ns = cg.esphome_ns.namespace(CONF_UNIPI)

UnipiComponent = unipi_ns.class_("UnipiComponent", cg.Component)

MULTI_CONF = True
CONFIG_SCHEMA = cv.Schema({cv.Required(CONF_ID): cv.declare_id(UnipiComponent)}).extend(
    cv.COMPONENT_SCHEMA
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
