/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
*/

#include <copyfiletask.h>
#include <downloadfiletask.h>
#include <fileio.h>

#include <QFutureWatcher>
#include <QSignalSpy>
#include <QTest>
#include <QTemporaryFile>

using namespace QInstaller;

static const qint64 scLargeSize = 4194304LL;

class tst_Task : public QObject
{
    Q_OBJECT

private slots:
    void copyFile()
    {
        QTemporaryFile file;
        file.setAutoRemove(false);

        if (file.open()) {
            const QString filename = file.fileName();
            QInstaller::blockingWrite(&file, QByteArray(scLargeSize, '1'));
            file.close();

            CopyFileTask fileTask(filename);
            QFutureWatcher<FileTaskResult> watcher;

            QSignalSpy started(&watcher, SIGNAL(started()));
            QSignalSpy finished(&watcher, SIGNAL(finished()));
            QSignalSpy progress(&watcher, SIGNAL(progressValueChanged(int)));

            watcher.setFuture(QtConcurrent::run(&CopyFileTask::doTask, &fileTask));

            watcher.waitForFinished();
            QTest::qWait(10); // Spin the event loop to deliver queued signals.

            QCOMPARE(started.count(), 1);
            QCOMPARE(finished.count(), 1);

            FileTaskResult result = watcher.result();
            QCOMPARE(watcher.future().resultCount(), 1);

            QVERIFY(QFile(result.target()).exists());
            QCOMPARE(file.size(), QFile(result.target()).size());
            QCOMPARE(result.checkSum().toHex(), QByteArray("85304f87b8d90554a63c6f6d1e9cc974fbef8d32"));
        }
    }

    void downloadFile()
    {
        QTemporaryFile file;
        file.setAutoRemove(false);

        if (file.open()) {
            const QString filename = file.fileName();
            QInstaller::blockingWrite(&file, QByteArray(scLargeSize, '1'));
            file.close();

            DownloadFileTask fileTask(QLatin1String("file:///") + filename);
            QFutureWatcher<FileTaskResult> watcher;

            QSignalSpy started(&watcher, SIGNAL(started()));
            QSignalSpy finished(&watcher, SIGNAL(finished()));
            QSignalSpy progress(&watcher, SIGNAL(progressValueChanged(int)));

            watcher.setFuture(QtConcurrent::run(&DownloadFileTask::doTask, &fileTask));

            watcher.waitForFinished();
            QTest::qWait(10); // Spin the event loop to deliver queued signals.

            QCOMPARE(started.count(), 1);
            QCOMPARE(finished.count(), 1);

            FileTaskResult result = watcher.result();
            QCOMPARE(watcher.future().resultCount(), 1);

            QVERIFY(QFile(result.target()).exists());
            QCOMPARE(file.size(), QFile(result.target()).size());
            QCOMPARE(result.checkSum().toHex(), QByteArray("85304f87b8d90554a63c6f6d1e9cc974fbef8d32"));
        }
    }
};

QTEST_MAIN(tst_Task)

#include "tst_task.moc"
