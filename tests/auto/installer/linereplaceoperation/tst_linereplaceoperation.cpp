/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
*/
#include "../shared/packagemanager.h"

#include <linereplaceoperation.h>
#include <packagemanagercore.h>

#include <QFile>
#include <QTest>

using namespace KDUpdater;
using namespace QInstaller;

class tst_linereplaceoperation : public QObject
{
    Q_OBJECT

private:
    void installFromCLI(const QString &repository)
    {
        QString installDir = QInstaller::generateTemporaryFileName();
        QVERIFY(QDir().mkpath(installDir));
        QScopedPointer<PackageManagerCore> core(PackageManager::getPackageManagerWithInit
                (installDir, repository));
        core->installDefaultComponentsSilently();

        QFile file(installDir + QDir::separator() + "A.txt");
        QVERIFY(file.open(QIODevice::ReadOnly) | QIODevice::Text);
        QTextStream stream(&file);
        QCOMPARE(stream.readLine(), QLatin1String("This line was replaced."));
        QCOMPARE(stream.readLine(), QLatin1String("Another line."));
        file.close();
        QDir dir(installDir);
        QVERIFY(dir.removeRecursively());
    }

private slots:
    void initTestCase()
    {
        m_testFilePath = qApp->applicationDirPath() + QDir::toNativeSeparators("/test");
    }

    void testMissingArguments()
    {
        LineReplaceOperation op(nullptr);

        op.backup();

        QVERIFY(op.testOperation());
        QVERIFY(!op.performOperation());

        QCOMPARE(UpdateOperation::Error(op.error()), UpdateOperation::InvalidArguments);
        QCOMPARE(op.errorString(), QString("Invalid arguments in LineReplace: "
                                           "0 arguments given, exactly 3 arguments expected."));

        op.setArguments(QStringList() << m_testFilePath << "" << "replace");
        QVERIFY(!op.performOperation());

        QCOMPARE(UpdateOperation::Error(op.error()), UpdateOperation::InvalidArguments);
        QCOMPARE(op.errorString(), QString("Invalid argument in LineReplace: "
                                           "Empty search argument is not supported."));

        op.setArguments(QStringList() << "" << "search" << "replace");
        QTest::ignoreMessage(QtWarningMsg, "QFSFileEngine::open: No file name specified");
        QVERIFY(!op.performOperation());

        QCOMPARE(UpdateOperation::Error(op.error()), UpdateOperation::UserDefinedError);
        QCOMPARE(op.errorString(), QString("Cannot open file \"\" for reading: No file name specified"));
    }

    void testSearchReplace_data()
    {
        QTest::addColumn<QString>("source");
        QTest::addColumn<QString>("search");
        QTest::addColumn<QString>("replace");
        QTest::addColumn<QString>("expected");
        QTest::newRow("Lorem ipsum") << "Lorem ipsum dolore sit amet, consectetur adipiscing elit,\n"
            "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua." << "Lorem" << "Replaced"
            << "Replaced\nsed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n";
        QTest::newRow(".ini syntax") << "[section]\na=x\nb=y" << "a=" << "a=y" << "[section]\na=y\nb=y\n";
        QTest::newRow(".xml syntax") << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<Test>\n<Tag></Tag>\n"
            "</Test>" << "<Tag></Tag>" << "<Tag>el</Tag>" << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<Test>\n<Tag>el</Tag>\n</Test>\n";
    }

    void testSearchReplace()
    {
        QFETCH(QString, source);
        QFETCH(QString, search);
        QFETCH(QString, replace);
        QFETCH(QString, expected);

        QFile file(m_testFilePath);
        QVERIFY(file.open(QIODevice::WriteOnly | QIODevice::Text));

        QTextStream stream(&file);
        stream << source << Qt::endl;
        file.close();

        LineReplaceOperation op(nullptr);
        op.setArguments(QStringList() << m_testFilePath << search << replace);

        QVERIFY2(op.performOperation(), op.errorString().toLatin1());

        QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));
        QCOMPARE(stream.readAll(), expected);

        file.close();
        QVERIFY(file.remove());
    }

    void testLineReplaceFromScript()
    {
        installFromCLI(":///data/repository");
    }

    void testLineReplaceFromXML()
    {
        installFromCLI(":///data/xmloperationrepository");
    }

private:
    QString m_testFilePath;
};

QTEST_MAIN(tst_linereplaceoperation)

#include "tst_linereplaceoperation.moc"
