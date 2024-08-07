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

#include <libhal-soft/i2c_minimum_speed.hpp>

#include <libhal-util/math.hpp>

namespace hal::soft {
/**
 * @addtogroup i2c
 * @{
 */

minimum_speed_i2c::minimum_speed_i2c(hal::i2c& p_i2c, hertz p_frequency)
  : m_i2c(&p_i2c)
  , m_lowest_seen_frequency(p_frequency)
{
}

void minimum_speed_i2c::driver_configure(settings const& p_new_setting)
{
  if (m_lowest_seen_frequency > p_new_setting.clock_rate) {
    m_lowest_seen_frequency = p_new_setting.clock_rate;
    m_i2c->configure(p_new_setting);
  }
}

void minimum_speed_i2c::driver_transaction(
  hal::byte p_address,
  std::span<hal::byte const> p_data_out,
  std::span<hal::byte> p_data_in,
  hal::function_ref<hal::timeout_function> p_timeout)
{
  m_i2c->transaction(p_address, p_data_out, p_data_in, p_timeout);
}
/** @} */
}  // namespace hal::soft
