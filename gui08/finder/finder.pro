QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

macx: {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    finder_widget.cc \
    main.cc \
    main_window.cc \
    seeker.cc \
    user_event.cc

HEADERS += \
    finder_widget.h \
    main_window.h \
    seeker.h \
    user_event.h

FORMS += \
    main_window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
