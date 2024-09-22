#include "unipi.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include <cstdio>
#include <sys/stat.h>

namespace esphome {
namespace unipi {

static const char *const TAG = "unipi";
static const char *const UNIPI_PLC = "/run/unipi-plc/by-sys";

void UnipiComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Unipi...");

  struct stat sb;
  if (stat(UNIPI_PLC, &sb) == -1) {
    ESP_LOGE(TAG, "Cannot stat %s", UNIPI_PLC);
    this->mark_failed();
    return;
  }
  if ((sb.st_mode & S_IFMT) != S_IFDIR) {
    ESP_LOGE(TAG, "%s is not a directory", UNIPI_PLC);
    this->mark_failed();
    return;
  }
}

void UnipiComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Unipi:");

  if (this->is_failed()) {
    ESP_LOGE(TAG, "Inoperable, %s not found", UNIPI_PLC);
  }
}

void UnipiGPIOPin::setup() {
  std::string type, filename, fmode;

  switch (this->mode_) {
    case IoMode::DI:
      type = "DI";
      fmode = "r";
      break;
    case IoMode::DO:
      type = "DO";
      fmode = "r+";
      break;
    case IoMode::RO:
      type = "RO";
      fmode = "r+";
      break;
  }

  filename = str_sprintf("%s/%s%u.%u/value", UNIPI_PLC, type.c_str(), this->slot_, this->pin_);

  ESP_LOGD(TAG, "Opening... %s", filename.c_str());
  this->fp_ = fopen(filename.c_str(), fmode.c_str());
  if (!this->fp_) {
    ESP_LOGE(TAG, "Could not open %s", filename.c_str());
    this->parent_->status_set_warning("non-existent pin");
    return;
  }
  setbuf(this->fp_, nullptr);
}

std::string UnipiGPIOPin::dump_summary() const {
  return str_sprintf("%u.%u via Unipi", this->slot_, this->pin_);
}

bool UnipiGPIOPin::digital_read() {
  if (!this->fp_) {
    return false;
  }

  rewind(this->fp_);
  int val = getc(this->fp_);
  fflush(this->fp_);

  return val != '0';
}

void UnipiGPIOPin::digital_write(bool value) {
  if (!this->fp_) {
    return;
  }

  rewind(this->fp_);
  if (fwrite(value ? "1\n" : "0\n", 1, 2, this->fp_) != 2) {
    ESP_LOGE(TAG, "Could not write to pin %u.%u", this->slot_, this->pin_);
  }
  fflush(this->fp_);
}

}  // namespace unipi
}  // namespace esphome
