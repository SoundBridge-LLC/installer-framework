/**************************************************************************
**
** Copyright (C) 2025 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Installer Framework.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
**************************************************************************/

#ifndef DOWNLOADARCHIVESJOB_H
#define DOWNLOADARCHIVESJOB_H

#include "job.h"
#include "packagemanagercore.h"
#include <QtCore/QPair>
#include <QtCore/QElapsedTimer>

QT_BEGIN_NAMESPACE
class QTimerEvent;
QT_END_NAMESPACE

#include "filedownloader.h"

namespace QInstaller {

class MessageBoxHandler;

class DownloadArchivesJob : public Job
{
    Q_OBJECT

public:
    explicit DownloadArchivesJob(PackageManagerCore *core, const QString &objectName);
    ~DownloadArchivesJob();

    int numberOfDownloads() const { return m_archivesDownloaded; }
    void setArchivesToDownload(const QList<QPair<QString, QString>> &archives);
    void setExpectedTotalSize(quint64 total);

Q_SIGNALS:
    void progressChanged(double progress);
    void outputTextChanged(const QString &progress);
    void downloadStatusChanged(const QString &status);

    void hashDownloadReady(const QString &localPath);
    void fileDownloadReady(const QString &localPath);

protected:
    void doStart() override;
    void doCancel() override;
    void timerEvent(QTimerEvent *event) override;

public Q_SLOTS:
    void registerFile(const QInstaller::FileTaskItem &item);
    void fileDownloaded(const QString &fileName, const QString &componentName);
    void setTotalProcessedAmount();

protected Q_SLOTS:
    void downloadCompleted();
    void sha1DownloadFinished();
    void downloadAborted(const JobError error, const QString &errorStr);
    void finishWithError(const QString &error);
    void fetchArchives();
    void networkDisconnected();

private:
    void setupDownloaders();
    void onDownloadStatusChanged(const quint64 currentDownloaded);

private:
    PackageManagerCore *m_core;
    QHash<QString, KDUpdater::FileDownloader*> m_downloaders;

    int m_archivesDownloaded;
    QList<QPair<QString, QString> > m_archivesToDownload;

    bool m_canceled;
    int m_progressChangedTimerId;

    QElapsedTimer m_totalDownloadSpeedTimer;
    quint64 m_currentDownloaded;
    quint64 m_totalAmount;
};

} // namespace QInstaller

#endif  // DOWNLOADARCHIVESJOB_H
