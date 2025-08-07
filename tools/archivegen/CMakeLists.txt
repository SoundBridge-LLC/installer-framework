set(PROJECT_NAME archivegen)

################################################################################
# Source groups
################################################################################
set(Headers
    "../common/repositorygen.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "../common/repositorygen.cpp"
    "archive.cpp"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${Headers}
    ${Sources}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    TARGET_NAME_DEBUG   "archivegen"
    TARGET_NAME_RELEASE "archivegen"
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
    "${CMAKE_CURRENT_SOURCE_DIR}/../common"
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
        /SUBSYSTEM:CONSOLE;
        /NXCOMPAT;
        /DYNAMICBASE;
    )
endif()

################################################################################
# Dependencies
################################################################################
add_dependencies(${PROJECT_NAME}
    7z
    installer
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${common_link_libraries}
    7z
    installer
    Qt5::Qml
    Qt5::Xml
)

install(TARGETS ${PROJECT_NAME})
