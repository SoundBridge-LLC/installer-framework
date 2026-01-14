/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
*/

function Component()
{
    // Install to @RootDir@ instead of @HomeDir@ on Windows
    if (installer.value("os") === "win") {
        var homeDir = installer.value("HomeDir");
        var targetDir = installer.value("TargetDir").replace(homeDir, "@RootDir@");
        installer.setValue("TargetDir", targetDir);
    }

    // do not show component selection page
    installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
    // no startmenu entry so no need to ask where to create it
    installer.setDefaultPageVisible(QInstaller.StartMenuSelection, false);
}
