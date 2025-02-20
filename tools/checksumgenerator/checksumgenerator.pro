TEMPLATE = app
INCLUDEPATH += . ..
TARGET = checksumgenerator

include(../../installerfw.pri)

QT -= gui
CONFIG += console
CONFIG -= import_plugins

SOURCES += main.cpp
