cmake_minimum_required(VERSION 3.10)
set(CMAKE_PREFIX_PATH "/home/justin/Qt/6.6.1/gcc_64/lib/cmake")

# Define project name and version
project(CalculatePlugin VERSION 0.1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Find the QtWidgets library
find_package(Qt6Core REQUIRED)

# Define a macro to indicate the library compilation
add_definitions(-DCALCULATEPLUGIN_LIBRARY)
# Add any additional definitions here
# Example: add_definitions(-DQT_MESSAGELOGCONTEXT)

# Set source and header files
set(SOURCES calculateplugin.cpp)
set(HEADERS CalculatePlugin_global.h calculateinterface.h calculateplugin.h)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../PluginInterface)

# Create shared library
add_library(CalculatePlugin SHARED ${SOURCES} ${HEADERS})

# Use the Widgets module from Qt 5
target_link_libraries(CalculatePlugin Qt6::Core)

# Set the target properties
set_target_properties(CalculatePlugin PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Install the library to the desired output directory
install(TARGETS CalculatePlugin LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../../plugins)

# Additional files to distribute
set(DISTFILES calculateplugin.json)
