#include "unipi_binary_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace unipi {

static const char *const TAG = "unipi.binary_sensor";

void UnipiBinarySensor::setup() {
  this->pin_->setup();
  this->publish_initial_state(this->pin_->digital_read());
}

void UnipiBinarySensor::dump_config() {
  LOG_BINARY_SENSOR("", "Unipi Binary Sensor", this);
  LOG_PIN("  Pin: ", this->pin_);
}

void UnipiBinarySensor::update() {
  this->publish_state(this->pin_->digital_read());
}

float UnipiBinarySensor::get_setup_priority() const { return setup_priority::HARDWARE; }

}  // namespace unipi
}  // namespace esphome
