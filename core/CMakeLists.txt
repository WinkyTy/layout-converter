# Core library CMakeLists.txt

# Create the core library
add_library(layout_converter_core SHARED
    src/layout_engine.cpp
    src/key_system.cpp
)

# Set include directories
target_include_directories(layout_converter_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Set compile definitions
target_compile_definitions(layout_converter_core
    PRIVATE
        LAYOUT_CONVERTER_EXPORTS
)

# Set properties
set_target_properties(layout_converter_core PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    OUTPUT_NAME "layout_converter_core"
)

# Install rules
install(TARGETS layout_converter_core
    EXPORT LayoutConverterTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/ DESTINATION include) 