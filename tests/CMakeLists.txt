# Tests CMakeLists.txt

# Create test executable
add_executable(layout_converter_tests
    test_layout_converter.cpp
)

# Link against the core library
target_link_libraries(layout_converter_tests
    PRIVATE
        layout_converter_core
)

# Set include directories
target_include_directories(layout_converter_tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/core/include
)

# Add test
add_test(NAME KeyIDSystemTests COMMAND layout_converter_tests) 