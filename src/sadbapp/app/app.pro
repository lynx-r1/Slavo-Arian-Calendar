# #####################################################################
# Automatically generated by qmake (2.01a) ?? ???. 18 19:00:10 2009
# #####################################################################
unix:EXTENTION=.bin

TEMPLATE = app
TARGET = sadb$${EXTENTION}
DEPENDPATH += .
INCLUDEPATH += ../include \
    .
DESTDIR = ../../../bin
LIBS += -L../../../lib \
    -lsawidgetslib \
    -lsadatabase \
    -lsadbgui \
    -lsatimelib_c \
    -lsatimelib_cpp \
    -lsorokovniklib \
    -lsorokovnikdb \
    -ldk4lib \
    -ldk4db
QT += sql

# Input
SOURCES += main.cpp
RESOURCES += ../sadbapp.qrc
QMAKE_CLEAN = ../../../bin/$${TARGET}
unix {
    OBJECTS_DIR = .obj
    MOC_DIR = .moc
    UI_DIR = .ui
    RCC_DIR = .rcc
    target.path += $$(SAKALENDAR_INSTALL_PREFIX_UNIX)/bin
    scripts.files = sadb
    scripts.path = $$(SAKALENDAR_INSTALL_PREFIX_UNIX)/bin
    icons.files = sadb.png
    icons.path = $$(SAKALENDAR_INSTALL_PREFIX_UNIX)/share/icons
    INSTALLS = target \
        scripts \
        icons
} else:win32 {
    RC_FILE = sadbapp.rc
    target.path += $$(SAKALENDAR_INSTALL_PREFIX_WIN)
    INSTALLS = target
}
