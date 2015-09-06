TEMPLATE = app

CONFIG += warn_on qmltestcase
QT += qml quick
CONFIG += c++11

SOURCES += main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = ../imports

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    tst_board.qml \
    tst_boardview.qml

