CMAKE_MINIMUM_REQUIRED(VERSION 3.12 FATAL_ERROR)

LIST(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/../cmake")

INCLUDE(Policies)
INCLUDE(ExternalProject)

PROJECT(thirdparty)

# glm

SET(glm_ROOT       ${CMAKE_BINARY_DIR}/glm)
SET(glm_SOURCE_DIR ${CMAKE_BINARY_DIR}/glm_src)

FIND_PACKAGE(glm CONFIG QUIET)

IF(NOT glm_FOUND)
    ExternalProject_Add(thirdparty-glm
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

# glfw

SET(glfw3_ROOT         ${CMAKE_BINARY_DIR}/glfw3)
SET(glfw3_SOURCE_DIR   ${CMAKE_BINARY_DIR}/glfw3_src)
SET(glfw3_BINARY_DIR   ${CMAKE_BINARY_DIR}/glfw3_bin)

FIND_PACKAGE(glfw3 QUIET)

IF (NOT glfw3_FOUND)
    ExternalProject_Add(thirdparty-glfw
        GIT_REPOSITORY      "https://github.com/glfw/glfw"
        GIT_TAG             "3.2.1"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          ${glfw3_SOURCE_DIR}
        BINARY_DIR          ${glfw3_BINARY_DIR}
        CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${glfw3_ROOT} 
                            -DGLFW_BUILD_EXAMPLES=OFF
                            -DGLFW_BUILD_TESTS=OFF
                            -DGLFW_BUILD_DOCS=OFF
                            -DGLFW_INSTALL=ON
                            -DBUILD_SHARED_LIBS=ON
        UPDATE_COMMAND      ""
        BUILD_COMMAND       ""
        INSTALL_COMMAND     ${CMAKE_COMMAND} --build . --target install --config Release
        TEST_COMMAND        ""
    )
ENDIF()

# Assimp

SET(assimp_ROOT       ${CMAKE_BINARY_DIR}/assimp)
SET(assimp_SOURCE_DIR ${CMAKE_BINARY_DIR}/assimp_src)
SET(assimp_BINARY_DIR ${CMAKE_BINARY_DIR}/assimp_bin)

FIND_PACKAGE(assimp 5.0 CONFIG QUIET)

IF(NOT assimp_FOUND)
    ExternalProject_Add(thirdparty_assimp
        GIT_REPOSITORY      "https://github.com/assimp/assimp.git"
        GIT_TAG             "master"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          ${assimp_SOURCE_DIR}
        BINARY_DIR          ${assimp_BINARY_DIR}
        CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${assimp_ROOT}
                            -DINJECT_DEBUG_POSTFIX=OFF
                            -DASSIMP_BUILD_ASSIMP_TOOLS=OFF
                            -DASSIMP_BUILD_SAMPLES=OFF
                            -DASSIMP_BUILD_TESTS=OFF
                            -DBUILD_DOCS=OFF
        BUILD_COMMAND       ""
        UPDATE_COMMAND      ""
        TEST_COMMAND        ""
        INSTALL_COMMAND     ${CMAKE_COMMAND} --build . --target install --config Release
    )
ENDIF()
