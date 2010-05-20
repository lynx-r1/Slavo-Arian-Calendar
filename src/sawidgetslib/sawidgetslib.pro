# -------------------------------------------------
# Project created by QtCreator 2010-04-05T12:22:45
# -------------------------------------------------
TARGET = sawidgetslib
TEMPLATE = lib
INCLUDEPATH = ../include \
    .
DESTDIR = ../../lib

SOURCES += sagraphicswindow.cpp \
    sagraphicstitlebarbutton.cpp \
    sagraphicsstackedlayout.cpp \
    sagraphicspushbutton.cpp \
    sagraphicslabelitem.cpp \
    sagraphicsbuttongroup.cpp \
    sacolorbutton.cpp \
    sagraphicstoolbar.cpp
HEADERS += ../include/sagraphicswindow.h \
    ../include/sagraphicstitlebarbutton.h \
    ../include/sagraphicsstackedlayout.h \
    ../include/sagraphicspushbutton.h \
    ../include/sagraphicslabelitem.h \
    ../include/sagraphicsbuttongroup.h \
    ../include/sadefineslib.h \
    ../include/sacolorbutton.h \
    ../include/sagraphicstoolbar.h
QMAKE_CLEAN = ../../lib/lib$${TARGET}.*
unix {
    OBJECTS_DIR = .obj
    MOC_DIR = .moc
    target.path = $$(SAKALENDAR_INSTALL_PREFIX_UNIX)/lib
    INSTALLS = target
} else:win32 {
    target.path = $$(SAKALENDAR_INSTALL_PREFIX_WIN)
    INSTALLS = target
    QMAKE_CLEAN = ../../lib/$${TARGET}.*
}
