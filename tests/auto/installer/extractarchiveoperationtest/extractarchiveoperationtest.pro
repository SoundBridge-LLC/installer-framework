# Copyright (C) 2026 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

include(../../qttest.pri)

QT -= gui
QT += testlib

RESOURCES += data.qrc \
    ../shared/config.qrc
SOURCES = tst_extractarchiveoperationtest.cpp
