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

#include "spacewidget.h"

#include <QStyleOption>
#include <QPainter>
#include <QHBoxLayout>

#include "sysinfo.h"
#include "fileutils.h"
#include "horizontalruler.h"
#include "labelwithpixmap.h"

const QLatin1String SPACE_ITEM("|");
static const char *scSpaceRequired(QT_TRANSLATE_NOOP("QInstaller::SpaceWidget", "Space required: %1"));
static const char *scSpaceFreed(QT_TRANSLATE_NOOP("QInstaller::SpaceWidget", "Space freed: %1"));
static const char *scSpaceAvailable(QT_TRANSLATE_NOOP("QInstaller::SpaceWidget", "Space available: %1"));
static const char *scNoSpaceAvailable(QT_TRANSLATE_NOOP("QInstaller::SpaceWidget", "There is not enough disk space for the installation"));
static const char *scExceedsRecommended(QT_TRANSLATE_NOOP("QInstaller::SpaceWidget", "Space required on your hard drive is more than %1"));
static const char *scSpaceExceedsLimit(QT_TRANSLATE_NOOP("QInstaller::SpaceWidget", "Space exceeds the supported executable size %1 in Windows"));

using namespace QInstaller;

SpaceWidget::SpaceWidget(PackageManagerCore *core, bool showSpaceExceedWidget, QWidget *parent)
    : QWidget(parent)
    , m_core(core)
    , m_spaceRequiredLabel(nullptr)
    , m_spaceAvailableLabel(nullptr)
    , m_noSpaceAvailableLabel(nullptr)
    , m_showSpaceExceedWidget(showSpaceExceedWidget)
{
    setObjectName(QLatin1String("SpaceItem"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *spaceLabelLayout = new QHBoxLayout();
    mainLayout->addWidget(new HorizontalRuler());
    mainLayout->addLayout(spaceLabelLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    spaceLabelLayout->setContentsMargins(0, 0, 0, 0);

    m_spaceRequiredLabel = new QLabel();
    spaceLabelLayout->addWidget(m_spaceRequiredLabel);
    spaceLabelLayout->addWidget(new QLabel(SPACE_ITEM));

    m_spaceAvailableLabel = new QLabel();
    spaceLabelLayout->addWidget(m_spaceAvailableLabel);

    if (m_showSpaceExceedWidget) {
        m_noSpaceAvailableLabel = new LabelWithPixmap(tr(scNoSpaceAvailable), QLatin1String(":/Notification-Type.png"));
        spaceLabelLayout->addWidget(m_noSpaceAvailableLabel);
        m_noSpaceAvailableLabel->setVisible(false);
        m_noSpaceAvailableLabel->setObjectName(QLatin1String("SpaceWarning"));

        m_spaceRecommendationLabel = new LabelWithPixmap(tr(scNoSpaceAvailable), QLatin1String(":/warning.png"));
        spaceLabelLayout->addWidget(m_spaceRecommendationLabel);
        m_spaceRecommendationLabel->setVisible(false);
        m_spaceRecommendationLabel->setObjectName(QLatin1String("SpaceRecommendation"));
    }
    spaceLabelLayout->addStretch();
    installDirectoryChanged(m_core->value(scTargetDir));
    connect(m_core, &PackageManagerCore::installDirectoryChanged, this, &SpaceWidget::installDirectoryChanged);
    connect(m_core, &PackageManagerCore::availableSpaceChanged, this, &SpaceWidget::availableSpaceChanged);
}

void SpaceWidget::updateSpaceRequiredText()
{
    if (m_spaceRequiredLabel) {
        const qint64 requiredDiskSpace = m_core->requiredDiskSpace();
        if (requiredDiskSpace >= 0)
            m_spaceRequiredLabel->setText(tr(scSpaceRequired).arg(humanReadableSize(requiredDiskSpace)));
        else
            m_spaceRequiredLabel->setText(tr(scSpaceFreed).arg(humanReadableSize(requiredDiskSpace)));
    }
}

void SpaceWidget::installDirectoryChanged(const QString &newDirectory)
{
    if (m_spaceAvailableLabel) {
        const KDUpdater::VolumeInfo targetVolume = KDUpdater::VolumeInfo::fromPath(newDirectory);
        m_spaceAvailableLabel->setText(tr(scSpaceAvailable).arg(humanReadableSize(targetVolume.availableSize())));
    }
}

void SpaceWidget::availableSpaceChanged(const PackageManagerCore::SpaceInfo spaceStatus)
{
    updateSpaceRequiredText();
    if (!m_showSpaceExceedWidget || !m_noSpaceAvailableLabel || !m_spaceRecommendationLabel)
        return;

    m_noSpaceAvailableLabel->setVisible(false);
    m_spaceRecommendationLabel->setVisible(false);

    if (spaceStatus == PackageManagerCore::ExecutableSizeExceeded) {
        m_noSpaceAvailableLabel->setWarningText(tr(scSpaceExceedsLimit).arg(humanReadableSize(UINT_MAX)));
        m_noSpaceAvailableLabel->setVisible(true);
    } else if (spaceStatus == PackageManagerCore::SpaceExceeded) {
        m_noSpaceAvailableLabel->setWarningText(tr(scNoSpaceAvailable));
        m_noSpaceAvailableLabel->setVisible(true);
    } else if (spaceStatus == PackageManagerCore::RecommendedSizeExceeded) {
        m_spaceRecommendationLabel->setWarningText(tr(scExceedsRecommended).arg(humanReadableSize(scRecommendedMaxSize)));
        m_spaceRecommendationLabel->setVisible(true);
    }
}

void SpaceWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
