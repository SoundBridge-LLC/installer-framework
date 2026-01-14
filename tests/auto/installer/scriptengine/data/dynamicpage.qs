/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
*/

function assert(condition)
{
    if (!condition)
        throw new Error("Assertion failed!");
}

function Controller()
{
    var widget = gui.pageWidgetByObjectName("DynamicWidget");
    assert(typeof widget === 'object');
    var button = widget.Button;
    assert(typeof button === 'object');
}

Controller.prototype.ReadAndSetValues = function()
{
    var widget = gui.pageWidgetByObjectName("DynamicWidget");
    installer.setValue("DynamicWidget.final", widget.final);
    installer.setValue("DynamicWidget.commit", widget.commit);
    installer.setValue("DynamicWidget.complete", widget.complete);
}

Controller.prototype.UpdateAndSetValues = function ()
{
    var widget = gui.pageWidgetByObjectName("DynamicWidget");

    widget.final = false;
    widget.commit = false;
    widget.complete = true;

    installer.setValue("DynamicWidget.final", widget.final);
    installer.setValue("DynamicWidget.commit", widget.commit);
    installer.setValue("DynamicWidget.complete", widget.complete);
}
