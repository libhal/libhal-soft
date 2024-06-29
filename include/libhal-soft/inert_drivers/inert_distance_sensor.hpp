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

#pragma once

#include <libhal/distance_sensor.hpp>

namespace hal::soft {
/**
 * @brief Inert implementation of linear distance hardware
 *
 */
class inert_distance_sensor : public hal::distance_sensor
{
public:
  /**
   * @brief Create inert_distance_sensor object
   *
   * @param p_result - what will be returned from the read function.
   */
  constexpr inert_distance_sensor(hal::meters p_result)
    : m_result(p_result)
  {
  }

private:
  hal::meters driver_read()
  {
    return m_result;
  }

  hal::meters m_result;
};
}  // namespace hal::soft
