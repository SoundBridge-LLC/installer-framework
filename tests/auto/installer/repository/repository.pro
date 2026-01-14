# Copyright (C) 2026 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

include(../../qttest.pri)

QT += network
QT -= gui

SOURCES += tst_repository.cpp

RESOURCES += \
    settings.qrc \
    ../shared/config.qrc
