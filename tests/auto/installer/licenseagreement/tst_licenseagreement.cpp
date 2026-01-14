/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
*/
#include "../shared/packagemanager.h"

#include <packagemanagercore.h>

#include <QFile>
#include <QTest>

using namespace KDUpdater;
using namespace QInstaller;

class tst_licenseagreement : public QObject
{
    Q_OBJECT

private slots:
    void testAutoAcceptFromCLI()
    {
        QString installDir = QInstaller::generateTemporaryFileName();
        QVERIFY(QDir().mkpath(installDir));
        PackageManagerCore *core = PackageManager::getPackageManagerWithInit
                (installDir, ":///data/repository");

        core->setAutoAcceptLicenses();
        core->installDefaultComponentsSilently();

        QFile file(installDir + "/Licenses/gpl3.txt");
        QVERIFY(file.exists());
        QDir dir(installDir);
        QVERIFY(dir.removeRecursively());
        core->deleteLater();
    }
};

QTEST_MAIN(tst_licenseagreement)

#include "tst_licenseagreement.moc"
