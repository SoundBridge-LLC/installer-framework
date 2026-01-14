/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
*/

#include "../../installer/shared/verifyinstaller.h"
#include "../../installer/shared/packagemanager.h"

#include <QResource>

#include <binarycreator.h>
#include <binarycontent.h>
#include <binarydump.h>
#include <fileio.h>
#include <QProcess>

#include <QTest>

class tst_binarycreator : public QObject
{
     Q_OBJECT

private slots:
    void initTestCase()
    {
        VerifyInstaller::getInstallerBaseBinaryFile(m_installerBase);
    }

    void init()
    {
        m_installDir = QInstaller::generateTemporaryDirectory();
    }

    void createInstaller_data() {
        QTest::addColumn<QString>("packages");
        QTest::addColumn<QString>("config");
        QTest::addColumn<QString>("installerName");
        QTest::addColumn<bool>("containsPackages");
        QTest::addColumn<bool>("containsRepositories");
        QTest::addColumn<bool>("isHybrid");
        QTest::addColumn<bool>("offlineOnly");
        QTest::addColumn<bool>("onlineOnly");

        QTest::newRow("Online installer")
            << "data/packages" << "data/config/config.xml"
            << "online_installer"
            << true
            << true
            << false
            << false
            << false;
        QTest::newRow("OnlineOnly installer")
            << "data/packages" << "data/config/config.xml"
            << "online_only_installer"
            << false
            << true
            << false
            << false
            << true;
        QTest::newRow("Offline installer")
            << "data/packages" << "data/config/config.xml"
            << "offline_installer"
            << true
            << false
            << false
            << true
            << false;
        QTest::newRow("Offline hybrid installer")
            << "data/packages" << "data/config/config.xml"
            << "offline_hybrid_installer"
            << true
            << true
            << true
            << true
            << false;
    }

    void createInstaller()
    {
        QFETCH(QString, packages);
        QFETCH(QString, config);
        QFETCH(QString, installerName);
        QFETCH(bool, containsPackages);
        QFETCH(bool, containsRepositories);
        QFETCH(bool, isHybrid);
        QFETCH(bool, offlineOnly);
        QFETCH(bool, onlineOnly);

        qInstallMessageHandler(silentTestMessageHandler);

        //Create binary
        QInstallerTools::BinaryCreatorArgs args;
        {
            args.packagesDirectories.append(packages);
            args.configFile = config;
            args.target = m_installDir.absolutePath() + QLatin1Char('/') + installerName;
            args.templateBinary = m_installerBase;
            args.hybridInstaller = isHybrid;
            args.offlineOnly = offlineOnly;
            args.onlineOnly = onlineOnly;
            QString error;
            QInstallerTools::createBinary(args, error);
        }

        //Create dump from binary
        QString dumpPath;
        QVector<QByteArray> resourceMappings;
        QString target;
        {
            qint64 magicMarker;
            QList<QInstaller::OperationBlob> operations;
            QInstaller::ResourceCollectionManager manager;
            target = args.target;
#ifdef Q_OS_WIN
            target.append(QLatin1String(".exe"));
#endif
            QInstallerTools::BinaryDump::initializeBinaryDump(magicMarker, operations, manager, resourceMappings,
                QInstaller::BinaryContent::MagicCookie, target);
            QInstallerTools::BinaryDump bd;

            dumpPath = m_installDir.absolutePath() + QLatin1String("/dump");
            bd.dump(manager, dumpPath);
        }

        //Verify correct binary content from dump output
        {
            const QString dumpMetaContentPath = dumpPath + QLatin1String("/metadata/");
            QDir dir(dumpMetaContentPath + QLatin1String("A"));
            QCOMPARE(dir.exists(), containsPackages);

            dir = QDir(dumpMetaContentPath + QLatin1String("B"));
            QCOMPARE(dir.exists(), containsPackages);

            const QString configPath = dumpMetaContentPath + QLatin1String("/installer-config/config.xml");
            if (containsRepositories)
                VerifyInstaller::verifyFileContent(configPath, QLatin1String("Repository"));
            else
                VerifyInstaller::verifyFileHasNoContent(configPath, QLatin1String("Repository"));

            const QString configInternalPath = dumpMetaContentPath  + QLatin1String("config/config-internal.ini");

            if (isHybrid)
                VerifyInstaller::verifyFileContent(configInternalPath, QLatin1String("hybridInstaller=true"));
            else
                VerifyInstaller::verifyFileHasNoContent(configPath, QLatin1String("hybridInstaller=true"));
            if (offlineOnly)
                VerifyInstaller::verifyFileContent(configInternalPath, QLatin1String("offlineOnly=true"));
            else
                VerifyInstaller::verifyFileHasNoContent(configPath, QLatin1String("offlineOnly=true"));
        }

        // unmap resources
        foreach (const QByteArray &rccData, resourceMappings)
            QResource::unregisterResource((const uchar*) rccData.data(), QLatin1String(":/metadata"));

        //Run generated installer and verify output
        {
            QByteArray installOutput;
            VerifyInstaller::runInstaller(m_installDir.absolutePath() + QLatin1String("/tmp"), target, installOutput);

            if (offlineOnly || isHybrid) {
                QVERIFY2(!installOutput.contains("Retrieving information from remote repositories"),
                        qPrintable(QString("Offline installer accessed repositories")));
            }
            if (onlineOnly) {
                QVERIFY2(installOutput.contains("Retrieving information from remote repositories"),
                        qPrintable(QString("Online installer did not access repositories")));
            }
        }

        // Try running maintenance tool and verify output
        {
            QString maintenanceTool = m_installDir.absolutePath() + QLatin1String("/tmp/maintenancetool");
#ifdef Q_OS_WIN
            maintenanceTool.append(QLatin1String(".exe"));
#endif
            if (onlineOnly) { // we don't have valid repositories for online mt, nor local packages
                VerifyInstaller::runUninstaller(maintenanceTool);
                return;
            }

            QByteArray installOutput;
            VerifyInstaller::runMaintenanceTool(maintenanceTool, installOutput);

            if (offlineOnly && !isHybrid) {
                QVERIFY2(!installOutput.contains("Retrieving information from remote repositories"),
                         qPrintable(QString("Maintenance tool accessed repositories")));
            }
            if (isHybrid) {
                QVERIFY2(installOutput.contains("Retrieving information from remote repositories"),
                         qPrintable(QString("Maintenance tool did not access repositories")));
            }
            VerifyInstaller::runUninstaller(maintenanceTool);
        }
    }

    void cleanup()
    {
       QDir dir(m_installDir);
       QVERIFY(dir.removeRecursively());
    }

private:
    QDir m_installDir;
    QString m_installerBase;
};

QTEST_MAIN(tst_binarycreator)

#include "tst_binarycreator.moc"
