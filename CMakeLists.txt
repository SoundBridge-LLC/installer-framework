cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(installerfw C CXX)

find_package(base REQUIRED)
find_package(vector-widgets REQUIRED)
find_package(Qt5 REQUIRED)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(NOT DEFINED IFW_VERSION_STR OR NOT DEFINED IFW_VERSION OR NOT DEFINED GIT_SHA1)
    message(FATAL_ERROR "Version components are not all defined")
endif()

set(IFW_REPOSITORY_FORMAT_VERSION "1.0.0")

# Set common output directory for all configurations
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Also set per-config output directories (for multi-config generators like MSVC)
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_SOURCE_DIR}/lib/${OUTPUTCONFIG})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_SOURCE_DIR}/bin/${OUTPUTCONFIG})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_SOURCE_DIR}/bin/${OUTPUTCONFIG})
endforeach()

# Ensure CMake uses relative paths for shared libs
set(CMAKE_MACOSX_RPATH ON)
set(CMAKE_BUILD_WITH_INSTALL_RPATH OFF)

if(MSVC)
    set(platform_include_directories
        "${CMAKE_SOURCE_DIR}/src/libs/7zip/win/CPP"
    )
else()
    set(common_include_directories
        "${CMAKE_SOURCE_DIR}/src/libs/7zip/unix/CPP"
    )
endif()
set(common_include_directories
    "${CMAKE_SOURCE_DIR}/src/libs/7zip"
    "${CMAKE_SOURCE_DIR}/src/libs/kdtools"
    "${CMAKE_SOURCE_DIR}/src/libs/installer"
    ${platform_include_directories}
)

set(common_compile_definitions
    QT_NO_CAST_FROM_ASCII
    QT_NO_PRIVATE_HEADERS_WARNING
    QT_USE_QSTRINGBUILDER
    _GIT_SHA1_=${GIT_SHA1}
    IFW_VERSION_STR=${IFW_VERSION_STR}
    IFW_VERSION=${IFW_VERSION}
    IFW_REPOSITORY_FORMAT_VERSION=${IFW_REPOSITORY_FORMAT_VERSION}
    LUMIT_INSTALLER
)

if(MSVC)
    set(platform_link_libraries
        Qt5::WinExtras
    )
elseif(APPLE)
    set(platform_link_libraries
        "-framework Carbon"
        "-framework Security"
    )
endif()
set(common_link_libraries
    Qt5::UiTools
    Qt5::CorePrivate
    Qt5::Concurrent
    Qt5::Network
    Qt5::Qml
    Qt5::Xml
    Qt5::PrintSupport
    ${platform_link_libraries}
)

add_subdirectory(src/libs/7zip)
add_subdirectory(src/libs/installer)
add_subdirectory(src/sdk)
add_subdirectory(tools/archivegen)
add_subdirectory(tools/binarycreator)
add_subdirectory(tools/devtool)
add_subdirectory(tools/repocompare)
add_subdirectory(tools/repogen)
add_subdirectory(tools/uninstallermaker)
