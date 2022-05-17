QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

macx: {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000  # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    key_event_filter.cc \
    main.cc \
    main_window.cc

HEADERS += \
    global_consts.h \
    key_event_filter.h \
    main_window.h

FORMS += \
    main_window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

TRANSLATIONS += gui_en.ts
CODECFORSRC = UTF-8

DISTFILES +=
