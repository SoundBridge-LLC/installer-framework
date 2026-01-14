/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
*/

function Component()
{
}

Component.prototype.createOperations = function ()
{
    console.log("Component::createOperations()");
    component.createOperations();

    component.addOperation("EmptyArg", "Arg", "Arg2", "");
    component.addOperation("EmptyArg", "Arg", "", "Arg3");
    component.addOperation("EmptyArg", "", "Arg2", "Arg3");
    component.addOperation("EmptyArg", ["Arg", "Arg2", ""]);

    component.addElevatedOperation("EmptyArg", "eArg", "eArg2", "");
    component.addElevatedOperation("EmptyArg", "eArg", "", "eArg3");
    component.addElevatedOperation("EmptyArg", "", "eArg2", "eArg3");
    component.addElevatedOperation("EmptyArg", ["eArg", "eArg2", ""]);
}
