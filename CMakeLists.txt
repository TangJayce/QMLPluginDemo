cmake_minimum_required(VERSION 3.14)
project(QMLPlugin)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MODULE_NAME DashboardEditor)
set(OUTPUT_PATH ${PROJECT_SOURCE_DIR}/install)
set(PLUGIN_PATH ${OUTPUT_PATH}/MOZA/${MODULE_NAME})

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick Qml REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick QuickControls2 REQUIRED)

file(GLOB_RECURSE source_files "src/*.cc" "src/*.h")
file(GLOB_RECURSE qml_files qml/*.qml)

add_library(${PROJECT_NAME} SHARED
        ${source_files}
        qml/qmldir
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Quick
        Qt${QT_VERSION_MAJOR}::QuickControls2
        Qt${QT_VERSION_MAJOR}::Qml
)

set(QT5_BIN_PATH ${Qt5_DIR}/../../../bin)
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${PLUGIN_PATH}
)
install(FILES qml/qmldir ${qml_files} DESTINATION ${PLUGIN_PATH})
# 安装完成后执行脚本生成qmltypes
install(CODE "execute_process(
        COMMAND ${QT5_BIN_PATH}/qmlplugindump.exe MOZA.${MODULE_NAME} 1.0 ${OUTPUT_PATH}
        -nonrelocatable -output ${PLUGIN_PATH}/plugins.qmltypes
)")

# install自动生成
add_executable(Main main.cc)

# demo
add_executable(Demo
        example/demo.cc
        example/app/qml.qrc
        ${source_files}
)
target_include_directories(Demo PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(Demo PRIVATE
       Qt${QT_VERSION_MAJOR}::Core
       Qt${QT_VERSION_MAJOR}::Quick
       Qt${QT_VERSION_MAJOR}::QuickControls2
       Qt${QT_VERSION_MAJOR}::Qml
)
