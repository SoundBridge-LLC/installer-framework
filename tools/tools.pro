TEMPLATE = subdirs

SUBDIRS += \
    archivegen \
    binarycreator \
    repogen \
    devtool \
    repocompare \
    checksumgenerator

macos: \
    SUBDIRS += updater
