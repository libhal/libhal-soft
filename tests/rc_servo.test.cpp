#include <libhal-soft/rc_servo.hpp>

#include <libhal-mock/pwm.hpp>
#include <libhal-mock/testing.hpp>

#include <boost/ut.hpp>

namespace hal {
void rc_servo_test()
{
  using namespace boost::ut;

  "hal::servo::rc_servo::create"_test = []() {
    // Setup
    hal::mock::pwm pwm0;
    hal::mock::pwm pwm1;
    hal::mock::pwm pwm2;

    // Exercise
    // use defaults
    auto servo0 = hal::rc_servo::create(pwm0);
    // 100Hz (or 10ms per update) with 500us being max negative start and 2500us
    // being max positive.
    auto servo1 = hal::rc_servo::create<100, 500, 2500>(pwm1);
    pwm2.spy_frequency.trigger_error_on_call(1);
    auto servo2 = hal::rc_servo::create(pwm2);

    // Verify
    expect(bool{ servo0 });
    expect(bool{ servo1 });
    expect(!servo2);
  };

  "hal::servo::rc_servo::position"_test = []() {
    // Setup
    constexpr auto expected0 = float(0.05);
    constexpr auto expected1 = float(0.10);
    constexpr auto expected2 = float(0.15);
    constexpr auto expected3 = float(0.20);
    constexpr auto expected4 = float(0.25);

    constexpr auto angle0 = hal::degrees(0);
    constexpr auto angle1 = hal::degrees(45);
    constexpr auto angle2 = hal::degrees(90);
    constexpr auto angle3 = hal::degrees(135);
    constexpr auto angle4 = hal::degrees(180);

    hal::mock::pwm pwm;
    auto servo = hal::rc_servo::create<100, 500, 2500, 0, 180>(pwm).value();

    // Exercise
    auto result0 = servo.position(angle0);
    auto result1 = servo.position(angle1);
    auto result2 = servo.position(angle2);
    auto result3 = servo.position(angle3);
    auto result4 = servo.position(angle4);

    // Verify
    expect(bool{ result0 });
    expect(bool{ result1 });
    expect(bool{ result2 });
    expect(bool{ result3 });
    expect(bool{ result4 });

    expect(that % expected0 ==
           std::get<0>(pwm.spy_duty_cycle.call_history().at(0)));
    expect(that % expected1 ==
           std::get<0>(pwm.spy_duty_cycle.call_history().at(1)));
    expect(that % expected2 ==
           std::get<0>(pwm.spy_duty_cycle.call_history().at(2)));
    expect(that % expected3 ==
           std::get<0>(pwm.spy_duty_cycle.call_history().at(3)));
    expect(that % expected4 ==
           std::get<0>(pwm.spy_duty_cycle.call_history().at(4)));
  };

  "hal::servo::rc_servo::position negative angles"_test = []() {
    // Setup
    constexpr auto expected0 = float(0.05);
    constexpr auto expected1 = float(0.10);
    constexpr auto expected2 = float(0.15);
    constexpr auto expected3 = float(0.20);
    constexpr auto expected4 = float(0.25);

    constexpr auto angle0 = hal::degrees(-90);
    constexpr auto angle1 = hal::degrees(-45);
    constexpr auto angle2 = hal::degrees(0);
    constexpr auto angle3 = hal::degrees(45);
    constexpr auto angle4 = hal::degrees(90);

    hal::mock::pwm pwm;
    auto servo = hal::rc_servo::create<100, 500, 2500, -90, 90>(pwm).value();

    // Exercise
    auto result0 = servo.position(angle0);
    auto result1 = servo.position(angle1);
    auto result2 = servo.position(angle2);
    auto result3 = servo.position(angle3);
    auto result4 = servo.position(angle4);

    // Verify
    expect(bool{ result0 });
    expect(bool{ result1 });
    expect(bool{ result2 });
    expect(bool{ result3 });
    expect(bool{ result4 });

    expect(that % expected0 ==
           std::get<0>(pwm.spy_duty_cycle.call_history().at(0)));
    expect(that % expected1 ==
           std::get<0>(pwm.spy_duty_cycle.call_history().at(1)));
    expect(that % expected2 ==
           std::get<0>(pwm.spy_duty_cycle.call_history().at(2)));
    expect(that % expected3 ==
           std::get<0>(pwm.spy_duty_cycle.call_history().at(3)));
    expect(that % expected4 ==
           std::get<0>(pwm.spy_duty_cycle.call_history().at(4)));
  };

  "servo errors test"_test = []() {
    // Setup
    hal::mock::pwm pwm;
    constexpr hal::degrees min_angle = 0.0f;
    constexpr hal::degrees max_angle = 90.0f;
    auto test = hal::rc_servo::create<100, 500, 2500>(pwm).value();

    // Exercise
    hal::attempt_all(
      [&test]() -> hal::status {
        HAL_CHECK(test.position(max_angle + 45.0f));
        return hal::new_error();
      },
      // Verify
      [min_angle, max_angle](std::errc p_error_code,
                             hal::servo::range_error p_range_error) {
        expect(std::errc::invalid_argument == p_error_code);
        expect(that % min_angle == p_range_error.min);
        expect(that % max_angle == p_range_error.max);
      },
      []() { expect(false) << "None of the above errors were thrown!"; });
  };
};
}  // namespace hal