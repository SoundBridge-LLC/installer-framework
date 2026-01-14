/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
*/

#include <packagemanagercore.h>
#include <elevatedexecuteoperation.h>

#include <QTest>

#define QUOTE_(x) #x
#define QUOTE(x) QUOTE_(x)

using namespace QInstaller;

class tst_elevatedexecuteoperation : public QObject
{
    Q_OBJECT

private slots:
    void testExecuteOperation()
    {
        m_core.setValue(QLatin1String("QMAKE_BINARY"), QUOTE(QMAKE_BINARY));
        m_core.setValue(QLatin1String("QMAKE_BINARY_OLD"), QLatin1String("FAKE_QMAKE"));
        ElevatedExecuteOperation operation(&m_core);
        operation.setArguments(QStringList() << QLatin1String("UNDOEXECUTE") << QLatin1String("FAKE_QMAKE") << QLatin1String("-v"));

        QTest::ignoreMessage(QtDebugMsg, "\"FAKE_QMAKE\" started, arguments: \"-v\"");
        QString message =  "Failed to run undo operation \"Execute\" for component . Trying again with arguments %1, -v";
        QTest::ignoreMessage(QtDebugMsg, qPrintable(message.arg(QUOTE(QMAKE_BINARY))));
        message =  "\"%1\" started, arguments: \"-v\"";
        QTest::ignoreMessage(QtDebugMsg, qPrintable(message.arg(QUOTE(QMAKE_BINARY))));

        QCOMPARE(operation.undoOperation(), true);
        QCOMPARE(Operation::Error(operation.error()), Operation::NoError);
    }

private:
    PackageManagerCore m_core;
};

QTEST_MAIN(tst_elevatedexecuteoperation)

#include "tst_elevatedexecuteoperation.moc"
