/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
*/

function Controller() {

}

Controller.prototype.ComponentSelectionPageCallback = function() {

    var page = gui.pageWidgetByObjectName("ComponentSelectionPage");
    page.addVirtualComponentToUninstall("component")
}
