#-------------------------------------------------
# This is an example of a plugin for Pitikapp.
#-------------------------------------------------

PLUGIN_NAME = PitikappExamplePlugin

# The project must be a library.
TEMPLATE = lib

# Required for QFileIconProvider
QT += widgets

# The name must always be "plugin".
TARGET = plugin

QMAKE_CXXFLAGS += /std:c++latest

SOURCES += \
        ModuleCounter/ModuleCounter.cpp \
        ModuleProcessRunTime/ModuleProcessRunTime.cpp \
        ModuleProcessRunTime/ModuleProcessRunTimeData.cpp \
        ModuleProcessRunTime/ProcessSelectionHelper.cpp \
        ModuleDiskUsage/ModuleDiskUsage.cpp \
        PitikappExamplePlugin.cpp

HEADERS += \
        ModuleCounter/ModuleCounter.h \
        ModuleProcessRunTime/ModuleProcessRunTime.h \
        ModuleProcessRunTime/ModuleProcessRunTimeData.h \
        ModuleProcessRunTime/ProcessSelectionHelper.h \
        ModuleDiskUsage/ModuleDiskUsage.h \
        PitikappExamplePlugin.h

include($$PWD/../lib/PitikappPluginLibrary.pri)

# Point to your Pitikapp installation plugin directory.
DESTDIR=$$PWD/../../Pitikapp/bin/plugins/$$PLUGIN_NAME

RESOURCES += \
    resources.qrc

LIBS += -lUser32
