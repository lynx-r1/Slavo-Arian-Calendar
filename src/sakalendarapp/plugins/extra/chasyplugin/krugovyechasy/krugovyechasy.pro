# #####################################################################
# Automatically generated by qmake (2.01a) ?? ???. 15 04:45:37 2009
# #####################################################################
TEMPLATE = app
TARGET = krugovyechasy
DEPENDPATH += .
INCLUDEPATH += ../../../include \
    .
DESTDIR = ../../../../bin
LIBS += -L../../../../libs \
    -lkdtimers \
    -lsakdcore

# Input
HEADERS += krugovyechasy.h
SOURCES += main.cpp \
    krugovyechasy.cpp
unix { 
    OBJECTS_DIR = .obj
    MOC_DIR = .moc
    UI_DIR = .ui
    RCC_DIR = .rcc
}
RESOURCES += ../../../../images/resources.qrc
