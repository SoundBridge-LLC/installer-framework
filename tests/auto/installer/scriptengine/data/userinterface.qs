/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
*/

function Component()
{
    // check that the .ui file has been properly loaded
    console.log("checked: " + component.userInterface("form").checkBox.checked);
}
