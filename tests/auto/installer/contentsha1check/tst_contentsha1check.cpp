/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
*/
#include "../shared/packagemanager.h"
#include "../shared/verifyinstaller.h"

#include <component.h>
#include <packagemanagercore.h>

#include <QLoggingCategory>
#include <QTest>
#include <QMessageBox>

using namespace QInstaller;

typedef QList<QPair<QString, QString> > ComponentResourceHash;
typedef QPair<QString, QString> ComponentResource;

static QStringList expectedMessages;

void downloadingArchiveOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(type)
    Q_UNUSED(context)
    QByteArray localMsg = msg.toLocal8Bit();
    if (!msg.startsWith("Archive "))
        return;
    if (expectedMessages.contains(msg))
        expectedMessages.removeOne(msg);
}

class tst_ContentSha1Check : public QObject
{
    Q_OBJECT

private slots:

    void testInstall_data()
    {
        QTest::addColumn<QString>("repository");
        QTest::addColumn<QStringList>("installComponents");
        QTest::addColumn<PackageManagerCore::Status>("status");
        QTest::addColumn<ComponentResourceHash>("componentResources");
        QTest::addColumn<QStringList >("installedFiles");
        QTest::addColumn<QStringList >("expectedDownloadingArchiveMessages");

         /*********** Install with checksum check **********/
        ComponentResourceHash componentResources;
        componentResources.append(ComponentResource("A", "1.0.2-1content.txt"));
        componentResources.append(ComponentResource("B", "1.0.0-1content.txt"));

        QTest::newRow("Check checksum")
                << ":///data/repositorywithchecksumcheck"
                << (QStringList() << "A" << "B")
                << PackageManagerCore::Success
                << componentResources
                << (QStringList() << "components.xml" << "A.txt" << "B.txt")
                << (QStringList() << "Archive \"1.0.2-1content.7z\" downloaded for component A."
                                  << "Archive \"1.0.0-1content.7z\" downloaded for component B.");
    }

    void testInstallWithInvalidChecksum_data()
    {
        QTest::addColumn<QString>("repository");
        QTest::addColumn<QStringList>("installComponents");
        QTest::addColumn<PackageManagerCore::Status>("status");
        QTest::addColumn<ComponentResourceHash>("componentResources");
        QTest::addColumn<QStringList >("installedFiles");

        /*********** Install with checksum check **********/
        ComponentResourceHash componentResources;

        QTest::newRow("Invalid checksum")
                << ":///data/repositorywithinvalidchecksum"
                << (QStringList() << "E" << "F")
                << PackageManagerCore::Failure
                << componentResources
                << (QStringList());
    }

    void testInstall()
    {
        QFETCH(QString, repository);
        QFETCH(QStringList, installComponents);
        QFETCH(PackageManagerCore::Status, status);
        QFETCH(ComponentResourceHash, componentResources);
        QFETCH(QStringList, installedFiles);
        QFETCH(QStringList, expectedDownloadingArchiveMessages);

        expectedMessages = expectedDownloadingArchiveMessages;
        QScopedPointer<PackageManagerCore> core(PackageManager::getPackageManagerWithInit
                (m_installDir, repository));
        qInstallMessageHandler(downloadingArchiveOutput);

        QCOMPARE(status, core->installSelectedComponentsSilently(QStringList() << installComponents));
        for (const ComponentResource &resource : componentResources)
            VerifyInstaller::verifyInstallerResources(m_installDir, resource.first, resource.second);
        VerifyInstaller::verifyFileExistence(m_installDir, installedFiles);

        QVERIFY(expectedMessages.isEmpty());
    }

    void testInstallWithInvalidChecksum()
    {
        QFETCH(QString, repository);
        QFETCH(QStringList, installComponents);
        QFETCH(PackageManagerCore::Status, status);
        QFETCH(ComponentResourceHash, componentResources);
        QFETCH(QStringList, installedFiles);

        QScopedPointer<PackageManagerCore> core(PackageManager::getPackageManagerWithInit
                (m_installDir, repository));
        core->setMessageBoxAutomaticAnswer("DownloadError", QMessageBox::Cancel);
        core->setMessageBoxAutomaticAnswer("installationError", QMessageBox::Ok);

        QCOMPARE(status, core->installSelectedComponentsSilently(QStringList() << installComponents));
        QVERIFY(!QDir().exists(m_installDir));
    }

    void init()
    {
        m_installDir = QInstaller::generateTemporaryFileName();
        QVERIFY(QDir().mkpath(m_installDir));
    }

    void cleanup()
    {
        QDir dir(m_installDir);
        QVERIFY(dir.removeRecursively());
    }

private:
    QString m_installDir;
    QStringList m_expectedMessages;
};


QTEST_MAIN(tst_ContentSha1Check)

#include "tst_contentsha1check.moc"
