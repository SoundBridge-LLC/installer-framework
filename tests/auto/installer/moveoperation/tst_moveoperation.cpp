/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
*/
#include "../shared/packagemanager.h"

#include <updateoperations.h>
#include <packagemanagercore.h>

#include <QTest>

using namespace KDUpdater;
using namespace QInstaller;

class tst_moveoperation : public QObject
{
    Q_OBJECT

private:
    void installFromCLI(const QString &repository)
    {
        QScopedPointer<PackageManagerCore> core(PackageManager::getPackageManagerWithInit
                (m_testDirectory, repository));
        core->installDefaultComponentsSilently();

        QFile movedFile(m_testDirectory + QDir::separator() + "DestinationFolder/testFile.txt");
        QVERIFY(movedFile.exists());
        QFile originalFile(m_sourceFile);
        QVERIFY(!originalFile.exists());

        core->setPackageManager();
        core->commitSessionOperations();

        core->uninstallComponentsSilently(QStringList() << "A");

        QVERIFY(!movedFile.exists());
        QVERIFY(originalFile.exists());
    }

private slots:
    void initTestCase()
    {
        m_testDirectory = QInstaller::generateTemporaryFileName();
        QVERIFY(QDir().mkpath(m_testDirectory));
        QVERIFY(QDir(m_testDirectory).exists());

        QString testFile = QDir::toNativeSeparators("/testFile.txt");
        m_sourceFile = qApp->applicationDirPath() + testFile;
        QFile file(m_sourceFile);
        QVERIFY(file.open(QIODevice::WriteOnly)); //Generates the m_sourceFile
        file.close();

        m_destinationDirectory = m_testDirectory + QDir::toNativeSeparators("/test");
        QVERIFY(QDir().mkpath(m_destinationDirectory));
        m_destinationFile = m_destinationDirectory + QDir::toNativeSeparators(testFile);
    }

    void testMissingArguments()
    {
        MoveOperation op;

        QVERIFY(op.testOperation());
        QVERIFY(!op.performOperation());

        QCOMPARE(UpdateOperation::Error(op.error()), UpdateOperation::InvalidArguments);
        QCOMPARE(op.errorString(), QString("Invalid arguments in Move: 0 arguments given, 2 to 4 arguments "
            "expected in the form: <complete source file name> <complete destination file name> [UNDOOPERATION, \"\"]."));
    }

    void testMoveFile()
    {
        MoveOperation op;
        op.setArguments(QStringList() << m_sourceFile << m_destinationFile);
        op.backup();
        QVERIFY2(op.performOperation(), op.errorString().toLatin1());
        QVERIFY(!QFile::exists(m_sourceFile));
        QVERIFY(QFile::exists(m_destinationFile));

        QVERIFY(op.undoOperation());
        QVERIFY(QFile::exists(m_sourceFile));
        QVERIFY(!QFile::exists(m_destinationFile));
    }

    void testMoveFileDestinationExists()
    {
        QFile file(m_destinationFile);
        QVERIFY(file.open(QIODevice::WriteOnly)); //Creates the destination file
        file.close();

        MoveOperation op;
        op.setArguments(QStringList() << m_sourceFile << m_destinationFile);
        op.backup();
        QVERIFY2(op.performOperation(), op.errorString().toLatin1());
        QVERIFY(!QFile::exists(m_sourceFile));
        QVERIFY(QFile::exists(m_destinationFile));

        QVERIFY(op.undoOperation());
        QVERIFY(QFile::exists(m_sourceFile));
        QVERIFY(QFile::exists(m_destinationFile));
    }

    void testMoveOperationFromScript()
    {
        installFromCLI(":///data/repository");
    }

    void testMoveOperationFromXML()
    {
        installFromCLI(":///data/xmloperationrepository");
    }

    void cleanupTestCase()
    {
        QDir dir(m_testDirectory);
        QVERIFY(dir.removeRecursively());
        QVERIFY(QFile::remove(m_sourceFile));
    }

private:
    QString m_testDirectory;
    QString m_sourceFile;
    QString m_destinationDirectory;
    QString m_destinationFile;
};

QTEST_MAIN(tst_moveoperation)

#include "tst_moveoperation.moc"
