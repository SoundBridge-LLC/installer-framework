/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
*/

function Component()
{
    var error = true;
    if (error) {
        installer.setValue("component_errors", installer.value("component_errors") + ";;;"
            + "Error in component: " + component.name);
    }
    installer.setValue("ComponentError", true);
}
