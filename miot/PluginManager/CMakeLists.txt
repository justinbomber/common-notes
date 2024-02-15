# cmake_minimum_required(VERSION 3.5)

# project(PluginManager VERSION 0.1 LANGUAGES CXX)

# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
# find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# # Define macros
# add_definitions(-DPLUGINMANAGER)
# # Add any additional definitions here
# # Example: add_definitions(-DQT_MESSAGELOGCONTEXT)

# # Set source and header files
# set(SOURCES pluginmanager.cpp pluginspec.cpp)
# set(HEADERS pluginmanager.h pluginmanagerinterface.h pluginspec.h)

# # Include directories
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../PluginInterface)

# # Create shared library
# add_library(PluginManager SHARED ${SOURCES} ${HEADERS})

# # Link with Qt and Couchbase Lite C libraries
# target_link_libraries(PluginManager Qt5::Core Qt6::Core)

# # Set the target properties
# set_target_properties(PluginManager PROPERTIES VERSION ${PROJECT_VERSION})

# # Install the library to the desired output directory
# install(TARGETS PluginManager LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../../plugins)

# # Additional files to distribute
# set(DISTFILES pluginmanager.json)
cmake_minimum_required(VERSION 3.10)
project(PluginManager VERSION 0.1.0)
set(CMAKE_PREFIX_PATH "/home/justin/Qt/6.6.1/gcc_64/lib/cmake")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Core REQUIRED)

add_definitions(-DPLUGINMANAGER_LIBRARY)
add_definitions(-DQT_MESSAGELOGCONTEXT)

set(SOURCES 
    pluginmanager.cpp 
    pluginspec.cpp
)

set(HEADERS 
    pluginmanager.h 
    pluginmanagerinterface.h 
    pluginspec.h
)

include_directories($$PWD/../PluginInterface)

# Uncomment below line for static library, else shared library will be created
# add_library(PluginManager STATIC ${SOURCES} ${HEADERS})
add_library(PluginManager SHARED ${SOURCES} ${HEADERS})

target_link_libraries(PluginManager Qt6::Core)

set_target_properties(PluginManager PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

install(TARGETS PluginManager
    LIBRARY DESTINATION $$PWD/../../../../plugins
)

set(DISTFILES pluginmanager.json)
