TEMPLATE = lib
TARGET = chess_plugin
QT += qml quick xml
CONFIG += qt plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)
uri = com.vividcode.Chess

# Input
SOURCES += \
    plugin_plugin.cpp \
    chessboardmodel.cpp \
    piece.cpp \
    pawn.cpp \
    queen.cpp \
    rook.cpp \
    bishop.cpp \
    knight.cpp \
    king.cpp \
    recorder.cpp \
    recorderwriter.cpp \
    recorderreader.cpp \
    createaction.cpp \
    moveaction.cpp

HEADERS += \
    plugin_plugin.h \
    chessboardmodel.h \
    piece.h \
    pawn.h \
    queen.h \
    rook.h \
    bishop.h \
    knight.h \
    king.h \
    recorder.h \
    recorderwriter.h \
    recorderreader.h \
    createaction.h \
    moveaction.h

DISTFILES = qmldir \
    THANKS \
    images/games-chess-bishop-black.png \
    images/games-chess-bishop-white.png \
    images/games-chess-king-black.png \
    images/games-chess-king-white.png \
    images/games-chess-knight-black.png \
    images/games-chess-knight-white.png \
    images/games-chess-pawn-black.png \
    images/games-chess-pawn-white.png \
    images/games-chess-queen-black.png \
    images/games-chess-queen-white.png \
    images/games-chess-rook-black.png \
    images/games-chess-rook-white.png

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir
unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}

installPath = ../imports/$$replace(uri, \\., /)
qmldir.path = $$installPath
target.path = $$installPath

INSTALLS += target qmldir

RESOURCES += \
    plugin.qrc
