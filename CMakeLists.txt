cmake_minimum_required(VERSION 3.10)
project(EconomicEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "D:\\Qt\\5.14.1\\msvc2017_64")

find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(EconomicEngine EconomicEngineDebugGUI/main.cpp )
target_link_libraries(EconomicEngine Qt5::Widgets)