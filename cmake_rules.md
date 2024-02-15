# Cmake 寫法

## 1. 語法

**默認變數名稱解釋:**
- `${PROJECT_SOURCE_DIR}`:專案目錄
- `${PROJECT_NAME}`:專案名稱
---
**語法解釋:**
1. **項目設置和版本控制：**
  - `cmake_minimum_required(VERSION x.x)`：指定CMake的最小版本需求。
  - `project({PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)`:${PROJECT_NAME}本CMakeLists.txt的project名稱，VERSION x.x 此專案是第幾個版本，LANGUAGES:編譯用什麼語言。
2. **編譯器和標準設置：**
  - `set(CMAKE_CXX_STANDARD 17)` 和 `set(CMAKE_CXX_STANDARD_REQUIRED ON)`:確保使用C++17標準。
  - `add_compile_options(-Wno-unknown-pragmas)`: 用於降低特定編譯警告。
3. **自動代碼生成(QT相關)：**
  - `set(CMAKE_AUTOUIC ON)`:啟用自動UIC處理(QT)。
  - `set(CMAKE_AUTOMOC ON)`:啟用自動MOC處理(QT)。
  - `set(CMAKE_AUTORCC ON)`：啟用自動RCC處理(QT)。
4. **庫和包管理：**
  - `set(CMAKE_PREFIX_PATH "")`:自定義Cmake套件位置。
  - `find_package(Qt6 REQUIRED COMPONENTS Widgets Core Qml Gui)`:用於定位和配置Qt6必要的組件，如Widgets、Core、Qml、Gui，以支持圖形界面開發和應用程序邏輯。
  - `link_directories(目錄列表)`:為鏈接器添加庫文件搜索路徑。`目錄列表`中指定的每個目錄都會被添加到鏈接器的庫文件搜索路徑中，這有助於鏈接器找到需要的庫文件。
5. **源代碼和目標管理：**
  - `aux_source_directory(路徑 變數)`:獲取路徑下所有的.cpp/.c/.cc檔案，並賦值給變數中
  - `add_subdirectory()`:添加子目錄。
  - `add_executable(目標名稱 源文件列表)`:創建一個執行檔目標。`目標名稱`是生成的執行檔名，`源文件列表`是一個或多個源代碼文件，這些源文件將被編譯並鏈接以生成指定的執行檔。
  - `target_link_libraries(目標名稱 PRIVATE/PUBLIC/INTERFACE 库名稱列表)`:為指定的目標（通常是執行檔或者庫）鏈接其他庫。`目標名稱`是需要鏈接庫的目標，`PRIVATE/PUBLIC/INTERFACE`指定了鏈接的範圍和繼承規則，`庫名稱列表`是一個或多個目標需要鏈接的庫。
  - `add_library(庫名稱 STATIC/SHARED/MODULE 源文件列表)`:創建一個庫目標。`庫名稱`是生成的庫的名字，`STATIC/SHARED/MODULE`指定了庫的類型，`源文件列表`是一個或多個源代碼文件，這些源文件將被編譯並鏈接以生成指定的庫。
6. **安裝和路徑管理：**
  - `install(TARGETS 目標名稱 DESTINATION 安裝路徑)`:指定安裝規則，用於將目標（執行檔、庫等）安裝到指定的路徑。`目標名稱`是需要被安裝的目標，`DESTINATION`指定了目標的安裝路徑。
  - `include_directories(目錄列表)`:為編譯器添加頭文件搜索路徑。`目錄列表`中指定的每個目錄都會被添加到編譯器的頭文件搜索路徑中，這對於解決頭文件包含問題非常有用。
7. **變數和配置選項：**
  - `set(變數名 值)`:設置一個變數。`變數名`是要設置的變數的名字，`值`是分配給該變數的值。這個指令常用於自定義一些配置選項或者存儲中間結果。
  - `option(選項名 "描述" 值)`:定義一個開關選項。`選項名`是選項的名字，`描述`提供了這個選項的簡短描述，`值`是這個選項的初始值（ON或OFF）。這對於提供可配置的構建選項非常有用。
8. **訊息和調試：**
  - `message(狀態信息)`:在執行CMake配置過程中打印消息。`狀態信息`是要打印的文本，這對於調試CMake腳本或向用戶提供構建過程的信息非常有用。

### 執行順序
```cmake
# 1. 項目和最小版本要求
cmake_minimum_required(VERSION 3.10) # 指定CMake的最小版本
project(MyProject VERSION 1.0 LANGUAGES CXX) # 定義項目名稱、版本和使用的語言

# 2. 預設設定和標準
set(CMAKE_CXX_STANDARD 17) # 設置C++標準為C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON) # 確保使用指定的標準

# 3. 尋找依賴和包管理
find_package(Qt5 COMPONENTS Widgets REQUIRED) # 尋找Qt5包，指定Widgets組件

# 如果有特定的庫文件或頭文件目錄需要包含，可以使用以下指令：
include_directories(${YOUR_INCLUDE_DIR}) # 添加頭文件搜索路徑
link_directories(${YOUR_LIBRARY_DIR}) # 添加庫文件搜索路徑

# 4. 源代碼和目標定義
# 如果源文件很多，可以使用file(GLOB ...)來匹配所有相關源文件
file(GLOB MY_SOURCES "src/*.cpp") # 匹配src目錄下所有的cpp文件
add_executable(MyExecutable ${MY_SOURCES}) # 創建一個執行檔目標

# 5. 目標鏈接庫
# 根據需要鏈接到相應的庫。如果使用了Qt，需要鏈接到相應的Qt模塊。
target_link_libraries(MyExecutable Qt5::Widgets) # 鏈接Qt5的Widgets模塊到你的執行檔

# 6. 安裝規則（可選）
# 指定安裝目標和目標文件的安裝路徑
install(TARGETS MyExecutable DESTINATION bin) # 將執行檔安裝到bin目錄

# 7. 其他設置
# 設置自定義變數或開啟/關閉選項
option(MY_OPTION "Description of my option" ON) # 定義一個選項

# 8. 訊息打印（用於調試或信息展示）
message(STATUS "Configuring Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
```

## 2. 與qmake對應語法

**定義項目和版本**
- **CMake:**
```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject VERSION 1.0 LANGUAGES CXX)
```
- **qmake:**
```qmake
TEMPLATE = app
TARGET = MyProject
VERSION = 1.0
```

**設定C++標準**
- **CMake:**
```cmake
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
```
- **qmake:**
```qmake
CONFIG += c++17
```
**尋找和配置包**
- **CMake:**
```cmake
find_package(Qt5 COMPONENTS Widgets REQUIRED)
```
- **qmake:**
```qmake
QT += widgets
```
**指定源代碼文件**
- **CMake:**
```cmake
file(GLOB MY_SOURCES "src/*.cpp")
add_executable(MyExecutable ${MY_SOURCES})
```
- **qmake:**
```qmake
SOURCES += src/*.cpp
```
**鏈接庫**
- **CMake:**
```cmake
```
- **qmake:**
```qmake
```
**安裝目標**
- **CMake:**
```cmake
```
- **qmake:**
```qmake
```
**添加頭文件目錄**
- **CMake:**
```cmake
```
- **qmake:**
```qmake
```