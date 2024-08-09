// Copyright 2024 Khalil Estell
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <libhal-soft/inert_drivers/inert_temperature_sensor.hpp>

#include <boost/ut.hpp>

namespace hal::soft {
boost::ut::suite inert_temperature_sensor_test = []() {
  using namespace boost::ut;
  "inert_temperature_sensor"_test = []() {
    // Setup
    constexpr hal::celsius expected_temp = 0.5f;
    inert_temperature_sensor test(expected_temp);

    // Exercise
    auto result = test.read();

    // Verify
    expect(that % expected_temp == result);
  };
};
}  // namespace hal::soft
