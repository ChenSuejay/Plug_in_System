include(../HoneyBee.pri)
TEMPLATE = app
TARGET = honeyBee
DESTDIR = $$IDE_OUTPUT
SOURCES += main.cpp \
    ip_cfg.cpp

LIBS += -L $$IDE_LIBRARY_PATH/ -lExtensionSystem -llua

HEADERS += \
    ip_cfg.h
