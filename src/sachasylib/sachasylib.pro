# #####################################################################
# Automatically generated by qmake (2.01a) ?? ???. 15 04:45:37 2009
# #####################################################################
TEMPLATE = lib
TARGET = sachasylib
DEPENDPATH += .
INCLUDEPATH += ../include \
    .
DESTDIR = ../../lib
LIBS += -L../../lib \
    -lsatimelib_c \
    -lsatimelib_cpp \
    -lsawidgetslib

# Input
HEADERS += ../include/sachasy.h \
    ../include/sachasysettingsform.h
SOURCES += sachasy.cpp \
    sachasysettingsform.cpp
RESOURCES += sachasylib.qrc
FORMS += ui/sachasysettingsform.ui
UI_HEADERS_DIR = ../include/ui
UI_SOURCES_DIR = ui
QMAKE_CLEAN = ../../lib/lib$${TARGET}.*
unix {
    OBJECTS_DIR = .obj
    MOC_DIR = .moc
    RCC_DIR = .rcc
    target.path = $$(SAKALENDAR_INSTALL_PREFIX_UNIX)/lib
    INSTALLS = target
} else:win32 {
    target.path = $$(SAKALENDAR_INSTALL_PREFIX_WIN)
    INSTALLS = target
    QMAKE_CLEAN = ../../lib/$${TARGET}.*
}
