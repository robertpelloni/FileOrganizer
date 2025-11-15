cmake_minimum_required(VERSION 3.16)

project(FileOrganizerEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(FO_BUILD_CLI "Build the CLI engine" ON)
option(FO_BUILD_BENCH "Build benchmark tools" ON)

add_subdirectory(core)
if(FO_BUILD_CLI)
  add_subdirectory(cli)
endif()
if(FO_BUILD_BENCH)
  add_subdirectory(benchmarks)
endif()
