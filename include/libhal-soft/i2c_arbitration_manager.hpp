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

#include <concepts>
#include <ranges>

#include <libhal/error.hpp>
#include <libhal/i2c.hpp>

namespace hal::soft {

/**
 * @brief A i2c wrapper to ensure that the lowest i2c device frequency is used.
 *
 */
template<std::derived_from<hal::i2c> i2c_type>
class i2c_arbitration_manager : public hal::i2c
{
public:
  /**
   * @brief Create minimum_speed_i2c object.
   *
   * @param p_i2c - i2c object that the device will use
   * @param p_retry_limit - number of attempts before calling the arbitration
   * handler
   * @param p_arbitration_handler - the call that will be invoked when a
   * transaction has been rejected for the p_retry_limit number of times.
   */
  i2c_arbitration_manager(i2c_type& p_i2c,
                          std::uint32_t p_retry_limit,
                          hal::callback<void(void)> p_arbitration_handler)
    : m_i2c(&p_i2c)
    , m_retry_limit(p_retry_limit)
    , m_arbitration_handler(std::move(p_arbitration_handler))
  {
  }

private:
  /**
   * @brief Pass through configuration function from this class to the passed
   * i2c driver.
   *
   * @param p_new_setting - settings to be set
   */
  void driver_configure(settings const& p_new_setting) override
  {
    m_i2c->configure(p_new_setting);
  }

  void driver_transaction(
    hal::byte p_address,
    std::span<hal::byte const> p_data_out,
    std::span<hal::byte> p_data_in,
    hal::function_ref<hal::timeout_function> p_timeout) override
  {
    for (auto _ : std::views::iota(0U, m_retry_limit + 1)) {
      try {
        m_i2c->transaction(p_address, p_data_out, p_data_in, p_timeout);
        return;
      } catch (hal::resource_unavailable_try_again const&) {
        continue;
      }
    }
    m_arbitration_handler();
    throw hal::timed_out(m_i2c);
  }

  i2c_type* m_i2c;
  std::uint32_t m_retry_limit = 0;
  hal::callback<void(void)> m_arbitration_handler = []() {};
};
}  // namespace hal::soft
