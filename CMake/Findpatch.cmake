# Findpatch.cmake
#
# Finds the patch command
#
# If not found, and using windows, it will download patch.exe from GnuWin32
#
# This will define the following variables
#
#   patch_FOUND
#   patch_COMMAND
#

FIND_PROGRAM(
    patch_COMMAND
    NAMES patch
    PATHS
        ${CMAKE_BINARY_DIR}
    PATH_SUFFIXES
        bin
)

IF(NOT patch_COMMAND AND WIN32)
    FILE(DOWNLOAD
        "http://prdownloads.sourceforge.net/gnuwin32/patch-2.5.9-7-bin.zip"
        ${CMAKE_BINARY_DIR}/patch.zip
        EXPECTED_MD5 b9c8b31d62f4b2e4f1887bbb63e8a905
    )

    EXECUTE_PROCESS(
        COMMAND ${CMAKE_COMMAND} -E tar xf ${CMAKE_BINARY_DIR}/patch.zip
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

    FIND_PROGRAM(
        patch_COMMAND
        NAMES patch
        PATHS
            ${CMAKE_BINARY_DIR}
        PATH_SUFFIXES
            bin
    )
ENDIF()

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    patch
    REQUIRED_VARS 
        patch_COMMAND
)