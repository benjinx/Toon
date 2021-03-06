CMAKE_MINIMUM_REQUIRED(VERSION 3.15 FATAL_ERROR)

# Allow for custom CMake modules
LIST(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/CMake")

INCLUDE(Policies)

###
### Project
###

PROJECT(Toon)
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

OPTION(
    VERBOSE_LOGGING
    "Build Toon with Verbose Logging"
    OFF
)
IF(VERBOSE_LOGGING)
    ADD_DEFINITIONS(-DTOON_ENABLE_VERBOSE_LOGGING)
ENDIF()

OPTION(
    BUILD_TESTS
    "Build Tests"
    OFF
)
IF(BUILD_TESTS)
    ENABLE_TESTING()
ENDIF()

OPTION(
    BUILD_DEMOS
    "Build Demo Projects"
    ON
)

OPTION(
    BUILD_THIRDPARTY
    "Build Missing ThirdParty Dependencies"
    ON
)

###
### Modules and Dependencies
###

SET(
    MODULES 
    "All" 
    CACHE STRING
    "Semicolon-separated list of required modules, use 'All' to build all modules"
)

# Generate list of available modules
SET(AVAILABLE_MODULES
    "OpenGL"
    "SDL2"
    "STBI"
    "TinyOBJ"
    "Vulkan"
    "GLTF2"
)

IF(BUILD_DEMOS OR MODULES STREQUAL "All")
    SET(MODULES "${AVAILABLE_MODULES}")
ENDIF()

# Filter requested modules that are not available
SET(REQUIRED_MODULES "")
FOREACH(_module ${MODULES})
    IF(${_module} IN_LIST AVAILABLE_MODULES)
        LIST(APPEND REQUIRED_MODULES ${_module})
    ELSE()
        MESSAGE(WARNING "Module '${_module}' not supported on this platform")
    ENDIF()
ENDFOREACH()

# Generate list of required third party dependencies
SET(REQUIRED_DEPENDENCIES
    "glm"
    "GSL"
    "nlohmann_json"
    "Python3"
    "cflags"
)

IF("OpenGL" IN_LIST REQUIRED_MODULES)
    LIST(APPEND REQUIRED_MODULES "SDL2")
ENDIF()

IF("Vulkan" IN_LIST REQUIRED_MODULES)
    LIST(APPEND REQUIRED_DEPENDENCIES "VulkanSDK")
    LIST(APPEND REQUIRED_MODULES "SDL2")
ENDIF()

IF("SDL2" IN_LIST REQUIRED_MODULES)
    LIST(APPEND REQUIRED_DEPENDENCIES "SDL2")
ENDIF()

IF("Archive" IN_LIST REQUIRED_MODULES)
ENDIF()

IF(BUILD_TESTS)
    LIST(APPEND REQUIRED_DEPENDENCIES "GTest")
ENDIF()

LIST(REMOVE_DUPLICATES REQUIRED_MODULES)
LIST(REMOVE_DUPLICATES REQUIRED_DEPENDENCIES)

MESSAGE(STATUS "Available modules: ${AVAILABLE_MODULES}")
MESSAGE(STATUS "Building Modules: ${REQUIRED_MODULES}")
MESSAGE(STATUS "Required Dependencies: ${REQUIRED_DEPENDENCIES}")

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
    SET(cflags_ROOT         ${THIRDPARTY_BINARY_DIR}/cflags)

    # Set CMAKE_PREFIX_PATH for find-based packages
    LIST(APPEND CMAKE_PREFIX_PATH
        ${THIRDPARTY_BINARY_DIR}/GSL
        ${THIRDPARTY_BINARY_DIR}/GTest
    )

    # CMake refuses to quote the list when passing it on the command line, and `;` will
    # split the command in half, so we have to switch them to a different separator
    STRING(REPLACE ";" ":" _tmp "${REQUIRED_DEPENDENCIES}")
    
    EXECUTE_PROCESS(
        COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" 
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -DREQUIRED_DEPENDENCIES=${_tmp}
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

IF("glm" IN_LIST REQUIRED_DEPENDENCIES)
    FIND_PACKAGE(glm CONFIG REQUIRED)
    IF(NOT TARGET glm::glm)
        MESSAGE(SEND_ERROR "Incompatible version of glm found.")
    ENDIF()
ENDIF()

IF("GSL" IN_LIST REQUIRED_DEPENDENCIES)
    FIND_PACKAGE(GSL REQUIRED)
ENDIF()

IF("nlohmann_json" IN_LIST REQUIRED_DEPENDENCIES)
    FIND_PACKAGE(nlohmann_json CONFIG REQUIRED)
ENDIF()

IF("SDL2" IN_LIST REQUIRED_DEPENDENCIES)
    FIND_PACKAGE(SDL2 2.0.6 CONFIG REQUIRED)
ENDIF()

IF("GTest" IN_LIST REQUIRED_DEPENDENCIES)
    SET(GTEST_MSVC_SEARCH "MD")
    FIND_PACKAGE(GTest REQUIRED)
ENDIF()

IF("VulkanSDK" IN_LIST REQUIRED_DEPENDENCIES)
    FIND_PACKAGE(VulkanSDK REQUIRED)
ENDIF()

IF("Python3" IN_LIST REQUIRED_DEPENDENCIES)
    FIND_PACKAGE(Python3 3.8 COMPONENTS Interpreter)
ENDIF()

IF("cflags" IN_LIST REQUIRED_DEPENDENCIES)
    FIND_PACKAGE(cflags CONFIG REQUIRED)
ENDIF()

FIND_PROGRAM(gdb_COMMAND gdb)
FIND_PROGRAM(valgrind_COMMAND valgrind)

IF(TARGET SDL2::SDL2)
    GET_TARGET_PROPERTY(_location SDL2::SDL2 IMPORTED_LOCATION)
    GET_FILENAME_COMPONENT(SDL2_RUNTIME_DIR ${_location} DIRECTORY)

    IF (SDL2_RUNTIME_DIR STREQUAL "")
        STRING(TOUPPER ${CMAKE_BUILD_TYPE} _type)
        GET_TARGET_PROPERTY(_location SDL2::SDL2 IMPORTED_LOCATION_${_type})
        GET_FILENAME_COMPONENT(SDL2_RUNTIME_DIR ${_location} DIRECTORY)
    ENDIF()
ENDIF()

SET(TOON_MODULE_PATH 
    ${SDL2_RUNTIME_DIR}
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
        COMMAND ${docker_COMMAND} build --target builder-minimal -t whobrokethebuild/toon:builder-minimal .
        COMMAND ${docker_COMMAND} build --target builder -t whobrokethebuild/toon:builder .
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

    ADD_CUSTOM_TARGET(
        push-docker-images
        COMMAND ${docker_COMMAND} push whobrokethebuild/toon:builder-minimal
        COMMAND ${docker_COMMAND} push whobrokethebuild/toon:builder
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