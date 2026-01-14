/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
*/

#include "../shared/packagemanager.h"
#include "../shared/verifyinstaller.h"

#include <packagemanagercore.h>
#include <errors.h>

#include <QFile>
#include <QTest>

using namespace QInstaller;

class tst_CreateOffline : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        VerifyInstaller::getInstallerBaseBinaryFile(m_installerBase);
    }

    void init()
    {
        // Get new target directory for each test function
        m_targetDir = QInstaller::generateTemporaryFileName();
    }

    void cleanup()
    {
        QDir dir(m_targetDir);
        QVERIFY(dir.removeRecursively());
    }

    void testCreateOfflineInstaller_data()
    {
        QTest::addColumn<QString>("repository");
        QTest::addColumn<QString>("component");
        QTest::addColumn<PackageManagerCore::Status>("expectedStatus");
        QTest::newRow("Both metaformats | License")
            << ":///data/repository-bothmeta-license" << "org.qtproject.ifw.example.licenseagreement"
            << PackageManagerCore::Success;
        QTest::newRow("Both metaformats | UserInterface")
            << ":///data/repository-bothmeta-userinteface" << "or.qtproject.ifw.example.openreadme"
            << PackageManagerCore::Success;
        QTest::newRow("Component metaformat | Script")
            << ":///data/repository-componentmeta-script" << "org.qtproject.ifw.example"
            << PackageManagerCore::Success;
        QTest::newRow("Unified metaformat | Script")
            << ":///data/repository-unifiedmeta-script" << "org.qtproject.ifw.example"
            << PackageManagerCore::Success;
        QTest::newRow("Component metaformat | Empty component meta-archive")
            << ":///data/repository-componentmeta-emptymetafile" << "A"
            << PackageManagerCore::Success;
        QTest::newRow("Non-existing component")
            << ":///data/repository-unifiedmeta-script" << "a.dummy.component"
            << PackageManagerCore::Canceled;
        QTest::newRow("Invalid repository")
            << ":///data/repository-invalid" << "a.dummy.component"
            << PackageManagerCore::Canceled;
    }

    void testCreateOfflineInstaller()
    {
        QFETCH(QString, repository);
        QFETCH(QString, component);
        QFETCH(PackageManagerCore::Status, expectedStatus);

        QScopedPointer<PackageManagerCore> core(
            PackageManager::getPackageManagerWithInit(m_targetDir, repository));
        core->setCommandLineInstance(true);
        core->setOfflineBaseBinary(m_installerBase);
        core->setOfflineBinaryName("ifw_test_offline");
        core->setAutoAcceptLicenses();

        // Replace the custom message handler installed in QInstaller::init() to suppress all output
        qInstallMessageHandler(silentTestMessageHandler);

        QCOMPARE(expectedStatus, core->createOfflineInstaller(QStringList() << component, PackageManagerCore::NonHybrid));

        if (expectedStatus == PackageManagerCore::Success) {
#ifdef Q_OS_LINUX
            QVERIFY(QFile::exists(m_targetDir + QDir::separator() + "ifw_test_offline"));
#elif defined Q_OS_MACOS
            QVERIFY(QFile::exists(m_targetDir + QDir::separator() + "ifw_test_offline.dmg"));
#elif defined Q_OS_WIN
            QVERIFY(QFile::exists(m_targetDir + QDir::separator() + "ifw_test_offline.exe"));
#endif
        }
    }

    void testCreateOfflineWithUnstableComponent_data()
    {
        QTest::addColumn<QString>("repository");
        QTest::addColumn<QString>("component");
        QTest::addColumn<bool>("allowUnstable");
        QTest::addColumn<PackageManagerCore::Status>("expectedStatus");
        QTest::newRow("Allow unstable | Missing dependency with selected component")
            << ":///data/repository-missingdependency" << "example.with.unstable.dependency"
            << true << PackageManagerCore::Canceled;
        QTest::newRow("Disallow unstable | Missing dependency with selected component")
            << ":///data/repository-missingdependency" << "example.with.unstable.dependency"
            << false << PackageManagerCore::Canceled;
        QTest::newRow("Allow unstable | Missing dependency with other component")
            << ":///data/repository-missingdependency" << "example.without.unstable.dependency"
            << true << PackageManagerCore::Success;
        QTest::newRow("Disallow unstable | Missing dependency with other component")
            << ":///data/repository-missingdependency" << "example.without.unstable.dependency"
            << false << PackageManagerCore::Canceled;
    }

    void testCreateOfflineWithUnstableComponent()
    {
        QFETCH(QString, repository);
        QFETCH(QString, component);
        QFETCH(bool, allowUnstable);
        QFETCH(PackageManagerCore::Status, expectedStatus);

        QScopedPointer<PackageManagerCore> core(
            PackageManager::getPackageManagerWithInit(m_targetDir, repository));
        core->setCommandLineInstance(true);
        core->setOfflineBaseBinary(m_installerBase);
        core->setOfflineBinaryName("ifw_test_offline");
        core->settings().setAllowUnstableComponents(allowUnstable);
        core->autoAcceptMessageBoxes();

        // Replace the custom message handler installed in QInstaller::init() to suppress all output
        qInstallMessageHandler(silentTestMessageHandler);

        QCOMPARE(expectedStatus, core->createOfflineInstaller(QStringList() << component, PackageManagerCore::NonHybrid));
    }

private:
    QString m_targetDir;
    QString m_installerBase;
};

QTEST_GUILESS_MAIN(tst_CreateOffline)

#include "tst_createoffline.moc"
