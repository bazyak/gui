QT += qml quick sql widgets

macx: {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15
}

CONFIG += c++17

SOURCES += \
        all_tasks_window.cc \
        main.cc \
        progress_delegate.cc \
        task_service.cc \
        tasks_table_model.cc

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
    all_tasks_window.h \
    progress_delegate.h \
    task_service.h \
    tasks_table_model.h

FORMS += \
    all_tasks_window.ui
