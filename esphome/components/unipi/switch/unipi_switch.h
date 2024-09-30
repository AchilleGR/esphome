#pragma once

#include "../unipi.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"
#include "esphome/components/switch/switch.h"
#include <cstdio>

namespace esphome {
namespace unipi {

class UnipiSwitch : public switch_::Switch, public PollingComponent {
 public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override;
  void update() override;

  void set_pin(uint8_t slot, uint8_t pin) { this->slot_ = slot; this->pin_ = pin; }

 protected:
  void write_state(bool state) override;

  uint8_t slot_;
  uint8_t pin_;

  FILE *fp_;
};

}  // namespace unipi
}  // namespace esphome
