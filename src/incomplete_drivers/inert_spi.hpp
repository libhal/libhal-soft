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

#include <libhal/spi.hpp>

namespace hal::soft {
/**
 * @brief Inert implementation of Serial peripheral interface (SPI)
 * communication protocol hardware
 *
 */
class inert_spi : public hal::spi
{
public:
  /**
   * @brief Create inert_spi object
   *
   * @return result<inert_spi> - Constructed inert_spi object
   */
  static result<inert_spi> create()
  {
    return inert_spi();
  }

private:
  constexpr inert_spi()
  {
  }

  status driver_configure([[maybe_unused]] settings const& p_settings)
  {
    return hal::success();
  }

  result<transfer_t> driver_transfer(
    [[maybe_unused]] std::span<hal::byte const> p_data_out,
    [[maybe_unused]] std::span<hal::byte> p_data_in,
    [[maybe_unused]] hal::byte p_filler)
  {
    return transfer_t{};
  }
};
}  // namespace hal::soft
