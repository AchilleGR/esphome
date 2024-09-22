#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"
#include <cstdio>

namespace esphome {
namespace unipi {

enum IoMode { DI = 0, DO, RO };

class UnipiComponent : public Component {
 public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::IO; }
};

class UnipiGPIOPin : public GPIOPin, public Parented<UnipiComponent> {
 public:
  void set_slot(uint8_t slot) { this->slot_ = slot; }
  void set_pin(uint8_t pin) { this->pin_ = pin; }
  void set_mode(IoMode mode) { this->mode_ = mode; }
  void pin_mode(gpio::Flags flags) { }

  void setup() override;
  std::string dump_summary() const override;
  bool digital_read() override;
  void digital_write(bool value) override;

 protected:
  uint8_t slot_;
  uint8_t pin_;

  IoMode mode_;

  FILE *fp_;
};

}  // namespace unipi
}  // namespace esphome
