/* Copyright (C) 2026 The Qt Company Ltd.
 *
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
*/

#include <archivefactory.h>

#include <QObject>
#include <QTest>

using namespace QInstaller;

class MyRarArchive : public AbstractArchive
{
    Q_OBJECT

public:
    MyRarArchive(QObject *parent = nullptr)
        : AbstractArchive(parent)
    {}
    MyRarArchive(const QString &filename, QObject *parent = nullptr)
        : AbstractArchive(parent)
    {
        Q_UNUSED(filename)
    }

    bool open(QIODevice::OpenMode mode)
    {
        Q_UNUSED(mode)
        return true;
    };
    void close() {};
    void setFilename(const QString &filename) { Q_UNUSED(filename) };
    bool extract(const QString &dirPath)
    {
        Q_UNUSED(dirPath)
        return true;
    };
    bool extract(const QString &dirPath, const quint64 totalFiles)
    {
        Q_UNUSED(dirPath)
        Q_UNUSED(totalFiles)
        return true;
    };
    bool create(const QStringList &data)
    {
        Q_UNUSED(data)
        return true;
    };
    QVector<ArchiveEntry> list() { return QVector<ArchiveEntry>(); };
    bool isSupported() { return true; };

public slots:
    void cancel() {};
};

class tst_archivefactory : public QObject
{
    Q_OBJECT

private slots:
    void testCreateDefaultArchiveHandler_data()
    {
        QTest::addColumn<QString>("handler");
        QTest::addColumn<QString>("filename");
        QTest::addColumn<QStringList>("types");
        QTest::newRow("LibArchive")
            << "LibArchive" << "myfile.zip"
            << (QStringList() << "tar" << "tar.gz" << "tar.bz2" << "tar.xz" << "zip" << "7z" << "qbsp");
    }

    void testCreateDefaultArchiveHandler()
    {
        QFETCH(QString, handler);
        QFETCH(QString, filename);
        QFETCH(QStringList, types);

        QVERIFY(ArchiveFactory::instance().containsProduct(handler));
        for (auto &type : types)
            QVERIFY(ArchiveFactory::isSupportedType("file." + type));

        QScopedPointer<AbstractArchive> archive(ArchiveFactory::instance().create(filename));
        QVERIFY(archive);
    }

    void testCreateUnknownArchive_data()
    {
        QTest::addColumn<QString>("filename");
        QTest::newRow("Unknown suffix") << "invalid.file";
        QTest::newRow("Suffix with known part 1") << "myfile.7z.sha1";
        QTest::newRow("Suffix with known part 2") << "myfile.tar.gz.sha1";
    }

    void testCreateUnknownArchive()
    {
        QFETCH(QString, filename);

        QVERIFY(!ArchiveFactory::isSupportedType(filename));
        QScopedPointer<AbstractArchive> archive(ArchiveFactory::instance().create(filename));
        QVERIFY(!archive);
    }

    void testRegisterNewHandler()
    {
        ArchiveFactory::instance().registerArchive<MyRarArchive>("MyRarHandler", QStringList() << "rar");
        QVERIFY(ArchiveFactory::instance().containsProduct("MyRarHandler"));
        QVERIFY(ArchiveFactory::isSupportedType("file.rar"));

        QScopedPointer<AbstractArchive> archive(ArchiveFactory::instance().create("myfile.rar"));
        QVERIFY(archive);
    }
};

QTEST_MAIN(tst_archivefactory)

#include "tst_archivefactory.moc"
