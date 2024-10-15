#include "unipi_output.h"
#include "esphome/core/log.h"
#include "esphome/components/output/binary_output.h"
#include <cstdio>

namespace esphome {
namespace unipi {

static const char *const TAG = "unipi.output";
static const char *const UNIPI_PLC = "/run/unipi-plc/by-sys";

void UnipiOutput::setup() {
  std::string filename;

  filename = str_sprintf("%s/%s/value", UNIPI_PLC, this->pin_.c_str());

  ESP_LOGCONFIG(TAG, "Setting up Unipi, pin %s...", this->pin_.c_str());
  ESP_LOGD(TAG, "Opening... %s", filename.c_str());
  this->fp_ = fopen(filename.c_str(), "r+");
  if (!this->fp_) {
    ESP_LOGE(TAG, "Could not open %s: %s", filename.c_str(), strerror(errno));
    this->mark_failed();
    return;
  }
  setbuf(this->fp_, nullptr);
}

void UnipiOutput::dump_config() {
  LOG_BINARY_OUTPUT(this);

  if (!this->is_failed()) {
    ESP_LOGCONFIG(TAG, "  Pin: %s", this->pin_.c_str());
  } else {
    ESP_LOGE(TAG, "  Pin: %s (inoperable)", this->pin_.c_str());
  }
}

void UnipiOutput::write_state(bool state) {
  rewind(this->fp_);
  if (fwrite(state ? "1\n" : "0\n", 1, 2, this->fp_) != 2) {
    ESP_LOGE(TAG, "Could not write to pin %s: %s", this->pin_.c_str(), strerror(errno));
    // XXX: warning?
  }
  fflush(this->fp_);
}

float UnipiOutput::get_setup_priority() const { return setup_priority::IO; }

}  // namespace unipi
}  // namespace esphome
