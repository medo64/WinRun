APP_PRODUCT = "WinRun"
APP_COMPANY = "Josip Medved"
APP_VERSION = "0.0.0"
APP_COPYRIGHT = "Copyright 2023 Josip Medved <jmedved@jmedved.com>"
APP_DESCRIPTION = "WinRun: Quick run dialog"

DEFINES += "APP_PRODUCT=\"\\\"$$APP_PRODUCT\\\"\""
DEFINES += "APP_COMPANY=\"\\\"$$APP_COMPANY\\\"\""
DEFINES += "APP_VERSION=\\\"$$APP_VERSION\\\""
DEFINES += "APP_COPYRIGHT=\"\\\"$$APP_COPYRIGHT\\\"\""
DEFINES += "APP_DESCRIPTION=\"\\\"$$APP_DESCRIPTION\\\"\""
DEFINES += "APP_QT_VERSION=\\\"$$QT_VERSION\\\""

APP_COMMIT = $$system(git -C \"$$_PRO_FILE_PWD_\" log -n 1 --format=%h)
APP_COMMIT_DIFF = $$system(git -C \"$$_PRO_FILE_PWD_\" diff)
!isEmpty(APP_COMMIT_DIFF) {
    APP_COMMIT = $$upper($$APP_COMMIT) #upper-case if working directory is dirty
}
DEFINES += "APP_COMMIT=\"\\\"$$APP_COMMIT\\\"\""


QT       += core gui widgets
QT       += network  # QLocalServer/QLocalSocket
QT       += printsupport

unix {
    QT  += x11extras
    LIBS += -lX11 -lxcb
}

win32 {
    QMAKE_TARGET_PRODUCT = $$APP_PRODUCT
    QMAKE_TARGET_COMPANY = $$APP_COMPANY
    QMAKE_TARGET_COPYRIGHT = $$APP_COPYRIGHT
    QMAKE_TARGET_DESCRIPTION = $$APP_DESCRIPTION
    RC_ICONS = icons/app.ico
    VERSION = $$APP_VERSION + ".0"
}

DEFINES += QT_DEPRECATED_WARNINGS
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

CONFIG += c++11
QMAKE_CXXFLAGS_WARN_ON += -Wall
QMAKE_CXXFLAGS_WARN_ON += -Wextra
QMAKE_CXXFLAGS_WARN_ON += -Wshadow
QMAKE_CXXFLAGS_WARN_ON += -Wdouble-promotion

QMAKE_CXXFLAGS_DEBUG *= -pg
QMAKE_LFLAGS_DEBUG *= -pg


SOURCES += \
        ui/mainwindow.cpp \
        medo/about.cpp \
        medo/appsetupmutex.cpp \
        medo/config.cpp \
        medo/singleinstance.cpp \
        app.cpp \
        execute.cpp \
        settings.cpp \
        state.cpp \
        visuals.cpp

unix {
    SOURCES += \
        medo/dconfHotkey.cpp \
}

win32 {
}

HEADERS += \
        ui/mainwindow.h \
        medo/about.h \
        medo/appsetupmutex.h \
        medo/config.h \
        medo/singleinstance.h \
        execute.h \
        settings.h \
        state.h \
        visuals.h

unix {
    HEADERS += \
        medo/dconfHotkey.h
}

win32 {
}

FORMS += \
        ui/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    winrun.qrc

DISTFILES += \
    .astylerc


TEMPLATE = app
TARGET = winrun
