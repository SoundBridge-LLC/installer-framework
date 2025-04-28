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

#include "labelwithpixmap.h"

#include "fileutils.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QStyleOption>
#include <QPainter>

using namespace QInstaller;

LabelWithPixmap::LabelWithPixmap(const QString &text, QString pixmap, QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QLatin1String("LabelWithPixmap"));
    QHBoxLayout *layout = new QHBoxLayout(this);

    m_labelPixmap = new QLabel(this);
    m_labelPixmap->setObjectName(QLatin1String("PixmapLabel"));
    QInstaller::replaceHighDpiImage(pixmap);
    m_labelPixmap->setPixmap(QPixmap(pixmap));
    layout->addWidget(m_labelPixmap);

    m_warningText = new QLabel(text, this);
    m_warningText->setWordWrap(true);
    m_warningText->setObjectName(QLatin1String("LabelText"));

    layout->addWidget(m_warningText, 1);
}

void LabelWithPixmap::setWarningText(const QString &text)
{
    m_warningText->setText(text);
}

void LabelWithPixmap::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

QSize LabelWithPixmap::sizeHint() const
{
    int width = 10; // includes padding
    int height = 0;

    if (!m_warningText->text().isEmpty()) {
        QFontMetrics metrics(m_warningText->font());
        width += metrics.horizontalAdvance(m_warningText->text());
        height = metrics.height(); // Single line height
    }

    if (!m_labelPixmap->pixmap().isNull()) {
        QSize pixmapSize = m_labelPixmap->pixmap().size();
        width += pixmapSize.width();
        height = qMax(height, pixmapSize.height());
    }

    width += layout()->spacing();
    QMargins margins = layout()->contentsMargins();
    width += margins.left() + margins.right();
    height += margins.top() + margins.bottom();

    return QSize(width, height);
}
