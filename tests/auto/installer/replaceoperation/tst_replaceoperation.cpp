/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
*/
#include "../shared/packagemanager.h"

#include <fileutils.h>
#include <replaceoperation.h>
#include <packagemanagercore.h>

#include <QDir>
#include <QFile>
#include <QTest>
#include <QRandomGenerator>

using namespace KDUpdater;
using namespace QInstaller;

class tst_replaceoperation : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        m_testDirectory = QInstaller::generateTemporaryFileName();
        m_testFilePath = m_testDirectory + "/test." + QString::number(QRandomGenerator::global()->generate() % 1000);
    }

    void testWrongArguments()
    {
        ReplaceOperation missingArgumentsOperation(nullptr);
        missingArgumentsOperation.setArguments(QStringList() << "testFile" << "testSearch");

        // should do nothing if there are missing arguments
        QVERIFY(missingArgumentsOperation.testOperation());
        QCOMPARE(missingArgumentsOperation.performOperation(), false);

        ReplaceOperation invalidModeArgumentOperation(nullptr);
        invalidModeArgumentOperation.setArguments(QStringList() << "testFile"
            << "testSearch" << "testReplace" << "invalid");

        // should do nothing if there is an invalid mode argument
        QVERIFY(invalidModeArgumentOperation.testOperation());
        QCOMPARE(invalidModeArgumentOperation.performOperation(), false);

        QCOMPARE(UpdateOperation::Error(invalidModeArgumentOperation.error()),
            UpdateOperation::InvalidArguments);

        QString compareString = "Current mode argument calling \"Replace\" with "
            "arguments \"testFile; testSearch; testReplace; invalid\" is not supported. "
            "Please use string or regex.";

        QCOMPARE(invalidModeArgumentOperation.errorString(), compareString);

        ReplaceOperation emptySearchArgumentOperation(nullptr);
        emptySearchArgumentOperation.setArguments(QStringList() << "testFile"
            << "" << "testReplace" << "regex");

        // should do nothing if there is an empty search argument
        QVERIFY(emptySearchArgumentOperation.testOperation());
        QCOMPARE(emptySearchArgumentOperation.performOperation(), false);

        QCOMPARE(UpdateOperation::Error(emptySearchArgumentOperation.error()),
            UpdateOperation::InvalidArguments);

        compareString = "Current search argument calling \"Replace\" with "
            "empty search argument is not supported.";

        QCOMPARE(emptySearchArgumentOperation.errorString(), compareString);
    }

    void testStringSearchReplace()
    {
        QVERIFY(QDir().mkpath(m_testDirectory));
        QVERIFY(QDir(m_testDirectory).exists());

        QFile file(m_testFilePath);
        QVERIFY(file.open(QIODevice::WriteOnly));

        QTextStream stream(&file);
        stream << "Lorem ipsum dolore sit amet, consectetur adipiscing elit, sed do eiusmod "
                  "tempor incididunt ut labore et dolore magna aliqua." << Qt::endl;
        file.close();

        ReplaceOperation searchReplaceOperation(nullptr);
        searchReplaceOperation.setArguments(QStringList() << m_testFilePath
            << "dolore" << "test");

        // should succeed
        QVERIFY(searchReplaceOperation.testOperation());
        QCOMPARE(searchReplaceOperation.performOperation(), true);

        QVERIFY(file.open(QIODevice::ReadOnly));

        QString fileContent = stream.readAll();
        QCOMPARE(fileContent, QLatin1String(
            "Lorem ipsum test sit amet, consectetur adipiscing elit, sed do eiusmod "
            "tempor incididunt ut labore et test magna aliqua.\n"));

        file.close();
        QVERIFY(file.remove());
        QVERIFY(QDir().rmdir(m_testDirectory));
    }

    void testRegexSearchReplace()
    {
        // Test with three different regexes, one containing
        // a capturing group

        QVERIFY(QDir().mkpath(m_testDirectory));
        QVERIFY(QDir(m_testDirectory).exists());

        QFile file(m_testFilePath);
        QVERIFY(file.open(QIODevice::WriteOnly));

        QTextStream stream(&file);
        stream << "one | 10/10/2010 | three | 1.2345 | 0.00001 "
                  "| 7 | A <i>bon mot</i>." << Qt::endl;
        file.close();

        ReplaceOperation searchReplaceOperation(nullptr);
        searchReplaceOperation.setArguments(QStringList() << m_testFilePath
            << "\\d{1,2}/\\d{1,2}/\\d{4}" << "date-match" << "regex");

        // should succeed
        QVERIFY(searchReplaceOperation.testOperation());
        QCOMPARE(searchReplaceOperation.performOperation(), true);

        searchReplaceOperation.setArguments(QStringList() << m_testFilePath
            << "[-+]?[0-9]*\\.?[0-9]+" << "number-match" << "regex");

        // should succeed
        QVERIFY(searchReplaceOperation.testOperation());
        QCOMPARE(searchReplaceOperation.performOperation(), true);

        searchReplaceOperation.setArguments(QStringList() << m_testFilePath
            << "<i>([^<]*)</i>" << "\\emph{\\1}" << "regex");

        // should succeed
        QVERIFY(searchReplaceOperation.testOperation());
        QCOMPARE(searchReplaceOperation.performOperation(), true);

        QVERIFY(file.open(QIODevice::ReadOnly));

        QString fileContent = stream.readAll();
        QCOMPARE(fileContent, QLatin1String("one | date-match | three "
            "| number-match | number-match | number-match | A \\emph{bon mot}.\n"));

        file.close();
        QVERIFY(file.remove());
        QVERIFY(QDir().rmdir(m_testDirectory));
    }

    void testPerformingFromCLI()
    {
        QVERIFY(QDir().mkpath(m_testDirectory));
        PackageManagerCore *core = PackageManager::getPackageManagerWithInit
                (m_testDirectory, ":///data/repository");

        core->installDefaultComponentsSilently();

        QFile file(m_testDirectory + QDir::separator() + "A.txt");
        QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));
        QTextStream stream(&file);
        QCOMPARE(stream.readLine(), QLatin1String("text to replace."));
        QCOMPARE(stream.readLine(), QLatin1String("Another text."));
        file.close();
        QDir dir(m_testDirectory);
        QVERIFY(dir.removeRecursively());
        core->deleteLater();
    }

private:
    QString m_testDirectory;
    QString m_testFilePath;
};

QTEST_MAIN(tst_replaceoperation)

#include "tst_replaceoperation.moc"
