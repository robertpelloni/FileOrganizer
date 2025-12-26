set(BENCH_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/bench_scanner.cpp
)

add_executable(fo_bench_scanner ${BENCH_SOURCES})

target_link_libraries(fo_bench_scanner PRIVATE fo_core)

target_compile_features(fo_bench_scanner PRIVATE cxx_std_20)

if(MSVC)
  target_compile_options(fo_bench_scanner PRIVATE /W4 /permissive-)
else()
  target_compile_options(fo_bench_scanner PRIVATE -Wall -Wextra -Wpedantic)
endif()

find_package(benchmark CONFIG QUIET)
if(benchmark_FOUND)
    add_executable(fo_benchmarks fo_benchmarks.cpp)
    target_link_libraries(fo_benchmarks PRIVATE fo_core benchmark::benchmark benchmark::benchmark_main)
    target_compile_features(fo_benchmarks PRIVATE cxx_std_20)
endif()

