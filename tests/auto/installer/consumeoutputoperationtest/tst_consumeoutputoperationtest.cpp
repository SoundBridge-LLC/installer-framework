/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
*/
#include "../shared/packagemanager.h"

#include <packagemanagercore.h>
#include <consumeoutputoperation.h>
#include <qinstallerglobal.h>
#include <errors.h>

#include <QTest>
#include <QProcess>

#define QUOTE_(x) #x
#define QUOTE(x) QUOTE_(x)

using namespace KDUpdater;
using namespace QInstaller;

class tst_consumeoutputoperationtest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        // be aware that this would enable eating the qDebug output
        //QInstaller::init();

        m_fakeQtPath =  QDir::toNativeSeparators(qApp->applicationDirPath()) + QDir::separator()
            + "fakeQt" + QDir::separator();
        QVERIFY(QDir().mkpath(m_fakeQtPath + "bin"));
        m_testOutput = getOutputFrom(QUOTE(QMAKE_BINARY), QStringList("-query"));
        qputenv("qmakePath", QUOTE(QMAKE_BINARY)); //Read from script
    }

    void testMissingArguments()
    {
        ConsumeOutputOperation operation(nullptr);

        QVERIFY(operation.testOperation());
        QVERIFY(!operation.performOperation());
        // undo does nothing so this should be true

        QVERIFY(operation.undoOperation());

        QCOMPARE(UpdateOperation::Error(operation.error()), UpdateOperation::InvalidArguments);
        //qDebug() << operation.errorString();
        QString compareString("Invalid arguments in ConsumeOutput: 0 arguments given, at least 2 "
                              "arguments expected in the form: <to be saved installer key name> "
                              "<executable> [argument1] [argument2] [...].");
        //qDebug() << compareString;
        QCOMPARE(operation.errorString(), compareString);
    }

    void testGetOutputFromQmake()
    {
        ConsumeOutputOperation operation(&m_core);

        operation.setArguments(QStringList() << "testConsumeOutputKey" << QUOTE(QMAKE_BINARY) << "-query");
        QVERIFY2(operation.performOperation(), qPrintable(operation.errorString()));
        QCOMPARE(Operation::Error(operation.error()), Operation::NoError);

        QCOMPARE(m_core.value("testConsumeOutputKey"), m_testOutput);
    }

    void testPerformingFromCLI()
    {
        QString installDir = QInstaller::generateTemporaryFileName();
        QVERIFY(QDir().mkpath(installDir));
        PackageManagerCore *core = PackageManager::getPackageManagerWithInit
                (installDir, ":///data/repository");

        core->installDefaultComponentsSilently();
        QCOMPARE(core->value("testConsumeOutputKeyFromScript"), m_testOutput);

        QDir dir(installDir);
        QVERIFY(dir.removeRecursively());
        core->deleteLater();
    }

    void cleanupTestCase()
    {
        try {
            removeDirectory(m_fakeQtPath);
        } catch (const QInstaller::Error &error) {
            QFAIL(qPrintable(error.message()));
        }
        qunsetenv("qmakePath");
    }

private:
    QString getOutputFrom(const QString &binary, const QStringList &arguments = QStringList())
    {
        QEventLoop loop;
        QProcess process;

        QObject::connect(&process, SIGNAL(finished(int,QProcess::ExitStatus)), &loop, SLOT(quit()));
        process.start(binary, arguments, QIODevice::ReadOnly);

        if (process.state() != QProcess::NotRunning)
            loop.exec();

        return process.readAllStandardOutput().trimmed();
    }


    PackageManagerCore m_core;
    QString m_fakeQtPath;
    QString m_testOutput;
};

QTEST_MAIN(tst_consumeoutputoperationtest)

#include "tst_consumeoutputoperationtest.moc"
