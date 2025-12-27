cmake_minimum_required(VERSION 3.16)

project(FileOrganizerEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

option(FO_BUILD_CLI "Build the CLI engine" ON)
option(FO_BUILD_BENCH "Build benchmark tools" ON)
option(FO_BUILD_TESTS "Build unit tests" ON)

add_subdirectory(core)
if(FO_BUILD_CLI)
  add_subdirectory(cli)
endif()
if(FO_BUILD_BENCH)
  add_subdirectory(benchmarks)
endif()
if(FO_BUILD_TESTS)
  add_subdirectory(tests)
endif()
