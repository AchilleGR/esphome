#pragma once

#include "../unipi.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"
#include "esphome/components/output/binary_output.h"
#include <cstdio>

namespace esphome {
namespace unipi {

class UnipiOutput : public output::BinaryOutput, public Component {
 public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override;

  void set_pin(std::string pin) { this->pin_ = pin; }

 protected:
  void write_state(bool state) override;

  std::string pin_;

  FILE *fp_;
};

}  // namespace unipi
}  // namespace esphome
