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

    if (m_selectionMode == Unselected)
        return m_core->componentsToUninstallNames().contains(component->name());

    if (m_core->componentsToInstallNames().contains(component->name()))
        return true;

    if (m_selectionMode == Selected && component->isInstalled()) {
        // Check if child components are selected. The children can be virtual or autodependencies. If parent
        // for those item is already installed, then children do not get filterAcceptsRow() -call.
        const QList<Component*> descendants = component->descendantComponents();
        for (const Component *desc : descendants) {
            if (m_core->componentsToInstallNames().contains(desc->name()))
                return true;
        }
    }

    return false;
}

} // namespace QInstaller
