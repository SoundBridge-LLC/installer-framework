# Copyright (C) 2026 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

include(../../qttest.pri)

QT -= gui
QT += testlib

SOURCES = tst_copyoperationtest.cpp

RESOURCES += \
    settings.qrc \
    ../shared/config.qrc
