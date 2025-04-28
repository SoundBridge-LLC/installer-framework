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
#ifndef LABELWITHPIXMAP_H
#define LABELWITHPIXMAP_H

#include "installer_global.h"

#include <QWidget>
#include <QLabel>

namespace QInstaller {

class INSTALLER_EXPORT LabelWithPixmap : public QWidget
{
    Q_OBJECT

public:
    explicit LabelWithPixmap(const QString &text, QString pixmap, QWidget *parent = nullptr);

public:
    void setWarningText(const QString &text);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override;

private:
    QLabel *m_warningText;
    QLabel *m_labelPixmap;
};

} // namespace QInstaller

#endif // LABELWITHPIXMAP_H
