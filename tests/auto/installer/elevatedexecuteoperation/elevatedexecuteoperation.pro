# Copyright (C) 2026 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

include(../../qttest.pri)

QT -= gui
QT += testlib qml

SOURCES = tst_elevatedexecuteoperation.cpp

DEFINES += "QMAKE_BINARY=$$fromNativeSeparators($$QMAKE_BINARY)"
