# Tests CMakeLists.txt

# Find Google Test
find_package(GTest QUIET)
if(NOT GTest_FOUND)
    # If GTest is not found, we'll skip tests for now
    message(WARNING "Google Test not found. Tests will be skipped.")
    return()
endif()

# Create test executable
add_executable(layout_converter_tests
    test_layout_converter.cpp
)

# Link against the core library and GTest
target_link_libraries(layout_converter_tests
    PRIVATE
        layout_converter_core
        GTest::gtest
        GTest::gtest_main
)

# Set include directories
target_include_directories(layout_converter_tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/core/include
)

# Add test
add_test(NAME LayoutConverterTests COMMAND layout_converter_tests) 