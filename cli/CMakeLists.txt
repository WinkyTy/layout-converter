# CLI tool CMakeLists.txt

# Create the CLI executable
add_executable(layout_converter_cli
    main.cpp
)

# Link against the core library
target_link_libraries(layout_converter_cli
    PRIVATE
        layout_converter_core
)

# Set include directories
target_include_directories(layout_converter_cli
    PRIVATE
        ${CMAKE_SOURCE_DIR}/core/include
)

# Set properties
set_target_properties(layout_converter_cli PROPERTIES
    OUTPUT_NAME "layout_converter"
)

# Install rules
install(TARGETS layout_converter_cli
    RUNTIME DESTINATION bin
) 