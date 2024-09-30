#include "unipi_switch.h"
#include "esphome/core/log.h"
#include "esphome/components/switch/switch.h"
#include <cstdio>

namespace esphome {
namespace unipi {

static const char *const TAG = "unipi.switch";
static const char *const UNIPI_PLC = "/run/unipi-plc/by-sys";

void UnipiSwitch::setup() {
  const std::string type = "DO"; // XXX XXX XXX
  std::string filename;

  filename = str_sprintf("%s/%s%u.%u/value", UNIPI_PLC, type.c_str(), this->slot_, this->pin_);

  ESP_LOGCONFIG(TAG, "Setting up Unipi, pin %d.%d...", this->slot_, this->pin_);
  ESP_LOGD(TAG, "Opening... %s", filename.c_str());
  this->fp_ = fopen(filename.c_str(), "r+");
  if (!this->fp_) {
    ESP_LOGE(TAG, "Could not open %s", filename.c_str());
    this->mark_failed();
    return;
  }
  setbuf(this->fp_, nullptr);

//  int val = getc(this->fp_);
//  fflush(this->fp_);
//  this->publish_initial_state(val != '0');
}

void UnipiSwitch::dump_config() {
  LOG_SWITCH("", "Unipi", this);

  if (!this->is_failed()) {
    ESP_LOGCONFIG(TAG, "  Pin: %u.%u", this->slot_, this->pin_);
  } else {
    ESP_LOGE(TAG, "  Pin: %u.%u (inoperable)", this->slot_, this->pin_);
  }
}

void UnipiSwitch::update() {
  rewind(this->fp_);
  int val = getc(this->fp_);
  fflush(this->fp_);

  this->publish_state(val != '0');
}

void UnipiSwitch::write_state(bool state) {
  rewind(this->fp_);
  if (fwrite(state ? "1\n" : "0\n", 1, 2, this->fp_) != 2) {
    ESP_LOGE(TAG, "Could not write to pin %u.%u", this->slot_, this->pin_);
    // XXX: warning?
  }
  fflush(this->fp_);
}

float UnipiSwitch::get_setup_priority() const {
  return setup_priority::IO;
}

}  // namespace unipi
}  // namespace esphome
