set(PROJECT_NAME binarycreator)

################################################################################
# Source groups
################################################################################
set(Headers
    "rcc/rcc.h"
)
source_group("Headers" FILES ${Headers})

set(Resources
    "binarycreator.qrc"
)
source_group("Resources" FILES ${Resources})

set(Sources
    "../common/repositorygen.cpp"
    "binarycreator.cpp"
    "rcc/rcc.cpp"
    "rcc/rccmain.cpp"
)
source_group("Sources" FILES ${Sources})

# Generate Qt files
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(ALL_FILES
    ${Headers}
    ${Resources}
    ${Sources}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    TARGET_NAME_DEBUG   "binarycreator"
    TARGET_NAME_RELEASE "binarycreator"
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
    "${CMAKE_CURRENT_SOURCE_DIR}/rcc"
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
