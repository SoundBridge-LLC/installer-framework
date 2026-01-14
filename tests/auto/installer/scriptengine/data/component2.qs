/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
*/

function Component()
{
    print("script function: " + arguments.callee.name);
    // adding some broken javascript code here
    broken + 789634
}
