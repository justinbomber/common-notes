cmake_minimum_required(VERSION 3.5)

project(CBLPlugin VERSION 0.1 LANGUAGES CXX)
set(CMAKE_PREFIX_PATH "/home/justin/Qt/6.6.1/gcc_64/lib/cmake")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Define macros
add_definitions(-DCBLPLUGIN_LIBRARY)
# Add any additional definitions here
# Example: add_definitions(-DQT_MESSAGELOGCONTEXT)

# Set source and header files
set(SOURCES cblplugin.cpp)
set(HEADERS CBLPlugin_global.h cblinterface.h cblplugin.h fhir_resources.h)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../PluginInterface)
include_directories(/usr/include/cbl)
include_directories(/usr/include/cbl++)

# Create shared library
add_library(CBLPlugin SHARED ${SOURCES} ${HEADERS})

# Link with Qt and Couchbase Lite C libraries
target_link_libraries(CBLPlugin Qt6::Core)
target_link_libraries(CBLPlugin -lcblite)

# Set the target properties
set_target_properties(CBLPlugin PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Install the library to the desired output directory
install(TARGETS CBLPlugin LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../../plugins)

# Additional files to distribute
set(DISTFILES cblplugin.json)
