# #####################################################################
# Automatically generated by qmake (2.01a) ?? ???. 13 01:29:23 2009
# #####################################################################
TEMPLATE = lib
CONFIG += plugin

# static
TARGET = sorokovnikplugin
DEPENDPATH += .
INCLUDEPATH += ../../../../include \
    ../../../include \
    .
DESTDIR = ../../../../../plugins
LIBS += -L../../../../../lib \
    -lsawidgetslib \
    -lsorokovniklib
QT += sql

# Input
HEADERS += sorokovnikplugin.h \
    sorokovnikokno.h
SOURCES += sorokovnikplugin.cpp \
    sorokovnikokno.cpp
QMAKE_CLEAN = ../../../../../plugins/lib$${TARGET}.*
unix {
    OBJECTS_DIR = .obj
    MOC_DIR = .moc
    target.path += $$(SAKALENDAR_INSTALL_PREFIX_UNIX)/plugins
    INSTALLS = target
} else:win32 {
    target.path += $$(SAKALENDAR_INSTALL_PREFIX_WIN)/plugins
    INSTALLS = target
    QMAKE_CLEAN = ../../../../../plugins/$${TARGET}.*
}
