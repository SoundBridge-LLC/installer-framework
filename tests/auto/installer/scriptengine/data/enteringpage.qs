/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
*/

function Controller()
{
}

Controller.prototype.EnteringPageCallback = function()
{
    var page = gui.pageWidgetByObjectName("EnteringPage");
    page.callbackInvoked();
}

