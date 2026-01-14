/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
*/

function Component()
{
    var result = QMessageBox["question"]("test.quit", "Installer", "Do you want to quit the installer?<br>" +
        "This message box was created using JavaScript.", QMessageBox.Ok | QMessageBox.Cancel);
    if (result == QMessageBox.Ok) {
        installer.setValue("FinishedText", "<font color='red' size=3>The installer was quit.</font>");
        installer.setDefaultPageVisible(QInstaller.TargetDirectory, false);
        installer.setDefaultPageVisible(QInstaller.ReadyForInstallation, false);
        installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
        installer.setDefaultPageVisible(QInstaller.StartMenuSelection, false);
        installer.setDefaultPageVisible(QInstaller.PerformInstallation, false);
        installer.setDefaultPageVisible(QInstaller.LicenseCheck, false);
        gui.clickButton(buttons.NextButton);
    } else {
        installer.setValue("FinishedText",
            "<font color='green' size=3>The installer was not quit by JavaScript.</font>");
    }
}
