cmake_minimum_required(VERSION 3.5)

project(PluginManagerTest VERSION 0.1 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "/home/justin/Qt/6.6.1/gcc_64/lib/cmake")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wno-unknown-pragmas)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Qml Gui)

# 添加子目錄
add_subdirectory(UtilsPlugin)
add_subdirectory(CBLPlugin)
add_subdirectory(DDSPlugin)
add_subdirectory(PluginManager)
add_subdirectory(CalculatePlugin)

# 設置 C++ 標準

# 添加源文件和頭文件
set(SOURCE_FILES
    PluginManagerTest/main.cpp
    PluginManagerTest/testpubobject.cpp
    PluginManagerTest/testsubobject.cpp
    PluginManagerTest/widget.cpp)

set(HEADER_FILES
    PluginManagerTest/testpubobject.h
    PluginManagerTest/testsubobject.h
    PluginManagerTest/widget.h
    PluginManagerTest/widget.ui)

# 處理 Qt UI 檔案
# set(UI_FILES PluginManagerTest/widget.ui)

# 使用 qt5_wrap_ui
# qt5_wrap_ui(UI_HEADERS ${UI_FILES})

# 包含目錄
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/PluginInterface
    ${CMAKE_CURRENT_SOURCE_DIR}/PluginManager
    ${CMAKE_CURRENT_SOURCE_DIR}/CalculatePlugin
    ${CMAKE_CURRENT_SOURCE_DIR}/DDSPlugin
    ${CMAKE_CURRENT_SOURCE_DIR}/CBLPlugin
    ${CMAKE_CURRENT_SOURCE_DIR}/UtilsPlugin
    $ENV{HOME}/rti_connext_dds-6.1.2/include
    $ENV{HOME}/rti_connext_dds-6.1.2/include/ndds
    $ENV{HOME}/rti_connext_dds-6.1.2/include/ndds/hpp
    /usr/include/cbl
    /usr/include/cbl++)

link_directories(
    $ENV{HOME}/rti_connext_dds-6.1.2/lib/x64Linux4gcc7.3.0
    /usr/lib/x86_64-linux-gnu)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 添加可執行文件
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

# 定義編譯器標誌
target_compile_definitions(${PROJECT_NAME} PRIVATE RTI_UNIX RTI_LINUX RTI_64BIT QT_MESSAGELOGCONTEXT)

# 鏈接 Qt5 和其他庫
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE nddscpp2 nddsc nddscore -ldl -lm -lpthread -rdynamic CalculatePlugin UtilsPlugin CBLPlugin DDSPlugin PluginManager)

# 安裝路徑
install(TARGETS ${PROJECT_NAME} DESTINATION /opt/${PROJECT_NAME}/)
