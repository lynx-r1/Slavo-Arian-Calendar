# #####################################################################
# Automatically generated by qmake (2.01a) ?? ???. 13 01:29:23 2009
# #####################################################################
TEMPLATE = lib
CONFIG += plugin
TARGET = novoletiyaplugin
DEPENDPATH += .
INCLUDEPATH += ../../include \
    .
DESTDIR = ../../../plugins
LIBS += -L../../../../libs \
    -lkdwidgets

# Input
HEADERS += nachalasorokovnikov/nachalasorokovnikov.h \
    nachalasorokovnikovplugin.h \
    nachalasorokovnikov/nachalasorokovnikovokno.h \
    nachalasorokovnikov/dennedeli.h
SOURCES += nachalasorokovnikov/nachalasorokovnikov.cpp \
    nachalasorokovnikovplugin.cpp \
    nachalasorokovnikov/nachalasorokovnikovokno.cpp \
    nachalasorokovnikov/dennedeli.cpp
unix {
    OBJECTS_DIR = .obj
    MOC_DIR = .moc
    UI_DIR = .ui
    RCC_DIR = .rcc
    INSTALL_PREFIX = /usr/local
    target.path += $$INSTALL_PREFIX/plugins
    INSTALLS = target
    QMAKE_CLEAN = ../../../plugins/libnovoletiyaplugin.*
}
