/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
*/

#include "init.h"
#include "updateoperations.h"

#include <QDir>
#include <QObject>
#include <QTest>
#include <QFile>
#include <QTextStream>

using namespace KDUpdater;
using namespace QInstaller;

class tst_rmdiroperationtest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
       QInstaller::init();
       QString path = QDir::current().path() + QDir::toNativeSeparators("/test");
       if (QDir(path).exists()) {
           QDir dir(path);
           QVERIFY(dir.removeRecursively());
       }
    }

    void testMissingArguments()
    {
        RmdirOperation op;

        QVERIFY(op.testOperation());
        QVERIFY(!op.performOperation());

        QCOMPARE(UpdateOperation::Error(op.error()), UpdateOperation::InvalidArguments);
        QCOMPARE(op.errorString(), QLatin1String("Invalid arguments in Rmdir: 0 arguments given, 1 to 3 "
            "arguments expected in the form: <file to remove> [UNDOOPERATION, \"\"]."));
    }

    void testRemoveDirectory_data()
    {
        QTest::addColumn<QString>("directory");
        QTest::addColumn<bool>("overrideUndo");
        QTest::newRow("/test") << "/test" << false;
        QTest::newRow("/test/test") << "/test/test" << false;
        QTest::newRow("/test/test/test") << "/test/test/test" << false;
        QTest::newRow("no undo") << "/test/test/test/test" << true;
    }

    void testRemoveDirectory()
    {
        QFETCH(QString, directory);
        QFETCH(bool, overrideUndo);

        QString path = QDir::current().path() + QDir::toNativeSeparators(directory);
        //Create first the directories utilizing MkdirOperation
        MkdirOperation op;
        op.setArguments(QStringList() << path);
        op.setArguments(op.arguments() << QLatin1String("UNDOOPERATION"));
        op.backup();
        QVERIFY2(op.performOperation(), op.errorString().toLatin1());
        QVERIFY2(QDir(path).exists(), path.toLatin1());

        RmdirOperation rmOp;
        rmOp.setArguments(QStringList() << path);
        if (overrideUndo)
            rmOp.setArguments(op.arguments() << QLatin1String("UNDOOPERATION"));
        rmOp.backup();
        rmOp.performOperation();
        QVERIFY2(!QDir(path).exists(), path.toLatin1());

        QVERIFY2(rmOp.undoOperation(), rmOp.errorString().toLatin1());
        if (overrideUndo) {
            QVERIFY2(!QDir(path).exists(), path.toLatin1());
        } else {
            QVERIFY2(QDir(path).exists(), path.toLatin1());
            QVERIFY(QDir(path).removeRecursively());
        }
    }
};

QTEST_MAIN(tst_rmdiroperationtest)

#include "tst_rmdiroperationtest.moc"
