# [DEPRECATED] libhal-soft

> [!CAUTION]
> This package is deprecated. The following files repos now have their own
> versions of each of these files. Prefer to use these.
>
> - `libhal-expander`
>   - `adc_mux.hpp`
> - `libhal-util`
>   - `atomic_spin_lock.hpp`
>   - `bit_bang_i2c.hpp`
>   - `bit_bang_spi.hpp`
>   - `i2c_minimum_speed.hpp`
>   - `inverter.hpp`
> - `libhal-actuator`
>   - `rc_servo.hpp`

[![✅CI](https://github.com/libhal/libhal-soft/actions/workflows/ci.yml/badge.svg)](https://github.com/libhal/libhal-soft/actions/workflows/ci.yml)
[![GitHub stars](https://img.shields.io/github/stars/libhal/libhal-soft.svg)](https://github.com/libhal/libhal-soft/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/libhal/libhal-soft.svg)](https://github.com/libhal/libhal-soft/network)
[![GitHub issues](https://img.shields.io/github/issues/libhal/libhal-soft.svg)](https://github.com/libhal/libhal-soft/issues)

Library for generic soft drivers officially supported by libhal

Soft drivers are drivers that combine hardware interfaces together to emulate the behavior of other interfaces. For a driver to be a soft driver it must implement or have a way to generate, construct or create implementations of hardware interfaces.

## Contributing

See [`CONTRIBUTING.md`](CONTRIBUTING.md) for details.

## License

Apache 2.0; see [`LICENSE`](LICENSE) for details.
