TEMPLATE = subdirs

include(../../../installerfw.pri)

CONFIG(libarchive) {
    SUBDIRS += libarchive
}
