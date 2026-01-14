/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
*/

#include "stringdata.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    assert(argc == 3 || !"incorrect number of arguments");

    if (!strcmp(argv[1], "utf8")) {
        printf("%s", utf8Text);
    } else {
        printf("%s", asciiText);
    }

    return atoi(argv[2]);
}

