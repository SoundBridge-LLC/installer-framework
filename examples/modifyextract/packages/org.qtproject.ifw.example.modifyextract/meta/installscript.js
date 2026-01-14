/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
*/

function Component()
{
}

Component.prototype.createOperationsForArchive = function(archive)
{
    // don't use the default operation
    // component.createOperationsForArchive(archive);

    // add an extract operation with a modified path
    component.addOperation("Extract", archive, "@TargetDir@/extractToAnotherPath");
}
