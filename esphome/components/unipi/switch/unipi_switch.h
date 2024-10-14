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

  void set_pin(std::string pin) { this->pin_ = pin; }

 protected:
  void write_state(bool state) override;

  std::string pin_;

  FILE *fp_;
};

}  // namespace unipi
}  // namespace esphome
