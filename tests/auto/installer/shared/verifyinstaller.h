/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
*/

#ifndef VERIFYINSTALLER_H
#define VERIFYINSTALLER_H

#include <packagemanagercore.h>

#include <QString>
#include <QStringList>
#include <QCryptographicHash>
#include <QFile>
#include <QDir>
#include <QtTest/QTest>
#include <QProcess>

#include <iostream>
#include <sstream>

#define QUOTE_(x) #x
#define QUOTE(x) QUOTE_(x)

struct VerifyInstaller
{
    static void verifyInstallerResources(const QString &installDir, const QString &componentName, const QString &fileName)
    {
        QDir dir(installDir + QDir::separator() + "installerResources" + QDir::separator() + componentName);
        QVERIFY2(dir.exists(), qPrintable(QLatin1String("Directory: \"%1\" does not exist").arg(dir.absolutePath())));
        QFileInfo fileInfo;
        fileInfo.setFile(dir, fileName);
        QVERIFY2(fileInfo.exists(), qPrintable(QLatin1String("File: \"%1\" does not exist for \"%2\".")
                .arg(fileName).arg(componentName)));
    }

    static void verifyInstallerResourcesDeletion(const QString &installDir, const QString &componentName)
    {
        QDir dir(installDir + QDir::separator() + "installerResources" + QDir::separator() + componentName);
        QVERIFY2(!dir.exists(), qPrintable(QLatin1String("Directory: \"%1\" is not deleted.").arg(dir.absolutePath())));
    }

    static void verifyInstallerResourceFileDeletion(const QString &installDir, const QString &componentName, const QString &fileName)
    {
        QDir dir(installDir + QDir::separator() + "installerResources" + QDir::separator() + componentName);
        QFileInfo fileInfo;
        fileInfo.setFile(dir, fileName);
        QVERIFY2(!fileInfo.exists(), qPrintable(QLatin1String("File: \"%1\" still exists for \"%2\".")
                                               .arg(fileName).arg(componentName)));
    }

    static void verifyFileExistence(const QString &installDir, const QStringList &fileList)
    {
        for (int i = 0; i < fileList.count(); i++) {
            bool fileExists = QFileInfo::exists(installDir + QDir::separator() + fileList.at(i));
            QVERIFY2(fileExists, QString("File \"%1\" does not exist.").arg(fileList.at(i)).toLatin1());
        }

        QDir dir(installDir);
        QCOMPARE(dir.entryList(QStringList() << "*.*", QDir::Files).count(), fileList.count());
    }

    static QString fileContent(const QString &fileName)
    {
        QFile file(fileName);
        QTextStream stream(&file);
        file.open(QIODevice::ReadOnly);
        QString str = stream.readAll();
        file.close();
        return str;
    }

    static void verifyFileContent(const QString &fileName, const QString &content)
    {
        QVERIFY2(fileContent(fileName).contains(content), qPrintable(QString("File: \"%1\" does not contain \"%2")
            .arg(fileName).arg(content)));
    }

    static void verifyFileHasNoContent(const QString &fileName, const QString &content)
    {
        QVERIFY2(!fileContent(fileName).contains(content), qPrintable(QString("File: \"%1\" contains \"%2\"")
            .arg(fileName).arg(content)));
    }

    static void runMaintenanceTool(const QString &maintenanceTool, QByteArray &output)
    {
        QVERIFY2(QFileInfo::exists(maintenanceTool), qPrintable(QString("Maintenancetool \"%1\""
             "does not exist").arg(maintenanceTool)));
        QEventLoop loop;
        QProcess process;

        QObject::connect(&process, SIGNAL(finished(int,QProcess::ExitStatus)), &loop, SLOT(quit()));
        process.start(maintenanceTool, QStringList() << "install" << "somePackage", QIODevice::ReadOnly);

        if (process.state() != QProcess::NotRunning)
            loop.exec();
        output = process.readAllStandardOutput().trimmed();
    }

    static void runUninstaller(const QString &maintenanceTool)
    {
        QVERIFY2(QFileInfo::exists(maintenanceTool), qPrintable(QString("Maintenancetool \"%1\""
            "does not exist").arg(maintenanceTool)));
        QEventLoop loop;
        QProcess process;

        QObject::connect(&process, SIGNAL(finished(int,QProcess::ExitStatus)), &loop, SLOT(quit()));
        process.start(maintenanceTool, QStringList() << "purge" << "-c", QIODevice::ReadOnly);

        if (process.state() != QProcess::NotRunning)
            loop.exec();
    }

    static void runInstaller(const QString &installDir, const QString &targetFile, QByteArray &output)
    {
        QVERIFY2(QFileInfo::exists(targetFile), qPrintable(QString("Installer \"%1\""
            "does not exist").arg(targetFile)));
        QEventLoop loop;
        QProcess process;

        QObject::connect(&process, SIGNAL(finished(int,QProcess::ExitStatus)), &loop, SLOT(quit()));
        process.start(targetFile, QStringList() << "install" << "--root" << installDir << "-c" << "--accept-licenses", QIODevice::ReadOnly);

        if (process.state() != QProcess::NotRunning)
            loop.exec();
        output = process.readAllStandardOutput().trimmed();
    }

    static void getInstallerBaseBinaryFile(QString &m_installerBase)
    {
        QString ifwDir = QUOTE(IFW_LIB_PATH);
        QDir dir;
        if (!ifwDir.isEmpty()) {
            dir.setPath(ifwDir);
            dir.cdUp();
            dir.cd("bin");
        }
        if (!dir.exists()) {
            dir = QDir::current();
            for (int i = 0; i < 4; i++) {
                if (!dir.cdUp())
                    break;
                if (dir.cd("bin"))
                    break;
            }
        }
        if (!dir.exists())
            QSKIP(qPrintable(QString("Could not find installer path \"%1\"").arg(dir.absolutePath())));

        m_installerBase = dir.absolutePath();
#ifdef Q_OS_WIN
        m_installerBase.append(QLatin1String("/installerbase.exe"));
#else
        m_installerBase.append(QLatin1String("/installerbase"));
#endif
        if (!QFile::exists(m_installerBase)) {
            QSKIP(qPrintable(QString("No \"%1\" binary found in source tree. This can be "
                "the case if this is an out of sources build or the binaries are "
                "installed to a location with a different path prefix.").arg(m_installerBase)));
        }
    }

    static void addToFileMap(const QDir &baseDir, const QFileInfo &fileInfo, QMap<QString, QByteArray> &map)
    {
        QDir directory(fileInfo.absoluteFilePath());
        directory.setFilter(QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::AllDirs | QDir::Files);
        QFileInfoList fileInfoList = directory.entryInfoList();

        foreach (const QFileInfo &info, fileInfoList) {
            if (info.isDir()) {
                map.insert(baseDir.relativeFilePath(info.filePath()), QByteArray());
                addToFileMap(baseDir, info, map);
            } else {
                QCryptographicHash hash(QCryptographicHash::Sha1);
                QFile file(info.absoluteFilePath());
                QVERIFY(file.open(QIODevice::ReadOnly));
                QVERIFY(hash.addData(&file));
                map.insert(baseDir.relativeFilePath(info.filePath()), hash.result().toHex());
                file.close();
            }
        }
    }

    template <typename Func, typename... Args>
    static void verifyListPackagesMessage(QInstaller::PackageManagerCore *core, const QString &message,
                                   Func func, Args... args)
    {
        std::ostringstream stream;
        std::streambuf *buf = std::cout.rdbuf();
        std::cout.rdbuf(stream.rdbuf());

        (core->*func)(std::forward<Args>(args)...);

        std::cout.rdbuf(buf);
        QVERIFY(stream && stream.tellp() == message.size());
        for (const QString &line : message.split(QLatin1String("\n")))
            QVERIFY(stream.str().find(line.toStdString()) != std::string::npos);
    }
};
#endif
