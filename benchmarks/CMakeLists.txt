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
