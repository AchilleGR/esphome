#pragma once

#include "../unipi.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace unipi {

class UnipiBinarySensor : public binary_sensor::BinarySensor, public PollingComponent {
 public:
  void set_pin(UnipiGPIOPin *pin) { this->pin_ = pin; }
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override;
  void update() override;

 protected:
  UnipiGPIOPin *pin_;
};

}  // namespace unipi
}  // namespace esphome
