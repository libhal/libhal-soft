# [DEPRECATED] libhal-soft

> [!CAUTION]
> This package is deprecated. The following files repos now have their own
> versions of each of these files. Prefer to use these.
>
> - [`libhal-expander`](https://github.com/libhal/libhal-expander)
>   - `adc_mux.hpp`
> - [`libhal-util`](https://github.com/libhal/libhal-util)
>   - `atomic_spin_lock.hpp`
>   - `bit_bang_i2c.hpp`
>   - `bit_bang_spi.hpp`
>   - `i2c_minimum_speed.hpp`
>   - `inverter.hpp`
> - [`libhal-actuator`](https://github.com/libhal/libhal-actuator)
>   - `rc_servo.hpp`

Library for generic soft drivers officially supported by libhal

Soft drivers are drivers that combine hardware interfaces together to emulate
the behavior of other interfaces. For a driver to be a soft driver it must
implement or have a way to generate, construct or create implementations of
hardware interfaces.

## License

Apache 2.0; see [`LICENSE`](LICENSE) for details.
