CMAKE_MINIMUM_REQUIRED(VERSION 3.12 FATAL_ERROR)

LIST(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/../cmake")

INCLUDE(Policies)
INCLUDE(ExternalProject)

PROJECT(thirdparty)

# nlohmann_json

SET(nlohmann_json_ROOT       ${CMAKE_BINARY_DIR}/nlohmann_json)
SET(nlohmann_json_SOURCE_DIR ${CMAKE_BINARY_DIR}/nlohmann_json_src)

FIND_PACKAGE(nlohmann_json CONFIG QUIET)

IF(NOT nlohmann_json_FOUND)
    ExternalProject_Add(
        thirdparty-nlohmann_json
        GIT_REPOSITORY      "https://github.com/nlohmann/json.git"
        GIT_TAG             "master"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          ${nlohmann_json_SOURCE_DIR}
        CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${nlohmann_json_ROOT}
                            -DJSON_BuildTests=OFF
        BUILD_COMMAND       ""
        UPDATE_COMMAND      ""
    )
ENDIF()

# glm

SET(glm_ROOT       ${CMAKE_BINARY_DIR}/glm)
SET(glm_SOURCE_DIR ${CMAKE_BINARY_DIR}/glm_src)

FIND_PACKAGE(glm CONFIG QUIET)

IF(NOT glm_FOUND)
    ExternalProject_Add(
        thirdparty-glm
        GIT_REPOSITORY      "https://github.com/g-truc/glm.git"
        GIT_TAG             "0.9.9.3"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          ${glm_SOURCE_DIR}
        CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${glm_ROOT}
                            -DGLM_TEST_ENABLE=OFF
        BUILD_COMMAND       ""
        UPDATE_COMMAND      ""
    )
ENDIF()

# SDL2

SET(SDL2_ROOT         ${CMAKE_BINARY_DIR}/SDL2)
SET(SDL2_SOURCE_DIR   ${CMAKE_BINARY_DIR}/SDL2_src)
SET(SDL2_BINARY_DIR   ${CMAKE_BINARY_DIR}/SDL2_bin)

FIND_PACKAGE(SDL2 QUIET)

IF (NOT SDL2_FOUND)
    ExternalProject_Add(
        thirdparty-SDL2
        HG_REPOSITORY      "http://hg.libsdl.org/SDL"
        HG_TAG             "release-2.0.12"
        SOURCE_DIR          ${SDL2_SOURCE_DIR}
        BINARY_DIR          ${SDL2_BINARY_DIR}
        CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${SDL2_ROOT}
                            -DBUILD_SHARED_LIBS=ON
        UPDATE_COMMAND      ""
        BUILD_COMMAND       ""
        INSTALL_COMMAND     ${CMAKE_COMMAND} --build . --target install --config Release
        TEST_COMMAND        ""
    )
ENDIF()
