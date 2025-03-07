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

#ifndef READYFORINSTALLATIONPAGE_P_H
#define READYFORINSTALLATIONPAGE_P_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QTreeView>
#include <QApplication>

#include "packagemanagergui.h"
#include "packagemanagercore.h"
#include "componentselectedproxymodel.h"

namespace Ui {
    class ComponentSummaryForm;
}

namespace QInstaller {

class SpaceWidget;

class TreeItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TreeItemDelegate(QObject *parent);

protected:
    void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

class ReadyForInstallationPagePrivate : public QObject
{
    Q_OBJECT
    friend class ReadyForInstallationPage;
    Q_DISABLE_COPY(ReadyForInstallationPagePrivate)

public:
    ReadyForInstallationPagePrivate(ReadyForInstallationPage *qq, PackageManagerCore *core);
    ~ReadyForInstallationPagePrivate();

private:
    bool entering();
    void initializeTitleLabel(QLabel *label);
    void initializeInstallTree(const QString &title);
    void initializeUninstallTree();
    void initializeTree(QSortFilterProxyModel *model, QTreeView *treeView, QWidget *container);
    void initializeUninstaller();
    void updateView(QSortFilterProxyModel *model, QTreeView *treeView, QWidget *container);
    void decorateTreeView(QTreeView *treeView);
    void adjustTreeWidgetHeight(QTreeView *treeView);
    void showSpaceWidget(bool show);

private:
    Ui::ComponentSummaryForm *m_ui;
    ReadyForInstallationPage *q;
    PackageManagerCore *m_core;
    SpaceWidget *m_spaceWidget;
    ComponentSelectedProxyModel *m_componentsSelectedProxyModel;
    ComponentSelectedProxyModel *m_componentsUnselectedProxyModel;
};

} // namespace QInstaller

#endif // READYFORINSTALLATIONPAGE_P_H
