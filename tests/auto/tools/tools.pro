TEMPLATE = subdirs

SUBDIRS += \
    repotest

linux-g++* | win32 {
    SUBDIRS += binarycreator
}
