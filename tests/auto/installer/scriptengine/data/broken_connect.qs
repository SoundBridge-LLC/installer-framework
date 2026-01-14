/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
*/

function BrokenConnect()
{
    emiter.emitted.connect(receive)
}

function receive()
{
    print("function receive()");
    // this will print an error.
    foo.bar = "test";
}
