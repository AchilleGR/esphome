#include <gtest/gtest.h>

#include <array>
#include <cstdint>

#include "esphome/components/modbus_controller/modbus_controller.h"

namespace esphome::modbus_controller::testing {

// A probe must come due exactly once per offline_skip_updates + 1 cycles from the trip point,
// for every phase between the trip cycle and the update counter. Pins the regression where a
// probe additionally required a range's skip_updates cadence to coincide, which some phase
// combinations never satisfy - the device then never polled again.
TEST(OfflineRetryCadence, DueOncePerWindowForEveryPhase) {
  for (uint16_t skip = 0; skip <= 5; skip++) {
    const uint16_t period = skip + 1;
    for (uint16_t offline_at = 0; offline_at <= 7; offline_at++) {
      uint16_t due_count = 0;
      for (uint32_t counter = offline_at; counter < offline_at + 4u * period; counter++) {
        if (offline_retry_due(static_cast<uint16_t>(counter), offline_at, skip))
          due_count++;
      }
      EXPECT_EQ(due_count, 4) << "skip=" << skip << " offline_at=" << offline_at;
    }
  }
}

// The first probe goes out within one window of going offline: after at most skip skipped cycles.
TEST(OfflineRetryCadence, FirstProbeWithinOneWindow) {
  for (uint16_t skip = 0; skip <= 5; skip++) {
    for (uint16_t offline_at = 0; offline_at <= 7; offline_at++) {
      uint16_t counter = offline_at;
      uint16_t skipped = 0;
      while (!offline_retry_due(counter, offline_at, skip)) {
        counter++;
        skipped++;
        ASSERT_LE(skipped, skip) << "skip=" << skip << " offline_at=" << offline_at;
      }
    }
  }
}

// The cadence neither stretches nor collapses when update_counter_ wraps past 65535.
TEST(OfflineRetryCadence, SurvivesCounterWraparound) {
  const uint16_t skip = 2;  // period 3
  const uint16_t offline_at = 65530;
  uint16_t counter = offline_at;
  uint16_t due_count = 0;
  for (int i = 0; i < 30; i++) {  // crosses the wrap mid-run
    if (offline_retry_due(counter, offline_at, skip))
      due_count++;
    counter++;
  }
  EXPECT_EQ(due_count, 10);
}

// An outage can last through many complete counter wraps. Reanchoring each issued probe keeps
// every window intact, including when the period does not divide 65536.
TEST(OfflineRetryCadence, RepeatedWindowsDoNotShortenAcrossFullCounterWraps) {
  constexpr std::array<uint16_t, 8> skips{0, 1, 11, 59, 119, 239, 1499, 65535};
  for (uint16_t skip : skips) {
    const uint32_t period = static_cast<uint32_t>(skip) + 1;
    for (uint16_t initial : {uint16_t{0}, uint16_t{32760}, uint16_t{65530}}) {
      uint16_t anchor = initial;
      uint16_t counter = initial;
      uint32_t previous = 0;
      uint32_t probes = 0;
      for (uint32_t tick = 1; tick <= 131072; tick++, counter++) {
        if (!offline_retry_due(counter, anchor, skip))
          continue;
        EXPECT_EQ(tick - previous, period) << "skip=" << skip << " initial=" << initial;
        previous = tick;
        probes++;
        anchor = static_cast<uint16_t>(counter + 1);
      }
      EXPECT_EQ(probes, 131072 / period);
    }
  }
}

}  // namespace esphome::modbus_controller::testing
