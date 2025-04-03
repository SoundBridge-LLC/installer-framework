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

#include "componentselectedproxymodel.h"
#include "componentmodel.h"
#include "fileutils.h"

namespace QInstaller {

/*!
    \inmodule QtInstallerFramework
    \class QInstaller::ComponentSelectedProxyModel
    \internal
*/

ComponentSelectedProxyModel::ComponentSelectedProxyModel(PackageManagerCore *core, SelectionMode selectionMode, QObject *parent)
    : QSortFilterProxyModel(parent)
    , m_core(core)
    , m_selectionMode(selectionMode)
{
    setRecursiveFilteringEnabled(true);
}

bool ComponentSelectedProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex sourceIndex = sourceModel()->index(sourceRow, 0, sourceParent);
    ComponentModel *model = qobject_cast<ComponentModel*>(sourceModel());
    if (!model)
        return false;
    Component *component = model->componentFromIndex(sourceIndex);
    if (!component)
        return false;

    if (m_selectionMode == Unselected
            && (m_core->componentsToUninstallNames().contains(component->name()) || childSelected(component))) {
        return true;
    } else if (m_selectionMode == Selected
            && (m_core->componentsToInstallNames().contains(component->name()) || childSelected(component))) {
        return true;
    }

    return false;
}

QVariant ComponentSelectedProxyModel::data(const QModelIndex &index, int role) const
{
    // We are only interested in special handling of size column
    if (index.column() != ComponentModelHelper::UncompressedSizeColumn)
        return QSortFilterProxyModel::data(index, role);

    QModelIndex sourceIndex = mapToSource(index);
    ComponentModel *model = qobject_cast<ComponentModel*>(sourceModel());
    if (!model)
        return QVariant();
    Component *component = model->componentFromIndex(sourceIndex);
    if (!component)
        return QVariant();

    if (m_selectionMode == Unselected
            && (m_core->componentsToUninstallNames().contains(component->name()) || childSelected(component))) {
        return humanReadableSize(component->uncompressedSize(ComponentModelHelper::Uninstall));
    } else if (m_selectionMode == Selected
            && (m_core->componentsToInstallNames().contains(component->name()) || childSelected(component))) {
        return humanReadableSize(component->uncompressedSize(ComponentModelHelper::Install));
    }

    return QVariant();
}

bool ComponentSelectedProxyModel::childSelected(const Component *component) const
{
    if (!component->isInstalled())
        return false;
    const QList<Component*> descendants = component->descendantComponents();
    for (const Component *desc : descendants) {
        if (m_selectionMode == Selected && m_core->componentsToInstallNames().contains(desc->name()))
            return true;
        if (m_selectionMode == Unselected && m_core->componentsToUninstallNames().contains(desc->name()))
            return true;
    }
    return false;
}

} // namespace QInstaller
