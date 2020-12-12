CMAKE_MINIMUM_REQUIRED(VERSION 3.15 FATAL_ERROR)

# Allow for custom CMake modules
LIST(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/CMake")

INCLUDE(Policies)

###
### Project
###

PROJECT(Temporality)
INCLUDE(Version)

###
### General Configuration
###

# Allow for custom organization of files in VisualStudio, Code::Blocks, etc
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# Don't use the install RPATH when building
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
SET(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)

IF(MSVC)
    # Disable Default Exception Handling
    STRING(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
ENDIF()

###
### Options
###

OPTION(VERBOSE_LOGGING "Build Temporality with Verbose Logging"             OFF)
IF(VERBOSE_LOGGING)
    ADD_DEFINITIONS(-DTEMPORALITY_ENABLE_VERBOSE_LOGGING)
ENDIF()

OPTION(BINARY_SHADERS "Build Temporality with Binary Shader Support"        OFF)
IF(BINARY_SHADERS)
    ADD_DEFINITIONS(-DTEMPORALITY_ENABLE_BINARY_SHADERS)
ENDIF()

OPTION(BUILD_TESTS "Build Tests"                                            OFF)
IF(BUILD_TESTS)
    ENABLE_TESTING()
ENDIF()

OPTION(BUILD_DEMOS "Build Demo Projects"                                    ON)
OPTION(BUILD_THIRDPARTY "Build Missing ThirdParty Dependencies"             ON)

###
### Third Party Dependencies
###

IF(BUILD_THIRDPARTY)
    SET(THIRDPARTY_SOURCE_DIR ${CMAKE_SOURCE_DIR}/ThirdParty)
    SET(THIRDPARTY_BINARY_DIR ${CMAKE_BINARY_DIR}/ThirdParty)

    FILE(MAKE_DIRECTORY ${THIRDPARTY_BINARY_DIR})

    # Set _ROOT variables for config-based packages
    SET(glm_ROOT            ${THIRDPARTY_BINARY_DIR}/glm/cmake)
    SET(nlohmann_json_ROOT  ${THIRDPARTY_BINARY_DIR}/nlohmann_json)
    SET(SDL2_ROOT           ${THIRDPARTY_BINARY_DIR}/SDL2)

    # Set CMAKE_PREFIX_PATH for find-based packages
    LIST(APPEND CMAKE_PREFIX_PATH
        ${THIRDPARTY_BINARY_DIR}/GSL
        ${THIRDPARTY_BINARY_DIR}/GTest
    )
    
    EXECUTE_PROCESS(
        COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" 
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            ${THIRDPARTY_SOURCE_DIR}
            WORKING_DIRECTORY ${THIRDPARTY_BINARY_DIR}
        RESULT_VARIABLE _result
    )
    IF(_result)
        MESSAGE("${_result}")
        MESSAGE(FATAL_ERROR "Failed to configure thirdparty projects")
    ENDIF()

    SET(_THIRDPARTY_BUILD_COMMAND ${CMAKE_COMMAND} --build . )

    # If we're using `make`, we can build with extra cores
    IF(CMAKE_GENERATOR STREQUAL "Unix Makefiles")
        SET(_THIRDPARTY_BUILD_COMMAND ${CMAKE_MAKE_PROGRAM} -l )
    ENDIF()

    EXECUTE_PROCESS(
        COMMAND ${_THIRDPARTY_BUILD_COMMAND}
        WORKING_DIRECTORY ${THIRDPARTY_BINARY_DIR}
        RESULT_VARIABLE _result
    )

    IF(_result)
        MESSAGE("${_result}")
        MESSAGE(FATAL_ERROR "Failed to build thirdparty projects")
    ENDIF()
ENDIF()

FIND_PACKAGE(glm CONFIG REQUIRED)
IF(NOT TARGET glm::glm)
    MESSAGE(SEND_ERROR "Incompatible version of glm found.")
ENDIF()

FIND_PACKAGE(GSL REQUIRED)

FIND_PACKAGE(nlohmann_json CONFIG REQUIRED)
FIND_PACKAGE(SDL2 2.0.6 CONFIG REQUIRED)

FIND_PROGRAM(gdb_COMMAND gdb)
FIND_PROGRAM(valgrind_COMMAND valgrind)

FIND_PACKAGE(Python3 3.8 COMPONENTS Interpreter)

IF(TARGET SDL2::SDL2)
    GET_TARGET_PROPERTY(_location SDL2::SDL2 IMPORTED_LOCATION)
    GET_FILENAME_COMPONENT(SDL2_RUNTIME_DIR ${_location} DIRECTORY)

    GET_TARGET_PROPERTY(_location SDL2::SDL2 IMPORTED_LOCATION_DEBUG)
    GET_FILENAME_COMPONENT(SDL2_RUNTIME_DIR_DEBUG ${_location} DIRECTORY)

    GET_TARGET_PROPERTY(_location SDL2::SDL2 IMPORTED_LOCATION_RELEASE)
    GET_FILENAME_COMPONENT(SDL2_RUNTIME_DIR_RELEASE ${_location} DIRECTORY)
ENDIF()

# Allow VS to find .dll's
SET(RUNTIME_SEARCH_PATH 
    ${SDL2_RUNTIME_DIR}
)

SET(RUNTIME_SEARCH_PATH_DEBUG
    ${SDL2_RUNTIME_DIR_DEBUG}
)

SET(RUNTIME_SEARCH_PATH_RELEASE
    ${SDL2_RUNTIME_DIR_RELEASE}
)

###
### Engine
###

ADD_SUBDIRECTORY(Engine)

###
### Modules
###

ADD_SUBDIRECTORY(Modules)

###
### Demos
###

SET(CMAKE_MSVCIDE_RUN_PATH ${RUNTIME_SEARCH_PATH})

IF(BUILD_DEMOS)
    ADD_SUBDIRECTORY(Demos)
ENDIF()

###
### Docker
###

FIND_PROGRAM(docker_COMMAND docker)

IF(docker_COMMAND)
    ADD_CUSTOM_TARGET(
        build-docker-images
        COMMAND ${docker_COMMAND} build --target builder-minimal -t whobrokethebuild/temporality:builder-minimal .
        COMMAND ${docker_COMMAND} build --target builder -t whobrokethebuild/temporality:builder .
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

    ADD_CUSTOM_TARGET(
        push-docker-images
        COMMAND ${docker_COMMAND} push whobrokethebuild/temporality:builder-minimal
        COMMAND ${docker_COMMAND} push whobrokethebuild/temporality:builder
        DEPENDS build-docker-images
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
ENDIF()

###
### CI
###

FIND_PROGRAM(circleci_COMMAND circleci)

IF(circleci_COMMAND)
    ADD_CUSTOM_TARGET(
        test-circleci
        COMMAND ${circleci_COMMAND} local execute --job "Clang, Release, System"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
ENDIF()