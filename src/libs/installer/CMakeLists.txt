set(PROJECT_NAME installer)

################################################################################
# Source groups
################################################################################
set(Forms
    "../kdtools/authenticationdialog.ui"
    "proxycredentialsdialog.ui"
    "serverauthenticationdialog.ui"
)
source_group("Forms" FILES ${Forms})

set(Headers
    "../kdtools/kdtoolsglobal.h"
    "../kdtools/kdjob.h"
    "../kdtools/kdgenericfactory.h"
    "../kdtools/kdselfrestarter.h"
    "../kdtools/kdrunoncechecker.h"
    "../kdtools/kdlockfile.h"
    "../kdtools/kdsysinfo.h"
    "../kdtools/kdupdater.h"
    "../kdtools/kdupdaterapplication.h"
    "../kdtools/kdupdaterfiledownloader.h"
    "../kdtools/kdupdaterfiledownloader_p.h"
    "../kdtools/kdupdaterfiledownloaderfactory.h"
    "../kdtools/kdupdaterpackagesinfo.h"
    "../kdtools/kdupdaterupdate.h"
    "../kdtools/kdupdaterupdateoperation.h"
    "../kdtools/kdupdaterupdateoperationfactory.h"
    "../kdtools/kdupdaterupdateoperations.h"
    "../kdtools/kdupdaterupdatesourcesinfo.h"
    "../kdtools/kdupdatertask.h"
    "../kdtools/kdupdaterupdatefinder.h"
    "../kdtools/kdupdaterupdatesinfo_p.h"
    "../kdtools/environment.h"
    "../kdtools/kdupdaterupdatesinfodata_p.h"
    "productkeycheck.h"
    "packagemanagercore.h"
    "packagemanagercore_p.h"
    "packagemanagergui.h"
    "binaryformat.h"
    "binaryformatengine.h"
    "binaryformatenginehandler.h"
    "repository.h"
    "utils.h"
    "errors.h"
    "component.h"
    "scriptengine.h"
    "componentmodel.h"
    "qinstallerglobal.h"
    "qtpatch.h"
    "consumeoutputoperation.h"
    "replaceoperation.h"
    "linereplaceoperation.h"
    "copydirectoryoperation.h"
    "simplemovefileoperation.h"
    "extractarchiveoperation.h"
    "extractarchiveoperation_p.h"
    "globalsettingsoperation.h"
    "createshortcutoperation.h"
    "createdesktopentryoperation.h"
    "registerfiletypeoperation.h"
    "addkitstospeeddialoperation.h"
    "environmentvariablesoperation.h"
    "installiconsoperation.h"
    "selfrestartoperation.h"
    "settings.h"
    "permissionsettings.h"
    "downloadarchivesjob.h"
    "init.h"
    "adminauthorization.h"
    "elevatedexecuteoperation.h"
    "fakestopprocessforupdateoperation.h"
    "lazyplaintextedit.h"
    "progresscoordinator.h"
    "minimumprogressoperation.h"
    "performinstallationform.h"
    "messageboxhandler.h"
    "licenseoperation.h"
    "component_p.h"
    "qprocesswrapper.h"
    "qsettingswrapper.h"
    "constants.h"
    "packagemanagerproxyfactory.h"
    "createlocalrepositoryoperation.h"
    "lib7z_facade.h"
    "link.h"
    "createlinkoperation.h"
    "packagemanagercoredata.h"
    "globals.h"
    "graph.h"
    "settingsoperation.h"
    "testrepository.h"
    "packagemanagerpagefactory.h"
    "abstracttask.h"
    "abstractfiletask.h"
    "copyfiletask.h"
    "downloadfiletask.h"
    "downloadfiletask_p.h"
    "unziptask.h"
    "observer.h"
    "runextensions.h"
    "metadatajob.h"
    "metadatajob_p.h"
    "installer_global.h"
    "scriptengine_p.h"
    "protocol.h"
    "remoteobject.h"
    "remoteclient.h"
    "remoteserver.h"
    "remoteclient_p.h"
    "remoteserver_p.h"
    "remotefileengine.h"
    "remoteserverconnection.h"
    "remoteserverconnection_p.h"
    "fileio.h"
    "binarycontent.h"
    "binarylayout.h"
    "installercalculator.h"
    "uninstallercalculator.h"
    "componentchecker.h"
    "proxycredentialsdialog.h"
    "serverauthenticationdialog.h"
    "keepaliveobject.h"
    "systeminfo.h"
    "localsocket.h"
    "VectorWizard.h"
)
if(APPLE)
    set(Additional_Headers
        "CreateDockIconOperation.h"
        "MacUtils.h"
    )
endif()
source_group("Headers" FILES ${Headers} ${Additional_Headers})

set(Resources
    "resources/installer.qrc"
)
source_group("Resources" FILES ${Resources})

set(Sources
    "../kdtools/kdjob.cpp"
    "../kdtools/kdselfrestarter.cpp"
    "../kdtools/kdrunoncechecker.cpp"
    "../kdtools/kdlockfile.cpp"
    "../kdtools/kdsysinfo.cpp"
    "../kdtools/kdupdaterapplication.cpp"
    "../kdtools/kdupdaterfiledownloader.cpp"
    "../kdtools/kdupdaterfiledownloaderfactory.cpp"
    "../kdtools/kdupdaterpackagesinfo.cpp"
    "../kdtools/kdupdaterupdate.cpp"
    "../kdtools/kdupdaterupdateoperation.cpp"
    "../kdtools/kdupdaterupdateoperationfactory.cpp"
    "../kdtools/kdupdaterupdateoperations.cpp"
    "../kdtools/kdupdaterupdatesourcesinfo.cpp"
    "../kdtools/kdupdatertask.cpp"
    "../kdtools/kdupdaterupdatefinder.cpp"
    "../kdtools/kdupdaterupdatesinfo.cpp"
    "../kdtools/environment.cpp"
    "productkeycheck.cpp"
    "packagemanagercore.cpp"
    "packagemanagercore_p.cpp"
    "packagemanagergui.cpp"
    "binaryformat.cpp"
    "binaryformatengine.cpp"
    "binaryformatenginehandler.cpp"
    "repository.cpp"
    "fileutils.cpp"
    "utils.cpp"
    "component.cpp"
    "scriptengine.cpp"
    "componentmodel.cpp"
    "qtpatch.cpp"
    "consumeoutputoperation.cpp"
    "replaceoperation.cpp"
    "linereplaceoperation.cpp"
    "copydirectoryoperation.cpp"
    "simplemovefileoperation.cpp"
    "extractarchiveoperation.cpp"
    "globalsettingsoperation.cpp"
    "createshortcutoperation.cpp"
    "createdesktopentryoperation.cpp"
    "registerfiletypeoperation.cpp"
    "addkitstospeeddialoperation.cpp"
    "environmentvariablesoperation.cpp"
    "installiconsoperation.cpp"
    "selfrestartoperation.cpp"
    "downloadarchivesjob.cpp"
    "init.cpp"
    "elevatedexecuteoperation.cpp"
    "fakestopprocessforupdateoperation.cpp"
    "lazyplaintextedit.cpp"
    "progresscoordinator.cpp"
    "minimumprogressoperation.cpp"
    "performinstallationform.cpp"
    "messageboxhandler.cpp"
    "licenseoperation.cpp"
    "component_p.cpp"
    "qprocesswrapper.cpp"
    "qsettingswrapper.cpp"
    "settings.cpp"
    "permissionsettings.cpp"
    "packagemanagerproxyfactory.cpp"
    "createlocalrepositoryoperation.cpp"
    "lib7z_facade.cpp"
    "link.cpp"
    "createlinkoperation.cpp"
    "packagemanagercoredata.cpp"
    "globals.cpp"
    "settingsoperation.cpp"
    "testrepository.cpp"
    "packagemanagerpagefactory.cpp"
    "abstractfiletask.cpp"
    "copyfiletask.cpp"
    "downloadfiletask.cpp"
    "unziptask.cpp"
    "observer.cpp"
    "metadatajob.cpp"
    "protocol.cpp"
    "remoteobject.cpp"
    "remoteclient.cpp"
    "remoteserver.cpp"
    "remotefileengine.cpp"
    "remoteserverconnection.cpp"
    "fileio.cpp"
    "binarycontent.cpp"
    "binarylayout.cpp"
    "installercalculator.cpp"
    "uninstallercalculator.cpp"
    "componentchecker.cpp"
    "proxycredentialsdialog.cpp"
    "serverauthenticationdialog.cpp"
    "keepaliveobject.cpp"
    "systeminfo.cpp"
    "VectorWizard.cpp"
)
if(MSVC)
    set(Additional_Sources
        "../kdtools/kdlockfile_win.cpp"
        "../kdtools/kdsysinfo_win.cpp"
        "adminauthorization_win.cpp"
        "sysinfo_win.cpp"
    )
elseif(APPLE)
    set(Additional_Sources
        "../kdtools/kdlockfile_unix.cpp"
        "../kdtools/kdsysinfo_mac.cpp"
        "adminauthorization_mac.cpp"
        "CreateDockIconOperation.mm"
        "MacUtils.mm"
    )
elseif(UNIX)
    set(Additional_Sources
        "../kdtools/kdlockfile_unix.cpp"
        "../kdtools/kdsysinfo_x11.cpp"
        "adminauthorization_x11.cpp"
    )
endif()
source_group("Sources" FILES ${Sources} ${Additional_Sources})

# Generate Qt files
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(ALL_FILES
    ${Forms}
    ${Headers}
    ${Additional_Headers}
    ${Resources}
    ${Sources}
    ${Additional_Sources}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    TARGET_NAME_DEBUG   "installer"
    TARGET_NAME_RELEASE "installer"
    OUTPUT_DIRECTORY_DEBUG   "${CMAKE_CURRENT_SOURCE_DIR}/../../../lib"
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../../../lib"
)

################################################################################
# Include directories
################################################################################
if(WIN32)
    set(platform_include_directories
        "${CMAKE_CURRENT_SOURCE_DIR}/../7zip/win/C"
        "${CMAKE_CURRENT_SOURCE_DIR}/../7zip/win/CPP"
    )
elseif(UNIX)
    set(platform_include_directories
        "${CMAKE_CURRENT_SOURCE_DIR}/../7zip/unix/C"
        "${CMAKE_CURRENT_SOURCE_DIR}/../7zip/unix/CPP"
        "${CMAKE_CURRENT_SOURCE_DIR}/../7zip/unix/CPP/myWindows"
        "${CMAKE_CURRENT_SOURCE_DIR}/../7zip/unix/CPP/include_windows"
    )
endif()
target_include_directories(${PROJECT_NAME} PUBLIC
    ${common_include_directories}
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
    "${CMAKE_CURRENT_SOURCE_DIR}/../kdtools"
    ${platform_include_directories}
)

################################################################################
# Compile definitions
################################################################################
if(MSVC)
    set(platform_compile_definitions
        WIN_LONG_PATH
        _UNICODE
        _NO_CRYPTO
    )
    # Remove /Zc:strictStrings in Release as per QMake config
    foreach(cfg Release RelWithDebInfo)
        string(REPLACE "/Zc:strictStrings" "" CMAKE_CXX_FLAGS_${cfg} "${CMAKE_CXX_FLAGS_${cfg}}")
    endforeach()
elseif(UNIX)
    set(platform_compile_definitions
        _FILE_OFFSET_BITS=64
        _LARGEFILE_SOURCE
        NDEBUG
        _REENTRANT
        ENV_UNIX
        UNICODE
        _UNICODE
        _NO_CRYPTO
    )
    if(APPLE)
        set(platform_compile_definitions
            ${platform_compile_definitions}
            ENV_MACOSX
        )
    endif()
endif()
target_compile_definitions(${PROJECT_NAME} PRIVATE
    ${common_compile_definitions}
    ${platform_compile_definitions}
    BUILD_SHARED_KDTOOLS
    BUILD_LIB_INSTALLER
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /Od;
            /Zi
        >
        $<$<CONFIG:Release>:
            /O2;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
        >
        /W3;
        /GR;
        -w34100;
        -w34189;
        -w44996;
        ${DEFAULT_CXX_EXCEPTION_HANDLING};
        /Zc:wchar_t
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /DEBUG
        >
        $<$<CONFIG:Release>:
            /INCREMENTAL:NO
        >
        /NOLOGO;
        /SUBSYSTEM:WINDOWS;
        /NXCOMPAT;
        /DYNAMICBASE
    )
endif()

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    7z
)

if(MSVC)
    set(platform_link_libraries
        Qt5::WinExtras
        oleaut32
        user32
        advapi32
        psapi
        ole32
        shell32
    )
elseif(APPLE)
    set(platform_link_libraries
        "-framework AppKit"
    )
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC
    ${common_link_libraries}
    7z
    base::base
    vector-widgets::vector-widgets
    Qt5::Qml
    Qt5::Network
    Qt5::Xml
    Qt5::Concurrent
    Qt5::Widgets
    Qt5::CorePrivate
    Qt5::GuiPrivate
    Qt5::WidgetsPrivate
    Qt5::PrintSupport
    ${platform_link_libraries}
)

install(TARGETS ${PROJECT_NAME})
