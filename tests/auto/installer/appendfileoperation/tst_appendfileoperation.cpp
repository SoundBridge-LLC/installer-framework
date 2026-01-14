/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
*/
#include "../shared/packagemanager.h"

#include <updateoperations.h>
#include <packagemanagercore.h>

#include <QFile>
#include <QTest>

using namespace KDUpdater;
using namespace QInstaller;

class tst_appendfileoperation : public QObject
{
    Q_OBJECT

private:
    void installFromCLI(const QString &repository)
    {
        QString installDir = QInstaller::generateTemporaryFileName();
        QVERIFY(QDir().mkpath(installDir));
        PackageManagerCore *core = PackageManager::getPackageManagerWithInit
                (installDir, repository);
        core->installDefaultComponentsSilently();

        QFile file(installDir + QDir::separator() + "A.txt");
        QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));
        QTextStream stream(&file);
        QCOMPARE(stream.readAll(), QLatin1String("Appended text: lorem ipsum"));
        file.close();

        core->setPackageManager();
        core->commitSessionOperations();
        // We cannot check the file contents here as it will be deleted on
        // undo Extract, but at least check that the uninstallation succeeds.
        QCOMPARE(PackageManagerCore::Success, core->uninstallComponentsSilently
                 (QStringList()<< "A"));

        QDir dir(installDir);
        QVERIFY(dir.removeRecursively());
        core->deleteLater();
    }

private slots:
    void initTestCase()
    {
        m_testFilePath = qApp->applicationDirPath() + QDir::toNativeSeparators("/test");
    }

    void testMissingArguments()
    {
        AppendFileOperation op;

        QVERIFY(op.testOperation());
        QVERIFY(!op.performOperation());

        QCOMPARE(UpdateOperation::Error(op.error()), UpdateOperation::InvalidArguments);
        QCOMPARE(op.errorString(), QString("Invalid arguments in AppendFile: 0 arguments given, 2 to 4 arguments expected in the form: <filename> <text to apply> [UNDOOPERATION, \"\"]."));

        op.setArguments(QStringList() << "" << "");
        QTest::ignoreMessage(QtWarningMsg, "QFSFileEngine::open: No file name specified");
        QTest::ignoreMessage(QtWarningMsg, "QFile::rename: Empty or null file name");
        QVERIFY(!op.performOperation());

        QCOMPARE(UpdateOperation::Error(op.error()), UpdateOperation::UserDefinedError);
        QCOMPARE(op.errorString(), QString("Cannot open file \"\" for writing: No file name specified"));
    }

    void testAppendText_data()
    {
        QTest::addColumn<QString>("source");
        QTest::addColumn<QString>("append");
        QTest::addColumn<QString>("expected");
        QTest::addColumn<bool>("overrideUndo");
        QTest::newRow("newline") << "Line1\nLine2\nLine3\n" << "AppendedText"
            << "Line1\nLine2\nLine3\nAppendedText" << false;
        QTest::newRow("no newline") << "Lorem ipsum " << "dolore sit amet"
            << "Lorem ipsum dolore sit amet" << false;

        QTest::newRow("no undo") << "Lorem ipsum " << "dolore sit amet"
            << "Lorem ipsum dolore sit amet" << true;
    }

    void testAppendText()
    {
        QFETCH(QString, source);
        QFETCH(QString, append);
        QFETCH(QString, expected);
        QFETCH(bool, overrideUndo);

        QFile file(m_testFilePath);
        QVERIFY(file.open(QIODevice::WriteOnly | QIODevice::Text));

        QTextStream stream(&file);
        stream << source << Qt::flush;
        file.close();

        AppendFileOperation *op = new AppendFileOperation();
        op->setArguments(QStringList() << m_testFilePath << append);
        if (overrideUndo)
            op->setArguments(op->arguments() << QLatin1String("UNDOOPERATION"));

        op->backup();
        QVERIFY(QFileInfo(op->value("backupOfFile").toString()).exists());

        QVERIFY2(op->performOperation(), op->errorString().toLatin1());
        QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));
        QCOMPARE(stream.readAll(), expected);
        file.close();

        QVERIFY2(op->undoOperation(), op->errorString().toLatin1());
        QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));
        if (overrideUndo)
            QCOMPARE(stream.readAll(), expected);
        else
            QCOMPARE(stream.readAll(), source);
        QString backupFileName = op->value("backupOfFile").toString();
        delete op;
        QVERIFY(!QFileInfo::exists(backupFileName));

        file.close();

        QVERIFY(file.remove());
    }

    void testApppendFromScript()
    {
        installFromCLI(":///data/repository");
    }

    void testAppendFromComponentXML()
    {
        installFromCLI(":///data/xmloperationrepository");
    }

private:
    QString m_testFilePath;
};

QTEST_MAIN(tst_appendfileoperation)

#include "tst_appendfileoperation.moc"
