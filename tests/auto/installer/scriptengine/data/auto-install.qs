/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
*/

function Controller()
{
    installer.setMessageBoxAutomaticAnswer("OverwriteTargetDirectory", QMessageBox.Yes);
    installer.setValue("GuiTestValue", "hello");
}

Controller.prototype.IntroductionPageCallback = function()
{
    var widget = gui.currentPageWidget();
    var updaterRadioButton = widget.findChild("UpdaterRadioButton");
    updaterRadioButton.checked = true;
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.ComponentSelectionPageCallback = function()
{
    var notExistingButtonId = 9999999;
    gui.clickButton(notExistingButtonId);
}

Controller.prototype.FinishedPageCallback = function()
{
    print("FinishedPageCallback - OK")
}

// Trailing comment to exercise QTIFW-1062