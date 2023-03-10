cmake_minimum_required(VERSION 3.15)

project(unit_test VERSION 0.0.1 LANGUAGES CXX)

if(DEFINED ENABLE_ASAN)
  set(ASAN_FLAG "-fsanitize=address" CACHE INTERNAL "Use AddressSanatizer")
endif(DEFINED ENABLE_ASAN)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # require at least gcc 11
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 11.0)
    message(FATAL_ERROR "GCC version must be at least 11!")
  endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # require at least clang 14
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 14)
    message(FATAL_ERROR "Clang version must be at least 14!")
  endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
  # require at least clang 14
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 14)
    message(FATAL_ERROR "Clang version must be at least 14!")
  endif()
else()
  message(WARNING "You are using an unsupported compiler! Compilation has only been tested with Clang and GCC, detected ${CMAKE_CXX_COMPILER_ID}.")
endif()

find_package(ut REQUIRED CONFIG)
find_package(libhal REQUIRED CONFIG)
find_package(libhal-util REQUIRED CONFIG)
find_package(libhal-mock REQUIRED CONFIG)

add_executable(${PROJECT_NAME}
  i2c_minimum_speed.test.cpp

  inert_accelerometer.test.cpp
  inert_adc.test.cpp
  inert_dac.test.cpp
  inert_distance_sensor.test.cpp
  inert_gyroscope.test.cpp
  inert_input_pin.test.cpp
  inert_interrupt_pin.test.cpp
  inert_magnetometer.test.cpp
  inert_motor.test.cpp
  inert_pwm.test.cpp
  inert_rotation_sensor.test.cpp
  inert_steady_clock.test.cpp
  inert_temperature_sensor.test.cpp
  inert_timer.test.cpp

  main.test.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC . ../include)
target_compile_options(${PROJECT_NAME} PRIVATE
  --coverage
  -fprofile-arcs
  -ftest-coverage
  ${ASAN_FLAG}
  -Werror
  -Wall
  -Wextra
  -Wshadow
  -Wnon-virtual-dtor
  -Wno-gnu-statement-expression
  -pedantic)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_link_options(${PROJECT_NAME} PRIVATE
  --coverage
  -fprofile-arcs
  -ftest-coverage
  ${ASAN_FLAG})
target_link_libraries(${PROJECT_NAME} PRIVATE 
  boost-ext-ut::ut 
  libhal::libhal 
  libhal::util 
  libhal::mock)

add_custom_command(TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND}
  -P ${CMAKE_SOURCE_DIR}/remove_gcda.cmake
  COMMENT "Deleting Previous Coverage Files..."
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
  