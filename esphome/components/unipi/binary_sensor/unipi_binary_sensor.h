#pragma once

#include "../unipi.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include <cstdio>

namespace esphome {
namespace unipi {

class UnipiBinarySensor : public binary_sensor::BinarySensor, public PollingComponent {
 public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override;
  void update() override;

  void set_pin(std::string pin) { this->pin_ = pin; }

 protected:
  std::string pin_;
  FILE *fp_;
};

}  // namespace unipi
}  // namespace esphome
