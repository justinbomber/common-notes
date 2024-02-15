cmake_minimum_required(VERSION 3.10)

# Define project name and version
project(DDSPlugin VERSION 0.1.0)
set(CMAKE_PREFIX_PATH "/home/justin/Qt/6.6.1/gcc_64/lib/cmake")

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -Wall -std=c++17")

# Find the Qt5Core and Qt5Network libraries
find_package(Qt6Core REQUIRED)
find_package(Qt6Network REQUIRED)

# Define macros and flags
add_definitions(-DDDSPLUGIN_LIBRARY -DRTI_UNIX -DRTI_LINUX -DRTI_64BIT)

# Set source and header files
set(SOURCES ddsplugin.cpp)
set(HEADERS DDSPlugin_global.h ddsinterface.h ddsplugin.h)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../PluginInterface)
include_directories($ENV{ROOT}/usr/include/x86_64-linux-gnu)
include_directories($ENV{HOME}/rti_connext_dds-6.1.2/include)
include_directories($ENV{HOME}/rti_connext_dds-6.1.2/include/ndds)
include_directories($ENV{HOME}/rti_connext_dds-6.1.2/include/ndds/hpp)

link_directories($ENV{HOME}/rti_connext_dds-6.1.2/lib/x64Linux4gcc7.3.0)
link_directories($ENV{ROOT}/lib/x86_64-linux-gnu)

# Create shared library
add_library(DDSPlugin SHARED ${SOURCES} ${HEADERS})

# Link with Qt and RTI DDS libraries
target_link_libraries(DDSPlugin Qt6::Core Qt6::Network)
target_link_libraries(DDSPlugin nddscpp2 nddsc nddscore -ldl -lm -lpthread -rdynamic)

# Set the target properties
set_target_properties(DDSPlugin PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Install the library to the desired output directory
install(TARGETS DDSPlugin LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../../plugins)

# Additional files to distribute
set(DISTFILES ddsplugin.json)
