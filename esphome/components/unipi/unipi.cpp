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

}  // namespace unipi
}  // namespace esphome
