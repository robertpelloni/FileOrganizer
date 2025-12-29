find_package(GTest CONFIG REQUIRED)

add_executable(fo_tests
    test_rule_engine.cpp
    test_export.cpp
    test_hasher.cpp
    test_scanner.cpp
    test_database.cpp
    test_integration.cpp
)

target_link_libraries(fo_tests PRIVATE GTest::gtest GTest::gtest_main fo_core)

include(GoogleTest)
gtest_discover_tests(fo_tests)
