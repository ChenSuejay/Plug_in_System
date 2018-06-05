include(../HoneyBee.pri)

TEMPLATE = lib
TARGET = ExtensionSystem
DESTDIR = $$IDE_LIBRARY_PATH
DEFINES += EXTENSIONSYSTEM_LIBRARY
SOURCES += \
    iplugin.cpp \
    pluginmanager.cpp \
    pluginspec.cpp

HEADERS += \
    iplugin.h \
    pluginmanager.h \
    pluginspec.h \
    pluginspec_p.h \
    externsionsystem_global.h

