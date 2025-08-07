set(PROJECT_NAME 7z)

################################################################################
# Source groups
################################################################################
if(WIN32)
    set(7ZIP_BASE win)
    set(Additional_Sources
        "${7ZIP_BASE}/CPP/Windows/DLL.cpp"
        "${7ZIP_BASE}/CPP/7zip/Archive/7z/7zCompressionMode.cpp"
    )
elseif(UNIX)
    set(7ZIP_BASE unix)
    set(Additional_Sources
        "${7ZIP_BASE}/CPP/Common/MyWindows.cpp"
        "${7ZIP_BASE}/CPP/Windows/Synchronization.cpp"
        "${7ZIP_BASE}/CPP/myWindows/myDateAndTime.cpp"
    )
endif()

if(NOT DEFINED 7ZIP_BASE)
    message(FATAL_ERROR "Unsupported platform")
endif()

set(Sources
    "${7ZIP_BASE}/CPP/Common/IntToString.cpp"
    "${7ZIP_BASE}/CPP/Common/MyString.cpp"
    "${7ZIP_BASE}/CPP/Common/MyVector.cpp"
    "${7ZIP_BASE}/CPP/Common/StringConvert.cpp"
    "${7ZIP_BASE}/CPP/Common/StringToInt.cpp"
    "${7ZIP_BASE}/CPP/Common/Wildcard.cpp"
    "${7ZIP_BASE}/CPP/Windows/FileDir.cpp"
    "${7ZIP_BASE}/CPP/Windows/FileFind.cpp"
    "${7ZIP_BASE}/CPP/Windows/FileName.cpp"
    "${7ZIP_BASE}/CPP/Windows/FileIO.cpp"
    "${7ZIP_BASE}/CPP/Windows/PropVariant.cpp"
    "${7ZIP_BASE}/CPP/Windows/PropVariantConversions.cpp"
    "${7ZIP_BASE}/CPP/Windows/System.cpp"
    "${7ZIP_BASE}/CPP/Windows/Time.cpp"
    "${7ZIP_BASE}/CPP/7zip/Common/CreateCoder.cpp"
    "${7ZIP_BASE}/CPP/7zip/Common/CWrappers.cpp"
    "${7ZIP_BASE}/CPP/7zip/Common/InBuffer.cpp"
    "${7ZIP_BASE}/CPP/7zip/Common/InOutTempBuffer.cpp"
    "${7ZIP_BASE}/CPP/7zip/Common/FileStreams.cpp"
    "${7ZIP_BASE}/CPP/7zip/Common/FilterCoder.cpp"
    "${7ZIP_BASE}/CPP/7zip/Common/LimitedStreams.cpp"
    "${7ZIP_BASE}/CPP/7zip/Common/LockedStream.cpp"
    "${7ZIP_BASE}/CPP/7zip/Common/MethodId.cpp"
    "${7ZIP_BASE}/CPP/7zip/Common/MethodProps.cpp"
    "${7ZIP_BASE}/CPP/7zip/Common/OutBuffer.cpp"
    "${7ZIP_BASE}/CPP/7zip/Common/ProgressUtils.cpp"
    "${7ZIP_BASE}/CPP/7zip/Common/StreamBinder.cpp"
    "${7ZIP_BASE}/CPP/7zip/Common/StreamObjects.cpp"
    "${7ZIP_BASE}/CPP/7zip/Common/StreamUtils.cpp"
    "${7ZIP_BASE}/CPP/7zip/Common/VirtThread.cpp"
    "${7ZIP_BASE}/CPP/7zip/UI/Common/ArchiveOpenCallback.cpp"
    "${7ZIP_BASE}/CPP/7zip/UI/Common/DefaultName.cpp"
    "${7ZIP_BASE}/CPP/7zip/UI/Common/EnumDirItems.cpp"
    "${7ZIP_BASE}/CPP/7zip/UI/Common/LoadCodecs.cpp"
    "${7ZIP_BASE}/CPP/7zip/UI/Common/OpenArchive.cpp"
    "${7ZIP_BASE}/CPP/7zip/UI/Common/SetProperties.cpp"
    "${7ZIP_BASE}/CPP/7zip/UI/Common/SortUtils.cpp"
    "${7ZIP_BASE}/CPP/7zip/UI/Common/TempFiles.cpp"
    "${7ZIP_BASE}/CPP/7zip/UI/Common/Update.cpp"
    "${7ZIP_BASE}/CPP/7zip/UI/Common/UpdateAction.cpp"
    "${7ZIP_BASE}/CPP/7zip/UI/Common/UpdateCallback.cpp"
    "${7ZIP_BASE}/CPP/7zip/UI/Common/UpdatePair.cpp"
    "${7ZIP_BASE}/CPP/7zip/UI/Common/UpdateProduce.cpp"
    "${7ZIP_BASE}/CPP/7zip/Archive/LzmaHandler.cpp"
    "${7ZIP_BASE}/CPP/7zip/Archive/SplitHandler.cpp"
    "${7ZIP_BASE}/CPP/7zip/Archive/XzHandler.cpp"
    "${7ZIP_BASE}/CPP/7zip/Archive/Common/CoderMixer2.cpp"
    "${7ZIP_BASE}/CPP/7zip/Archive/Common/CoderMixer2MT.cpp"
    "${7ZIP_BASE}/CPP/7zip/Archive/Common/HandlerOut.cpp"
    "${7ZIP_BASE}/CPP/7zip/Archive/Common/ItemNameUtils.cpp"
    "${7ZIP_BASE}/CPP/7zip/Archive/Common/ParseProperties.cpp"
    "${7ZIP_BASE}/CPP/7zip/Archive/Common/InStreamWithCRC.cpp"
    "${7ZIP_BASE}/CPP/7zip/Archive/Common/OutStreamWithCRC.cpp"
    "${7ZIP_BASE}/CPP/7zip/Archive/Common/MultiStream.cpp"
    "${7ZIP_BASE}/CPP/7zip/Archive/Common/DummyOutStream.cpp"
    "${7ZIP_BASE}/CPP/7zip/Archive/7z/7zDecode.cpp"
    "${7ZIP_BASE}/CPP/7zip/Archive/7z/7zEncode.cpp"
    "${7ZIP_BASE}/CPP/7zip/Archive/7z/7zExtract.cpp"
    "${7ZIP_BASE}/CPP/7zip/Archive/7z/7zFolderInStream.cpp"
    "${7ZIP_BASE}/CPP/7zip/Archive/7z/7zFolderOutStream.cpp"
    "${7ZIP_BASE}/CPP/7zip/Archive/7z/7zHandler.cpp"
    "${7ZIP_BASE}/CPP/7zip/Archive/7z/7zHandlerOut.cpp"
    "${7ZIP_BASE}/CPP/7zip/Archive/7z/7zHeader.cpp"
    "${7ZIP_BASE}/CPP/7zip/Archive/7z/7zIn.cpp"
    "${7ZIP_BASE}/CPP/7zip/Archive/7z/7zOut.cpp"
    "${7ZIP_BASE}/CPP/7zip/Archive/7z/7zProperties.cpp"
    "${7ZIP_BASE}/CPP/7zip/Archive/7z/7zSpecStream.cpp"
    "${7ZIP_BASE}/CPP/7zip/Archive/7z/7zUpdate.cpp"
    "${7ZIP_BASE}/CPP/7zip/Archive/7z/7zRegister.cpp"
    "${7ZIP_BASE}/CPP/7zip/Compress/Bcj2Coder.cpp"
    "${7ZIP_BASE}/CPP/7zip/Compress/Bcj2Register.cpp"
    "${7ZIP_BASE}/CPP/7zip/Compress/BcjCoder.cpp"
    "${7ZIP_BASE}/CPP/7zip/Compress/BcjRegister.cpp"
    "${7ZIP_BASE}/CPP/7zip/Compress/BranchCoder.cpp"
    "${7ZIP_BASE}/CPP/7zip/Compress/BranchMisc.cpp"
    "${7ZIP_BASE}/CPP/7zip/Compress/BranchRegister.cpp"
    "${7ZIP_BASE}/CPP/7zip/Compress/ByteSwap.cpp"
    "${7ZIP_BASE}/CPP/7zip/Compress/CopyCoder.cpp"
    "${7ZIP_BASE}/CPP/7zip/Compress/CopyRegister.cpp"
    "${7ZIP_BASE}/CPP/7zip/Compress/DeltaFilter.cpp"
    "${7ZIP_BASE}/CPP/7zip/Compress/Lzma2Decoder.cpp"
    "${7ZIP_BASE}/CPP/7zip/Compress/Lzma2Encoder.cpp"
    "${7ZIP_BASE}/CPP/7zip/Compress/Lzma2Register.cpp"
    "${7ZIP_BASE}/CPP/7zip/Compress/LzmaDecoder.cpp"
    "${7ZIP_BASE}/CPP/7zip/Compress/LzmaEncoder.cpp"
    "${7ZIP_BASE}/CPP/7zip/Compress/LzmaRegister.cpp"
    "${7ZIP_BASE}/C/Alloc.c"
    "${7ZIP_BASE}/C/Bra.c"
    "${7ZIP_BASE}/C/Bra86.c"
    "${7ZIP_BASE}/C/BraIA64.c"
    "${7ZIP_BASE}/C/CpuArch.c"
    "${7ZIP_BASE}/C/Delta.c"
    "${7ZIP_BASE}/C/LzFind.c"
    "${7ZIP_BASE}/C/LzFindMt.c"
    "${7ZIP_BASE}/C/Lzma2Dec.c"
    "${7ZIP_BASE}/C/Lzma2Enc.c"
    "${7ZIP_BASE}/C/LzmaDec.c"
    "${7ZIP_BASE}/C/LzmaEnc.c"
    "${7ZIP_BASE}/C/MtCoder.c"
    "${7ZIP_BASE}/C/Threads.c"
    "${7ZIP_BASE}/C/7zCrc.c"
    "${7ZIP_BASE}/C/7zCrcOpt.c"
    "${7ZIP_BASE}/C/7zStream.c"
    "${7ZIP_BASE}/C/Xz.c"
    "${7ZIP_BASE}/C/XzIn.c"
    "${7ZIP_BASE}/C/XzCrc64.c"
    "${7ZIP_BASE}/C/XzDec.c"
    "${7ZIP_BASE}/C/XzEnc.c"
    "${7ZIP_BASE}/C/Sha256.c"
)
source_group("Sources" FILES ${Sources} ${Additional_Sources})

set(ALL_FILES
    ${Sources}
    ${Additional_Sources}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    TARGET_NAME_DEBUG   "7z"
    TARGET_NAME_RELEASE "7z"
    OUTPUT_DIRECTORY_DEBUG   "${CMAKE_CURRENT_SOURCE_DIR}/../../../lib"
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../../../lib"
)

################################################################################
# Include directories
################################################################################
if(WIN32)
    set(platform_include_directories
        "${CMAKE_CURRENT_SOURCE_DIR}/win/C"
        "${CMAKE_CURRENT_SOURCE_DIR}/win/CPP"
    )
elseif(UNIX)
    set(platform_include_directories
        "${CMAKE_CURRENT_SOURCE_DIR}/unix/C"
        "${CMAKE_CURRENT_SOURCE_DIR}/unix/CPP"
        "${CMAKE_CURRENT_SOURCE_DIR}/unix/CPP/myWindows"
        "${CMAKE_CURRENT_SOURCE_DIR}/unix/CPP/include_windows"
    )
endif()
target_include_directories(${PROJECT_NAME} PUBLIC
    ${common_include_directories}
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
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
        _CRT_SECURE_NO_WARNINGS
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
        /NOLOGO
    )
elseif(UNIX)
    target_compile_options(${PROJECT_NAME} PRIVATE
        -w
        -fvisibility=hidden
    )
endif()

################################################################################
# Dependencies
################################################################################
target_link_libraries(${PROJECT_NAME} PUBLIC
    ${common_link_libraries}
    Qt5::Core
)
