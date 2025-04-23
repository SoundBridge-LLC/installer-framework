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

#include "stopprocessforupdateoperation.h"

#include "packagemanagercore.h"

namespace QInstaller {

/*!
    \inmodule QtInstallerFramework
    \class QInstaller::StopProcessForUpdateOperation
    \internal
*/

StopProcessForUpdateOperation::StopProcessForUpdateOperation(PackageManagerCore *core)
    : UpdateOperation(core)
{
    setName(QLatin1String("StopProcessForUpdate"));
}

void StopProcessForUpdateOperation::backup()
{
    return;
}

bool StopProcessForUpdateOperation::performOperation()
{
    setError(KDUpdater::UpdateOperation::NoError);
    if (!checkArgumentCount(1, 1, QLatin1String("<comma separated list of executable paths>")))
        return false;

    PackageManagerCore *const core = packageManager();
    if (!core) {
        setError(KDUpdater::UpdateOperation::UserDefinedError);
        setErrorString(tr("Cannot get package manager core."));
        return false;
    }

    QStringList processes = arguments().at(0).split(QLatin1Char(','), Qt::SkipEmptyParts);
    for (const QString &process : std::as_const(processes)) {
        if (!core->killProcess(process, 5000)) {
            setError(UserDefinedError);
            setErrorString(tr("Could not terminate process \"%1\"").arg(process));
            return false;
        }
    }

    return true;
}

bool StopProcessForUpdateOperation::undoOperation()
{
    return performOperation();
}

bool StopProcessForUpdateOperation::testOperation()
{
    return true;
}

} // namespace QInstaller
