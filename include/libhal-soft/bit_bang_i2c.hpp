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

#include <chrono>
#include <libhal/i2c.hpp>
#include <libhal/output_pin.hpp>
#include <libhal/units.hpp>

namespace hal {
/**
 * @brief A bit bang implementation for i2c.
 *
 * This implementation of i2c only needs 2 gpios and a steady_clock to work
 * correctly. It supports single-controller multiple peripherals but  we intend
 * to support multiple controllers in the future. bit-bang is a best-effort
 * implementation of i2c. The maximum achievable clock rate for the lpc4078 is
 * about 180kHz. Interrupts disrupt this controller.
 */
class bit_bang_i2c : public i2c
{
public:
  struct pins
  {
    output_pin* sda;
    output_pin* scl;
  };

  /**
   * @brief Construct a new i2c bit bang object
   *
   * @param p_pins This holds both scl and sda to be used inside of the driver
   * @param p_steady_clock A steady clock that should have a higher frequency
   * then the configured frequency for the bit bang
   * @param p_duty_cycle The duty cycle that the clock sent over scl will run at
   */
  bit_bang_i2c(pins p_pins,
               steady_clock& p_steady_clock,
               const float p_duty_cycle = 0.5f);

private:
  void driver_configure(const settings& p_settings) override;

  virtual void driver_transaction(
    hal::byte p_address,
    std::span<const hal::byte> p_data_out,
    std::span<hal::byte> p_data_in,
    function_ref<hal::timeout_function> p_timeout) override;

  /**
   * @brief This function will send the start condition it will also pull the
   * pins high before sending the start condition
   */
  void send_start_condition();

  /**
   * @brief This function will send the stop condition while also making sure
   * the sda pin is pulled low before sending the stop condition
   */
  void send_stop_condition();

  /**
   * @brief This function will go through the steps of writing the address
   * of the peripheral the controller wishes to speak to while also ensuring the
   * data written is acknowledged
   *
   * @param p_address The address of the peripheral, configured with the
   * read/write bit already that the controller is requesting to talk to
   * @param p_timeout A timeout function which is primarily used for clock
   * stretching to ensure the peripheral doesn't hold the line too long
   *
   * @throws hal::no_such_device when the address written to the bus was not
   * acknowledged
   */
  void write_address(hal::byte p_address,
                     function_ref<hal::timeout_function> p_timeout);

  /**
   * @brief This function will write the entire contents of the span out to the
   * bus while making sure all data gets acknowledged
   *
   * @param p_data_out This is a span of bytes which will be written to the bus
   * @param p_timeout A timeout function which is primarily used for clock
   * stretching to ensure the peripheral doesn't hold the line too long
   *
   * @throws hal::io_error when the data written to the bus was not
   * acknowledged
   */
  void write(std::span<const hal::byte> p_data_out,
             function_ref<hal::timeout_function> p_timeout);

  /**
   * @brief This function will handle writing a singular byte each call while
   * also retrieving the acknowledge bits
   *
   * @param p_byte_to_write This is the byte that will be written to the bus
   * @param p_timeout A timeout function which is primarily used for clock
   * stretching to ensure the peripheral doesn't hold the line too long
   *
   * @return bool true when the byte written was ack'd and false when it was
   * nack'd
   */
  bool write_byte(hal::byte p_byte_to_write,
                  function_ref<hal::timeout_function> p_timeout);

  /**
   * @brief This function will write a single bit at a time, dealing with
   * simulating the clock and the clock stretching feature
   *
   * @param p_bit_to_write The bit which will be written on the bus
   * @param p_timeout A timeout function which is primarily used for clock
   * stretching to ensure the peripheral doesn't hold the line too long
   */
  void write_bit(hal::byte p_bit_to_write,
                 function_ref<hal::timeout_function> p_timeout);

  /**
   * @brief This function will read in as many bytes as allocated inside of the
   * span while also acking or nacking the data
   *
   * @param p_data_in A span which will be filled with the bytes that will be
   * read from the bus
   * @param p_timeout A timeout function which is primarily used for clock
   * stretching to ensure the peripheral doesn't hold the line too long
   */
  void read(std::span<hal::byte> p_data_in,
            function_ref<hal::timeout_function> p_timeout);

  /**
   * @brief This function is responsible for reading a byte at a time off the
   * bus and also creating the byte from bits
   *
   * @return hal::byte the byte that has been read off of the bus
   */
  hal::byte read_byte();

  /**
   * @brief This function is responsible for reading a single bit at a time. It
   * will manage the clock and will release sda (pull it high) every time it is
   * called
   *
   * @return hal::byte which will house the single byte read from the bus
   */
  hal::byte read_bit();

  /// @brief An output pin which is the i2c scl pin
  output_pin* m_scl;

  /// @brief An output pin which is the i2c sda pin
  output_pin* m_sda;

  /// @brief A steady_clock provides a mechanism to delay the clock pulses of
  /// the scl line.
  steady_clock* m_clock;

  /// @brief The time that scl will be held high for
  uint64_t m_scl_high_ticks;

  /// @brief The time that scl will be held low for
  uint64_t m_scl_low_ticks;

  /// @brief All the information that the bus will need to operate on
  float m_duty_cycle;
};
}  // namespace hal
