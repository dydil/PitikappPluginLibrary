#---------------------------------------------
# This is an example of a plugin for Pitikapp.
#---------------------------------------------

PLUGIN_NAME = PitikappExamplePlugin

# --------------------------------------------
# The project must be a library.
# --------------------------------------------
TEMPLATE = lib

# --------------------------------------------
# widgets required for QFileIconProvider
# --------------------------------------------
QT += widgets qml

# --------------------------------------------
# The name must always be "plugin".
# --------------------------------------------
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

# -----------------------------------------------------------------------------------------
# This include file automatically links to the library and adds the required include paths.
# -----------------------------------------------------------------------------------------
include($$PWD/../lib/PitikappPluginLibrary.pri)

# ---------------------------------------------------------------------------------------
# Point to your Pitikapp installation plugin directory.
# Using a symlink makes it easier.
# By default, it would be:
# DESTDIR=C:/Users/UserName/AppData/Local/Pitikapp Remote Dashboard/plugins/$$PLUGIN_NAME
# -----------------------------------------------------------------------------------------
DESTDIR=$$PWD/$$PLUGIN_NAME

RESOURCES += \
    resources.qrc

# --------------------------------------------
# Lib required for some Windows.h functions.
# --------------------------------------------
LIBS += -lUser32
