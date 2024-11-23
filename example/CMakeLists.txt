cmake_minimum_required(VERSION 3.19)

project(PitikappExamplePlugin LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Qml Widgets)

qt_standard_project_setup()

set(CMAKE_AUTORCC ON)

qt_add_library(PitikappExamplePlugin
    ModuleCounter/ModuleCounter.cpp ModuleCounter/ModuleCounter.h
    ModuleCustomCounter/ModuleCustomCounter.cpp ModuleCustomCounter/ModuleCustomCounter.h
    ModuleCustomCounter/ModuleCustomCounterData.cpp ModuleCustomCounter/ModuleCustomCounterData.h
    ModuleDiskUsage/ModuleDiskUsage.cpp ModuleDiskUsage/ModuleDiskUsage.h
    ModuleProcessRunTime/ModuleProcessRunTime.cpp ModuleProcessRunTime/ModuleProcessRunTime.h
    ModuleProcessRunTime/ModuleProcessRunTimeData.cpp ModuleProcessRunTime/ModuleProcessRunTimeData.h
    ModuleProcessRunTime/ProcessSelectionHelper.cpp ModuleProcessRunTime/ProcessSelectionHelper.h
    PitikappExamplePlugin.cpp PitikappExamplePlugin.h
)

set_property(TARGET PitikappExamplePlugin APPEND PROPERTY SOURCES resources.qrc)

# The application always expect a file called "plugin.dll".
set_property(TARGET PitikappExamplePlugin PROPERTY OUTPUT_NAME "plugin")

# The plugin.dll file must be inside a folder, that will be copied into your Pitikapp installation directory.
# By default, it would be: C:/Users/UserName/AppData/Local/Pitikapp Remote Dashboard/plugins/${PLUGIN_NAME}
set_target_properties(PitikappExamplePlugin PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/pitikapp_example_plugin)

target_include_directories(PitikappExamplePlugin PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../lib)
target_link_directories(PitikappExamplePlugin PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../lib)

target_link_libraries(PitikappExamplePlugin PRIVATE
    Qt::Core
    Qt::Qml
    Qt::Widgets
    User32
    pitikappPlugin
)
