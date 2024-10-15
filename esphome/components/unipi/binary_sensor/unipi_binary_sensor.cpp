#include "unipi_binary_sensor.h"
#include "esphome/core/log.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include <cstdio>

namespace esphome {
namespace unipi {

static const char *const TAG = "unipi.binary_sensor";
static const char *const UNIPI_PLC = "/run/unipi-plc/by-sys";

void UnipiBinarySensor::setup() {
  std::string filename;

  filename = str_sprintf("%s/%s/value", UNIPI_PLC, this->pin_.c_str());

  ESP_LOGCONFIG(TAG, "Setting up Unipi, pin %s...", this->pin_.c_str());
  ESP_LOGD(TAG, "Opening... %s", filename.c_str());
  this->fp_ = fopen(filename.c_str(), "r");
  if (!this->fp_) {
    ESP_LOGE(TAG, "Could not open %s: %s", filename.c_str(), strerror(errno));
    this->mark_failed();
    return;
  }
  setbuf(this->fp_, nullptr);

  //  int val = getc(this->fp_);
  //  fflush(this->fp_);
  //  this->publish_initial_state(val != '0');
}

void UnipiBinarySensor::dump_config() {
  LOG_BINARY_SENSOR("", "Unipi Binary Sensor", this);

  if (!this->is_failed()) {
    ESP_LOGCONFIG(TAG, "  Pin: %s", this->pin_.c_str());
  } else {
    ESP_LOGE(TAG, "  Pin: %s (inoperable)", this->pin_.c_str());
  }
}

void UnipiBinarySensor::update() {
  rewind(this->fp_);
  int val = getc(this->fp_);
  fflush(this->fp_);

  this->publish_state(val != '0');
}

float UnipiBinarySensor::get_setup_priority() const { return setup_priority::IO; }

}  // namespace unipi
}  // namespace esphome
