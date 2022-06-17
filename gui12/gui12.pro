QT += qml quick gui widgets

macx: {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15
}

CONFIG += c++17

SOURCES += \
        html_parser.cc \
        main.cc \
        search_service.cc

resources.files = main.qml 
resources.prefix = /$${TARGET}
RESOURCES += resources \
    resources.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

HEADERS += \
    html_parser.h \
    search_service.h

FORMS +=
