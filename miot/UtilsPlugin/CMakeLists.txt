cmake_minimum_required(VERSION 3.10)

# Define project name and version
project(UtilsPlugin VERSION 0.1.0)
set(CMAKE_PREFIX_PATH "/home/justin/Qt/6.6.1/gcc_64/lib/cmake")

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Find the QtWidgets library
find_package(Qt6Core REQUIRED)

# Define a macro to indicate the library compilation
add_definitions(-DUTILSPLUGIN_LIBRARY)
# Add any additional definitions here
# Example: add_definitions(-DQT_MESSAGELOGCONTEXT)

# Set source and header files
set(SOURCES utilsplugin.cpp)
set(HEADERS UtilsPlugin_global.h utilsinterface.h utilsplugin.h)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../PluginInterface
    ${CMAKE_CURRENT_SOURCE_DIR}/../PluginManager)

# Create shared library
add_library(UtilsPlugin SHARED ${SOURCES} ${HEADERS})

# Use the Widgets module from Qt 5
target_link_libraries(UtilsPlugin Qt6::Core)

# Set the target properties
set_target_properties(UtilsPlugin PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Install the library to the desired output directory
install(TARGETS UtilsPlugin LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../../plugins)

# Additional files to distribute
set(DISTFILES utilsplugin.json)
