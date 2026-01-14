# Copyright (C) 2026 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#############################################################################
# Adapted from bld_ifw_tools.py and bldinstallercommon.py in qtsdk repository
#############################################################################

#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import os
import platform
import argparse
import shutil
from typing import Generator
from subprocess import check_call
from contextlib import contextmanager


@contextmanager
def cd(path: str) -> Generator:
    oldwd = os.getcwd()
    os.chdir(path)
    try:
        yield
    finally:
        os.chdir(oldwd)


def create_installer_package(src_dir: str, bld_dir: str, target_dir: str, target_name: str):
    print('Creating installer for Qt Installer Framework')

    # Temporary dir for creating installer containing the Qt Installer Framework itself
    package_dir = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'ifw-pkg')
    os.makedirs(package_dir, exist_ok=True)

    # Final directory for the installer containing the Qt Installer Framework itself
    os.makedirs(target_dir, exist_ok=True)

    # Copy binaries to temporary package dir
    shutil.copytree(os.path.join(bld_dir, 'bin'), os.path.join(package_dir, 'bin'), ignore=shutil.ignore_patterns("*.exe.manifest", "*.exp", "*.lib"))

    # Remove symbol information from binaries
    if sys.platform == 'linux':
        with cd(package_dir):
            check_call(["strip", os.path.join(package_dir, 'bin/archivegen')])
            check_call(["strip", os.path.join(package_dir, 'bin/binarycreator')])
            check_call(["strip", os.path.join(package_dir, 'bin/devtool')])
            check_call(["strip", os.path.join(package_dir, 'bin/installerbase')])
            check_call(["strip", os.path.join(package_dir, 'bin/repogen')])

    # Copy remaining payload to package dir
    if platform.machine() == "AMD64":
        shutil.copytree(os.path.join(bld_dir, 'doc/html'), os.path.join(package_dir, 'doc/html'))
    shutil.copytree(os.path.join(src_dir, 'examples'), os.path.join(package_dir, 'examples'))
    shutil.copy(os.path.join(src_dir, 'README'), package_dir)

    # Create 7z
    archive_file = os.path.join(src_dir, 'dist', 'packages', 'org.qtproject.ifw.binaries', 'data', 'data.7z')
    os.makedirs(os.path.dirname(archive_file), exist_ok=True)

    with cd(package_dir):
        check_call([os.path.join(package_dir, 'bin', 'archivegen'), archive_file, '*'])

    # Run binarycreator
    binary_creator = os.path.join(bld_dir, 'bin', 'binarycreator')
    config_file = os.path.join(src_dir, 'dist', 'config', 'config.xml')
    package_dir = os.path.join(src_dir, 'dist', 'packages')
    target = os.path.join(target_dir, target_name)
    with cd(package_dir):
        check_call([binary_creator, '--offline-only', '-c', config_file, '-p', package_dir, target])

    print('Installer package is at: {0}'.format(target))


if __name__ == "__main__":
    parser = argparse.ArgumentParser(prog="Script to create an installer which can be used to install the Qt IFW libraries and tools.")
    parser.add_argument("--src-dir", dest="src_dir", type=str, required=True, help="Absolute path to the installer framework source directory")
    parser.add_argument("--bld-dir", dest="bld_dir", type=str, required=True, help="Absolute path to the installer framework build directory")
    parser.add_argument("--target-dir", dest="target_dir", type=str, required=True, help="Absolute path to the generated installer target directory")
    parser.add_argument("--target-name", dest="target_name", type=str, required=True, help="Filename for the generated installer")

    args = parser.parse_args(sys.argv[1:])
    create_installer_package(args.src_dir, args.bld_dir, args.target_dir, args.target_name)
