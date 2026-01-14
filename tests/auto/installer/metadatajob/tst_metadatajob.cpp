/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
*/

#include "metadatajob.h"
#include "settings.h"

#include <binarycontent.h>
#include <component.h>
#include <errors.h>
#include <fileutils.h>
#include <packagemanagercore.h>
#include <progresscoordinator.h>

#include <QTest>

using namespace QInstaller;

class tst_MetaDataJob : public QObject
{
    Q_OBJECT

private slots:
    void testRepository()
    {
        PackageManagerCore core;
        core.setInstaller();
        QSet<Repository> repoList;
        Repository repo = Repository::fromUserInput(":///data/repository");
        repoList.insert(repo);
        core.settings().setDefaultRepositories(repoList);
        MetadataJob metadata;
        metadata.setPackageManagerCore(&core);
        metadata.start();
        metadata.waitForFinished();
        QCOMPARE(metadata.metadata().count(), 1);
    }

    void testRepositoryUpdateActionAdd()
    {
        PackageManagerCore core;
        core.setInstaller();
        QSet<Repository> repoList;
        Repository repo = Repository::fromUserInput(":///data/repositoryActionAdd/");
        repoList.insert(repo);
        core.settings().setDefaultRepositories(repoList);
        MetadataJob metadata;
        metadata.setPackageManagerCore(&core);

        QTest::ignoreMessage(QtDebugMsg, "Repository to add: \"Example repository\"");
        QTest::ignoreMessage(QtDebugMsg, "Repository to add: \"Example repository\"");
        metadata.start();
        metadata.waitForFinished();
        QCOMPARE(metadata.metadata().count(), 2);
    }

    void testRepositoryUpdateActionRemove()
    {
        PackageManagerCore core;
        core.setInstaller();
        QSet<Repository> repoList;
        Repository repo = Repository::fromUserInput(":///data/repositoryActionRemove");
        Repository repo2 = Repository::fromUserInput(":///data/repository");
        repoList.insert(repo);
        repoList.insert(repo2);
        core.settings().setDefaultRepositories(repoList);
        MetadataJob metadata;
        metadata.setPackageManagerCore(&core);

        QTest::ignoreMessage(QtDebugMsg, "Repository to remove: \"Example repository\"");
        QTest::ignoreMessage(QtDebugMsg, "Repository to remove: \"Example repository\"");
        metadata.start();
        metadata.waitForFinished();
        QCOMPARE(metadata.metadata().count(), 1);
    }

    void testZippedRepository_data()
    {
        QTest::addColumn<QStringList>("repositories");
        QTest::addColumn<int>("metacount");
        QTest::addColumn<bool>("allowUnstable");

        QStringList repositories;
        repositories << ":///data/repositoryZipped/repositoryZipped.7z";
        QTest::newRow("7z repository") << repositories << 1 << true;

        repositories.clear();
        repositories << ":///data/repository7zInvalidMetaSha1/repository7zInvalidMetaSha1.7z";
        QTest::newRow("7z with invalid meta sha1") << repositories << 0 << true;

        repositories.clear();
        repositories << ":///data/repositoryZipped/repositoryZipped.7z" << ":///data/repository7zInvalidMetaSha1/repository7zInvalidMetaSha1.7z";
        QTest::newRow("7z with one valid repository") << repositories << 1 << true;

        repositories.clear();
        repositories << ":///data/repository7zInvalidMetaSha1/repository7zInvalidMetaSha1.7z" << ":///data/repositoryZipped/repositoryZipped.7z";
        QTest::newRow("7z with one valid repository") << repositories << 0 << false;
    }

    void testZippedRepository()
    {
        QFETCH(QStringList, repositories);
        QFETCH(int, metacount);
        QFETCH(bool, allowUnstable);

        PackageManagerCore core;
        core.setInstaller();
        core.setTemporaryRepositories(repositories, false, true);
        core.settings().setAllowUnstableComponents(allowUnstable);

        MetadataJob metadata;
        metadata.setPackageManagerCore(&core);
        metadata.addDownloadType(DownloadType::CompressedPackage);
        metadata.setAutoDelete(true);
        metadata.start();
        metadata.waitForFinished();
        QCOMPARE(metadata.metadata().count(), metacount);
    }
};


QTEST_MAIN(tst_MetaDataJob)

#include "tst_metadatajob.moc"
