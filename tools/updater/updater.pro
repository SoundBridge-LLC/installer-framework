TEMPLATE = app
TARGET = maintenanceToolUpdater
INCLUDEPATH += . ..

include(../../installerfw.pri)

QT -= gui

CONFIG -= import_plugins
DESTDIR = $$IFW_APP_PATH

SOURCES += updater.cpp

macos:include(../../no_app_bundle.pri)

target.path = $$[QT_INSTALL_BINS]
INSTALLS += target
