# #####################################################################
# Automatically generated by qmake (2.01a) ?? ???. 13 01:22:29 2009
# #####################################################################
TEMPLATE = lib
TARGET = sakdgui
DEPENDPATH += .
INCLUDEPATH += ../include \
    ../../include \
    .
DESTDIR = ../../../lib
LIBS += -L../../../lib \
    -lsawidgetslib

# Input
HEADERS += ../include/sakalendar.h \
    ../include/sakalendarsettingsdialog.h
SOURCES += sakalendar.cpp \
    sakalendarsettingsdialog.cpp
FORMS += ui/sakalendar.ui \
    ui/sakalendarsettingsdialog.ui
UI_HEADERS_DIR = ../include/ui
UI_SOURCES_DIR = ui
RESOURCES += ../sakalendarapp.qrc
QMAKE_CLEAN = ../../../lib/lib$${TARGET}.*
unix {
    OBJECTS_DIR = .obj
    MOC_DIR = .moc
    RCC_DIR = .rcc
    target.path += $$(SAKALENDAR_INSTALL_PREFIX_UNIX)/lib
    INSTALLS = target
} else:win32 {
    target.path += $$(SAKALENDAR_INSTALL_PREFIX_WIN)
    INSTALLS = target
    QMAKE_CLEAN = ../../../lib/$${TARGET}
}
