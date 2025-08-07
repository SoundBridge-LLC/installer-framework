set(PROJECT_NAME installerbase)

################################################################################
# Translations
################################################################################
set(TRANSLATIONS_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/translations")
set(QT_INSTALLATION_DIR "$<TARGET_FILE_DIR:Qt5::qmake>/..")

set(Translations
    "translations/de.ts"
    "translations/en.ts"
    "translations/fr.ts"
    "translations/it.ts"
    "translations/ja.ts"
    "translations/pl.ts"
    "translations/ru.ts"
    "translations/zh_cn.ts"
)
set_source_files_properties(${Translations} PROPERTIES
    OUTPUT_LOCATION "${TRANSLATIONS_OUTPUT_DIR}"
)
qt5_add_translation(QM_FILES ${Translations})

set(Qt_Translations
    "translations/qt_fr.qm"
    "translations/qt_pl.qm"
    "translations/qt_zh_CN.qm"
    "translations/qtbase_de.qm"
    "translations/qtbase_ja.qm"
    "translations/qtbase_ru.qm"
    "translations/qtbase_it.qm"
)
add_custom_target(copy_qt_translations ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${Qt_Translations}
            "${TRANSLATIONS_OUTPUT_DIR}"
    WORKING_DIRECTORY "${QT_INSTALLATION_DIR}"
)

################################################################################
# Source groups
################################################################################
set(Forms
    "settingsdialog.ui"
)
source_group("Forms" FILES ${Forms})

set(Headers
    "commandlineparser.h"
    "console.h"
    "constants.h"
    "installerbase.h"
    "installerbasecommons.h"
    "sdkapp.h"
    "settingsdialog.h"
    "tabcontroller.h"
    "updatechecker.h"
)
source_group("Headers" FILES ${Headers})

set(Resources
    "installerbase.qrc"
)
if(MSVC)
    set(Additional_Resources
        "installerbase.rc"
    )
endif()
source_group("Resources" FILES ${Resources} ${Additional_Resources})

set(Sources
    "commandlineparser.cpp"
    "installerbase.cpp"
    "installerbasecommons.cpp"
    "main.cpp"
    "settingsdialog.cpp"
    "tabcontroller.cpp"
    "updatechecker.cpp"
)
if(MSVC)
    set(Additional_Sources
        "console_win.cpp"
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
    ${Resources}
    ${Additional_Resources}
    ${Sources}
    ${Additional_Sources}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    TARGET_NAME_DEBUG   "installerbase"
    TARGET_NAME_RELEASE "installerbase"
    OUTPUT_DIRECTORY_DEBUG   "${CMAKE_CURRENT_SOURCE_DIR}/../../bin"
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../../bin"
    INSTALL_RPATH "@executable_path;@executable_path/../lib"
)

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    ${common_include_directories}
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    ${common_compile_definitions}
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
        /DYNAMICBASE;
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/MANIFEST:NO")
endif()

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    copy_qt_translations
    7z
    installer
)

if(MSVC)
    set(platform_link_libraries
        Qt5::WinExtras
        user32
    )
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${common_link_libraries}
    7z
    installer
    base::base
    vector-widgets::vector-widgets
    Qt5::UiTools
    Qt5::CorePrivate
    Qt5::Concurrent
    Qt5::Network
    Qt5::Qml
    Qt5::Xml
    Qt5::PrintSupport
    ${platform_link_libraries}
)

install(TARGETS ${PROJECT_NAME})
