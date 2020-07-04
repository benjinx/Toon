CMAKE_MINIMUM_REQUIRED(VERSION 3.12 FATAL_ERROR)

# Allow for custom CMake modules
LIST(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

INCLUDE(Policies)

###
### Project
###

PROJECT(Temporality)

###
### Options
###

OPTION(BUILD_DEMOS      "Build Demo Projects"                               ON)
OPTION(BUILD_THIRDPARTY "Build Missing ThirdParty Dependencies"             ON)
OPTION(BUILD_PACKAGES   "Build Packages"                                    OFF)

OPTION(VERBOSE_LOGGING "Build Temporality with Verbose Logging enabled" OFF)
IF(VERBOSE_LOGGING)
    MESSAGE(STATUS "Verbose On.")
    ADD_DEFINITIONS(-DTEMPORALITY_ENABLE_VERBOSE_LOGGING)
ENDIF()

###
### Configuration
###

INCLUDE(Macros)
INCLUDE(Compiler)
INCLUDE(Version)

# Allow for custom organization of files in VisualStudio
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# Don't use the install RPATH when building
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# Limit Configuration Types
IF(CMAKE_CONFIGURATION_TYPES)
    SET(CMAKE_CONFIGURATION_TYPES Debug Release)
ENDIF()

###
### Third Party Dependencies
###

IF(BUILD_THIRDPARTY)
    SET(THIRDPARTY_SOURCE_DIR ${CMAKE_SOURCE_DIR}/thirdparty)
    SET(THIRDPARTY_BINARY_DIR ${CMAKE_BINARY_DIR}/thirdparty)

    FILE(MAKE_DIRECTORY ${THIRDPARTY_BINARY_DIR})

    SET(glm_ROOT            ${THIRDPARTY_BINARY_DIR}/glm)
    SET(nlohmann_json_ROOT  ${THIRDPARTY_BINARY_DIR}/nlohmann_json)
    SET(SDL2_ROOT           ${THIRDPARTY_BINARY_DIR}/SDL2)
    
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
FIND_PACKAGE(nlohmann_json CONFIG REQUIRED)
FIND_PACKAGE(SDL2 2.0.6 CONFIG)

GET_TARGET_PROPERTY(_location SDL2::SDL2 IMPORTED_LOCATION_RELEASE)
GET_FILENAME_COMPONENT(SDL2_RUNTIME_DIR ${_location} DIRECTORY)

# Allow VS to find .dll's
SET(RUNTIME_SEARCH_PATH 
    ${SDL2_RUNTIME_DIR}
)

FILE(RELATIVE_PATH 
    RESOURCE_PATH 
    ${CMAKE_BINARY_DIR} 
    ${CMAKE_SOURCE_DIR}/resources
)
                        
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

###
### Engine
###

CONFIGURE_FILE(
    ${CMAKE_SOURCE_DIR}/include/Config.hpp.in
    ${CMAKE_BINARY_DIR}/Config.hpp
)

# Glob
FILE(GLOB_RECURSE 
    _ENGINE_RESOURCES
    "${CMAKE_SOURCE_DIR}/Resources/*"
)

FILE(GLOB_RECURSE 
    _ENGINE_INCLUDES
    "${CMAKE_SOURCE_DIR}/include/*.hpp"
    "${CMAKE_SOURCE_DIR}/include/*.h"
)

FILE(GLOB_RECURSE 
    _ENGINE_SOURCES  
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.c"
    "${CMAKE_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_SOURCE_DIR}/src/*.h"
)

# Add source to executables
ADD_LIBRARY(
    ${PROJECT_NAME}
    ${CMAKE_BINARY_DIR}/Config.hpp
    ${_ENGINE_INCLUDES}
    ${_ENGINE_SOURCES}
    #${_ENGINE_RESOURCES}
)

# Include Dirs
TARGET_INCLUDE_DIRECTORIES(
    ${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/>
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/imgui
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_BINARY_DIR}
)

TARGET_COMPILE_OPTIONS(
    ${PROJECT_NAME}
    PUBLIC
        # Disable unknown pragmas warning, C++ exceptions
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wno-unknown-pragmas>
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Wno-unknown-pragmas>
        $<$<CXX_COMPILER_ID:MSVC>:/MP /wd4068 /GR- /std:c++17>
)

# Folders to filters
FOREACH(file IN ITEMS ${_ENGINE_INCLUDES} ${_ENGINE_SOURCES})
    FILE(RELATIVE_PATH file_path ${CMAKE_SOURCE_DIR} ${file})
    GET_FILENAME_COMPONENT(file_path ${file_path} DIRECTORY)
    FILE(TO_NATIVE_PATH ${file_path} file_path)
    SOURCE_GROUP(${file_path} FILES ${file})
ENDFOREACH()

# Link all libraries
TARGET_LINK_LIBRARIES(
    ${PROJECT_NAME}
    glm
    nlohmann_json::nlohmann_json
    SDL2::SDL2
)

IF(TARGET SDL2::SDL2main)
    TARGET_LINK_LIBRARIES(
        ${PROJECT_NAME}
        SDL2::SDL2main
    )
ENDIF()

INSTALL(TARGETS ${PROJECT_NAME}
    EXPORT TemporalityTargets
    INCLUDES DESTINATION include
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

INCLUDE(CMakePackageConfigHelpers)

WRITE_BASIC_package_VERSION_FILE(
    "${CMAKE_BINARY_DIR}/TemporalityConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

INSTALL(EXPORT TemporalityTargets
    FILE TemporalityTargets.cmake
    NAMESPACE Temporality::
    DESTINATION lib/cmake/Temporality
)

CONFIGURE_package_CONFIG_FILE(
    "${CMAKE_SOURCE_DIR}/cmake/TemporalityConfig.cmake.in"
    "${CMAKE_BINARY_DIR}/TemporalityConfig.cmake"
    INSTALL_DESTINATION lib/cmake/Temporality
)

INSTALL(EXPORT TemporalityTargets 
    DESTINATION lib/cmake/Temporality
)

INSTALL(FILES 
    "${CMAKE_BINARY_DIR}/TemporalityConfigVersion.cmake" 
    "${CMAKE_BINARY_DIR}/TemporalityConfig.cmake"
    DESTINATION lib/cmake/Temporality
)

FIND_PACKAGE(PkgConfig QUIET)

IF(PKG_CONFIG_FOUND)
    CONFIGURE_FILE(
        "${CMAKE_SOURCE_DIR}/cmake/Temporality.pc.in"
        "${CMAKE_BINARY_DIR}/Temporality.pc"
        @ONLY
    )

    INSTALL(FILES
        "${CMAKE_BINARY_DIR}/Temporality.pc"
        DESTINATION share/pkgconfig
    )
ENDIF()

INSTALL(DIRECTORY
    "${CMAKE_SOURCE_DIR}/include/"
    DESTINATION include
    PATTERN "*.in" EXCLUDE
)


###
### Demos
###

# Allow custom targets to find .dlls
SET(CMAKE_MSVC_RUN_PATH "${RUNTIME_SEARCH_PATH}")

IF(BUILD_DEMOS)
    ADD_SUBDIRECTORY(demos)

    SET_PROPERTY(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "HelloWorld")
ENDIF()

###
### Packaging
###

IF(BUILD_PACKAGES)
    ADD_SUBDIRECTORY(packages)
ENDIF()
