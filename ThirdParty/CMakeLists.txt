CMAKE_MINIMUM_REQUIRED(VERSION 3.15 FATAL_ERROR)

LIST(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/../CMake")

INCLUDE(Policies)
INCLUDE(ExternalProject)

PROJECT(thirdparty)

FIND_PACKAGE(patch REQUIRED)

SET(REQUIRED_DEPENDENCIES "" CACHE STRING "")

# See /CMakeLists.txt for explanation
STRING(REPLACE ":" ";" REQUIRED_DEPENDENCIES "${REQUIRED_DEPENDENCIES}")

# glm

IF("glm" IN_LIST REQUIRED_DEPENDENCIES)
    SET(glm_ROOT       ${CMAKE_BINARY_DIR}/glm/cmake)
    SET(glm_SOURCE_DIR ${CMAKE_BINARY_DIR}/glm)

    FIND_PACKAGE(glm CONFIG QUIET)

    IF(NOT glm_FOUND OR NOT TARGET glm::glm)
        ExternalProject_Add(
            thirdparty-glm
            URL                 "https://github.com/g-truc/glm/releases/download/0.9.9.8/glm-0.9.9.8.zip"
            URL_HASH            MD5=69895110052f0d711c9c54fbf385f6f5
            SOURCE_DIR          ${glm_SOURCE_DIR}
            CONFIGURE_COMMAND   ""
            BUILD_COMMAND       ""
            INSTALL_COMMAND     ""
            UPDATE_COMMAND      ""
        )
    ENDIF()
ENDIF()

# GSL

IF("GSL" IN_LIST REQUIRED_DEPENDENCIES)
    SET(GSL_ROOT       ${CMAKE_BINARY_DIR}/GSL)
    SET(GSL_SOURCE_DIR ${CMAKE_BINARY_DIR}/GSL_src)

    FIND_PACKAGE(GSL QUIET)

    IF(NOT GSL_FOUND)
        ExternalProject_Add(
            ThirdParty_GSL
            URL                 "https://github.com/microsoft/GSL/archive/v3.1.0.tar.gz"
            URL_HASH            MD5=b6910c54113f921b03dc06642cf7f11c
            SOURCE_DIR          ${GSL_SOURCE_DIR}
            CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${GSL_ROOT}
                                -DGSL_TEST=OFF
            BUILD_COMMAND       ""
            UPDATE_COMMAND      ""
        )
    ENDIF()
ENDIF()

# nlohmann_json

IF("nlohmann_json" IN_LIST REQUIRED_DEPENDENCIES)
    SET(nlohmann_json_ROOT       ${CMAKE_BINARY_DIR}/nlohmann_json)
    SET(nlohmann_json_SOURCE_DIR ${CMAKE_BINARY_DIR}/nlohmann_json_src)

    FIND_PACKAGE(nlohmann_json CONFIG QUIET)

    IF(NOT nlohmann_json_FOUND)
        ExternalProject_Add(
            thirdparty-nlohmann_json
            URL                 "https://github.com/nlohmann/json/archive/v3.9.1.tar.gz"
            URL_HASH            MD5=e386222fb57dd2fcb8a7879fc016d037
            SOURCE_DIR          ${nlohmann_json_SOURCE_DIR}
            CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${nlohmann_json_ROOT}
                                -DJSON_BuildTests=OFF
            BUILD_COMMAND       ""
            UPDATE_COMMAND      ""
        )
    ENDIF()
ENDIF()

# SDL2

IF("SDL2" IN_LIST REQUIRED_DEPENDENCIES)
    SET(SDL2_ROOT         ${CMAKE_BINARY_DIR}/SDL2)
    SET(SDL2_SOURCE_DIR   ${CMAKE_BINARY_DIR}/SDL2_src)
    SET(SDL2_BINARY_DIR   ${CMAKE_BINARY_DIR}/SDL2_bin)

    FIND_PACKAGE(SDL2 2.0.6 CONFIG QUIET)

    IF(CMAKE_GENERATOR STREQUAL "Visual Studio 16 2019")
    SET(_SDL2_PATCH_COMMAND ${patch_COMMAND} ${SDL2_SOURCE_DIR}/CMakeLists.txt -i ${CMAKE_SOURCE_DIR}/SDL2_vcruntime.patch )
    ENDIF()

    IF (NOT SDL2_FOUND)
        ExternalProject_Add(
            thirdparty-SDL2
            URL                 "https://www.libsdl.org/release/SDL2-2.0.12.tar.gz"
            URL_HASH            MD5=783b6f2df8ff02b19bb5ce492b99c8ff
            SOURCE_DIR          ${SDL2_SOURCE_DIR}
            BINARY_DIR          ${SDL2_BINARY_DIR}
            CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${SDL2_ROOT}
                                -DBUILD_SHARED_LIBS=ON
            UPDATE_COMMAND      ""
            BUILD_COMMAND       ""
            TEST_COMMAND        ""
            INSTALL_COMMAND     ${CMAKE_COMMAND} --build . --target install --config Release
            COMMAND             ${CMAKE_COMMAND} --build . --target install --config Debug
            PATCH_COMMAND       ${_SDL2_PATCH_COMMAND}
        )
    ENDIF()
ENDIF()

# GTest

IF("GTest" IN_LIST REQUIRED_DEPENDENCIES)
    SET(GTEST_ROOT       ${CMAKE_BINARY_DIR}/GTest)
    SET(GTEST_SOURCE_DIR ${CMAKE_BINARY_DIR}/GTest_src)
    SET(GTEST_BINARY_DIR ${CMAKE_BINARY_DIR}/GTest_bin)

    SET(GTEST_MSVC_SEARCH "MD")
    FIND_PACKAGE(GTest QUIET)

    IF(NOT GTEST_FOUND)
        ExternalProject_Add(
            ThirdParty_GTest
            URL                 "https://github.com/google/googletest/archive/release-1.10.0.tar.gz"
            URL_HASH            MD5=ecd1fa65e7de707cd5c00bdac56022cd
            SOURCE_DIR          ${GTEST_SOURCE_DIR}
            BINARY_DIR          ${GTEST_BINARY_DIR}
            CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${GTEST_ROOT}
                                -DCMAKE_POSITION_INDEPENDENT_CODE=ON
                                -DBUILD_SHARED_LIBS=ON
            BUILD_COMMAND       ""
            UPDATE_COMMAND      ""
            TEST_COMMAND        ""
            INSTALL_COMMAND     ${CMAKE_COMMAND} --build . --target install --config Release
        )
    ENDIF()
ENDIF()

# cflags

IF("cflags" IN_LIST REQUIRED_DEPENDENCIES)
    SET(cflags_ROOT       ${CMAKE_BINARY_DIR}/cflags)
    SET(cflags_SOURCE_DIR ${CMAKE_BINARY_DIR}/cflags_src)

    FIND_PACKAGE(cflags CONFIG QUIET)

    IF(NOT cflags_FOUND)
        ExternalProject_Add(
            ThirdParty_cflags
            URL                 "https://github.com/WhoBrokeTheBuild/cflags/archive/v2.0.1.zip"
            URL_HASH            MD5=957b2e6a481efd96c0ce72f291176cb4
            SOURCE_DIR          ${cflags_SOURCE_DIR}
            CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${cflags_ROOT}
            BUILD_COMMAND       ""
            UPDATE_COMMAND      ""
        )
    ENDIF()
ENDIF()

