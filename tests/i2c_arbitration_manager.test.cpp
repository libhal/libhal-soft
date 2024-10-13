#include <libhal-soft/i2c_arbitration_manager.hpp>

#include <libhal-mock/testing.hpp>

#include <boost/ut.hpp>
#include <libhal/error.hpp>

namespace {
struct fake_i2c : public hal::i2c
{
  void reset()
  {
    spy_configure.reset();
    spy_transaction.reset();
  }
  // Spy handler for hal::i2c::configure()
  hal::spy_handler<settings> spy_configure;
  /// Record of the out data from hal::i2c::transaction()
  hal::spy_handler<hal::byte,
                   std::span<hal::byte const>,
                   std::span<hal::byte>,
                   std::function<hal::timeout_function>>
    spy_transaction;

private:
  void driver_configure(settings const& p_settings) final
  {
    spy_configure.record(p_settings);
  }

  void driver_transaction(
    [[maybe_unused]] hal::byte p_address,
    [[maybe_unused]] std::span<hal::byte const> p_data_out,
    [[maybe_unused]] std::span<hal::byte> p_data_in,
    [[maybe_unused]] hal::function_ref<hal::timeout_function> p_timeout) final
  {
    throw hal::resource_unavailable_try_again(this);
  }
};

struct fake_i2c2 : public fake_i2c
{};
}  // namespace

namespace hal {
boost::ut::suite test_i2c_arbitration_manager = []() {
  using namespace boost::ut;

  "hal::soft::i2c_arbitration_manager"_test = []() {
    "::configure should simply pass info through"_test = []() {
      // Setup
      fake_i2c mock_i2c;
      bool arbitration_handler_called = false;
      hal::soft::i2c_arbitration_manager managed_i2c(
        mock_i2c, 4, [&arbitration_handler_called]() {
          arbitration_handler_called = true;
        });

      constexpr hal::i2c::settings minimum_default = {
        .clock_rate = 400'000.0,
      };
      constexpr hal::i2c::settings expected_upper_boundary = {
        .clock_rate = 3'000'000,
      };
      constexpr hal::i2c::settings expected_lower = { .clock_rate = 5 };

      // Exercise
      managed_i2c.configure(expected_upper_boundary);
      managed_i2c.configure(minimum_default);
      managed_i2c.configure(expected_lower);

      // Verify
      expect(false);
    };

    "transaction"_test = []() {
      // Setup
      constexpr hal::byte expected_address{ 0xAA };
      constexpr std::array<hal::byte const, 2> data_out{ hal::byte{ 0xAB },
                                                         hal::byte{ 0xFF } };
      std::span<hal::byte> data_in;
      bool has_been_called = false;
      std::function<hal::timeout_function> expected_timeout =
        [&has_been_called]() { has_been_called = true; };
      fake_i2c mock_i2c;
      fake_i2c2 mock_i2c2;
      bool arbitration_handler_called = false;
      hal::soft::i2c_arbitration_manager managed_i2c(
        mock_i2c, 4, [&arbitration_handler_called]() {
          arbitration_handler_called = true;
        });
      hal::soft::i2c_arbitration_manager managed_i2c2(
        mock_i2c2, 6, [&arbitration_handler_called]() {
          arbitration_handler_called = true;
        });

      static_assert(
        not std::is_same_v<decltype(managed_i2c), decltype(managed_i2c2)>,
        "Are not the same!");

      // Exercise
      try {
        managed_i2c.transaction(
          expected_address, data_out, data_in, expected_timeout);
      } catch (hal::timed_out const&) {
        expect(not arbitration_handler_called);
      }

// Verify
#if 0
      auto transaction_call_info =
        mock_i2c.spy_transaction.call_history().at(0);
      auto transaction_expected_address = std::get<0>(transaction_call_info);
      auto transaction_data_out = std::get<1>(transaction_call_info);
      auto transaction_data_in = std::get<2>(transaction_call_info);

      expect(expected_address == transaction_expected_address);
      expect(data_out.data() == transaction_data_out.data());
      expect(data_out.size() == transaction_data_out.size());
      expect(data_in.data() == transaction_data_in.data());
      expect(data_in.size() == transaction_data_in.size());
      expect(has_been_called);
#endif
      expect(false);
    };
  };
};
}  // namespace hal
