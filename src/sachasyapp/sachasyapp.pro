# #####################################################################
# Automatically generated by qmake (2.01a) ?? ???. 15 04:45:37 2009
# #####################################################################
unix:EXTENTION=.bin

TEMPLATE = app
TARGET = sachasy$${EXTENTION}
DEPENDPATH += .
INCLUDEPATH += ../include \
    .
DESTDIR = ../../bin
LIBS += -L../../lib \
    -lsatimelib_c \
    -lsatimelib_cpp \
    -lsawidgetslib \
    -lsachasylib

# Input
HEADERS += sachasyapp.h \
    sachasyappsettingsdialog.h
FORMS +=
SOURCES += main.cpp \
    sachasyapp.cpp \
    sachasyappsettingsdialog.cpp
RESOURCES += sachasyapp.qrc
QMAKE_CLEAN = ../../bin/$${TARGET}
unix {
    OBJECTS_DIR = .obj
    MOC_DIR = .moc
    RCC_DIR = .rcc
    target.path += $$(SAKALENDAR_INSTALL_PREFIX_UNIX)/bin
    scripts.files = sachasy
    scripts.path = $$(SAKALENDAR_INSTALL_PREFIX_UNIX)/bin
    icons.files = icons/sachasy.png
    icons.path = $$(SAKALENDAR_INSTALL_PREFIX_UNIX)/share/icons
    INSTALLS = target \
        scripts \
        icons
} else:win32 {
    RC_FILE = sachasyapp.rc
    target.path += $$(SAKALENDAR_INSTALL_PREFIX_WIN)
    INSTALLS = target
}

OTHER_FILES += \
    sachasy
