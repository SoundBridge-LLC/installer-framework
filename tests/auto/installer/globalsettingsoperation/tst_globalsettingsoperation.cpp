/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
*/

#include "../shared/packagemanager.h"

#include <globalsettingsoperation.h>
#include <environment.h>
#include <packagemanagercore.h>

#include <QSettings>
#include <QTest>

using namespace QInstaller;
using namespace KDUpdater;

class tst_globalsettingsoperation : public QObject
{
    Q_OBJECT

private:
    void cleanSettings()
    {
        QSettings testSettings("QtProject", "QtProject.QtIfwTest");
        testSettings.setValue("QtIfwTestKey", "");
    }


private slots:
    void initTestCase()
    {
        cleanSettings();
    }

    void cleanupTestCase()
    {
        cleanSettings();
    }

    void setGlobalSettingsValue()
    {
        GlobalSettingsOperation settingsOperation(nullptr);
        settingsOperation.setArguments(QStringList() <<  "QtProject" << "QtProject.QtIfwTest" << "QtIfwTestKey" << "QtIfwTestValue");
        settingsOperation.backup();
        QVERIFY2(settingsOperation.performOperation(), settingsOperation.errorString().toLatin1());

        QSettings testSettings("QtProject", "QtProject.QtIfwTest");
        QCOMPARE("QtIfwTestValue", testSettings.value("QtIfwTestKey"));
        QVERIFY2(settingsOperation.undoOperation(), settingsOperation.errorString().toLatin1());
        QCOMPARE("", testSettings.value("QtIfwTestKey"));
    }

    void setGlobalSettingsValueNoUndo()
    {

        GlobalSettingsOperation settingsOperation(nullptr);
        settingsOperation.setArguments(QStringList() <<  "QtProject" << "QtProject.QtIfwTest" << "QtIfwTestKey" << "QtIfwTestValue" << "UNDOOPERATION" << "");
        settingsOperation.backup();
        QVERIFY2(settingsOperation.performOperation(), settingsOperation.errorString().toLatin1());

        QSettings testSettings("QtProject", "QtProject.QtIfwTest");
        QCOMPARE("QtIfwTestValue", testSettings.value("QtIfwTestKey"));
        QVERIFY2(settingsOperation.undoOperation(), settingsOperation.errorString().toLatin1());
        QCOMPARE("QtIfwTestValue", testSettings.value("QtIfwTestKey"));
    }
};

QTEST_MAIN(tst_globalsettingsoperation)

#include "tst_globalsettingsoperation.moc"

