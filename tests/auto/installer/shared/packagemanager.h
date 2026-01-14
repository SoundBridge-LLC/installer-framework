/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
*/

#ifndef PACKAGEMANAGER_H
#define PACKAGEMANAGER_H

#include <packagemanagercore.h>
#include <binaryformatenginehandler.h>
#include <binarycontent.h>
#include <fileutils.h>
#include <settings.h>
#include <init.h>
#include <errors.h>

#include <QTest>

using namespace QInstaller;

void silentTestMessageHandler(QtMsgType, const QMessageLogContext &, const QString &) {}
void exitOnWarningMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    const QByteArray localMsg = msg.toLocal8Bit();
    const char *file = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";
    if (!(type == QtDebugMsg) && !(type == QtInfoMsg)) {
        fprintf(stderr, "Caught message: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        exit(1);
    }
}

struct PackageManager
{
    static PackageManagerCore *getPackageManager(const QString &targetDir, const QString &repository = QString())
    {
        BinaryFormatEngineHandler::instance()->clear();

        PackageManagerCore *core = new PackageManagerCore(BinaryContent::MagicInstallerMarker, QList<OperationBlob> ());
        QString appFilePath = QCoreApplication::applicationFilePath();
        core->disableWriteMaintenanceTool();
        core->setAutoConfirmCommand();
        if (!repository.isEmpty()) {
            QSet<Repository> repoList;
            Repository repo = Repository::fromUserInput(repository);
            repoList.insert(repo);
            core->settings().setDefaultRepositories(repoList);
        }
        core->setValue(scTargetDir, targetDir);
        return core;
    }

    static PackageManagerCore *getPackageManagerWithInit(const QString &targetDir, const QString &repository = QString())
    {
        QInstaller::init();
        qInstallMessageHandler(silentTestMessageHandler);
        return getPackageManager(targetDir, repository);
    }
};
#endif
