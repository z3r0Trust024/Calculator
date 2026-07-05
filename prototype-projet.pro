TEMPLATE = app

QT += qml quick
CONFIG += c++17

SOURCES += main.cpp \
    stringprocessorwrapper.cpp \
    calculator.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += stringprocessorwrapper.h \
    calculator.hpp

# Chemins vers les en-têtes de Boost
INCLUDEPATH += C:/boost_1_66_0

# Chemins vers les bibliothèques de Boost
LIBS += -LC:/boost_1_66_0/stage/lib \
        -lboost_regex-mgw49-mt-x32-1_66 
