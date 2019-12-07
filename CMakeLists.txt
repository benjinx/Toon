CMAKE_MINIMUM_REQUIRED(VERSION 3.12 FATAL_ERROR)

LIST(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

INCLUDE(Policies)

# Initialize Projects
PROJECT(Temporality)

INCLUDE(Macros)
INCLUDE(Compiler)

# Allow for custom organization of files in VisualStudio
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# Get Git revision
INCLUDE(GetGitRevisionDescription)
GET_GIT_HEAD_REVISION(GIT_REFSPEC PROJECT_VERSION_GIT_HASH)
STRING(SUBSTRING "${PROJECT_VERSION_GIT_HASH}" 0 8 PROJECT_VERSION_GIT_HASH)

GIT_DESCRIBE(VERSION --tags)
STRING(REGEX REPLACE "^v([0-9]+)\\..*" "\\1" PROJECT_VERSION_MAJOR "${VERSION}")
STRING(REGEX REPLACE "^v[0-9]+\\.([0-9]+).*" "\\1" PROJECT_VERSION_MINOR "${VERSION}")
STRING(REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" PROJECT_VERSION_PATCH "${VERSION}")
SET(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

# Allow verbose logging
OPTION(VERBOSE_LOGGING "Build Temporality with Verbose Logging enabled" OFF)
IF(VERBOSE_LOGGING)
    ADD_DEFINITIONS(-DTEMPORALITY_ENABLE_VERBOSE_LOGGING)
ENDIF()


# Third Party Dependencies

OPTION(BUILD_THIRDPARTY "Build Dusk ThirdParty Dependencies" ON)

IF(BUILD_THIRDPARTY)
    FILE(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/thirdparty")

    SET(GLM_ROOT_DIR        "${CMAKE_BINARY_DIR}/thirdparty/glm")
    SET(GLFW3_ROOT_DIR      "${CMAKE_BINARY_DIR}/thirdparty/glfw3")
    LIST(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/thirdparty/assimp)

    EXECUTE_PROCESS(
        COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" "${CMAKE_SOURCE_DIR}/thirdparty"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/thirdparty"
        RESULT_VARIABLE result
    )
    IF(result)
        MESSAGE("${result}")
        MESSAGE(FATAL_ERROR "Failed to configure thirdparty projects")
    ENDIF()

    SET(BUILD_COMMAND ${CMAKE_COMMAND} --build . )
    IF("${CMAKE_GENERATOR}" STREQUAL "Unix Makefiles")
        SET(BUILD_COMMAND ${CMAKE_MAKE_PROGRAM} -l )
    ENDIF()

    EXECUTE_PROCESS(
        COMMAND ${BUILD_COMMAND}
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/thirdparty"
        RESULT_VARIABLE result
    )

    IF(result)
        MESSAGE("${result}")
        MESSAGE(FATAL_ERROR "Failed to build thirdparty projects")
    ENDIF()
ENDIF()

FIND_PACKAGE(GLM REQUIRED)
FIND_PACKAGE(GLFW3 REQUIRED)
FIND_PACKAGE(assimp CONFIG 5.0 REQUIRED)

IF(TARGET assimp::assimp)
    # Extract release-specific configuration
    GET_TARGET_PROPERTY(soname      assimp::assimp IMPORTED_SONAME_RELEASE)
    GET_TARGET_PROPERTY(implib      assimp::assimp IMPORTED_IMPLIB_RELEASE)
    GET_TARGET_PROPERTY(location    assimp::assimp IMPORTED_LOCATION_RELEASE)

    # Set build type agnostic configuration
    SET_TARGET_PROPERTIES(
        assimp::assimp 
        PROPERTIES
            IMPORTED_SONAME     "${soname}"
            IMPORTED_IMPLIB     "${implib}"
            IMPORTED_LOCATION   "${location}"
    )
ENDIF()

# Allow VS to find .dll's
SET(CMAKE_MSVCIDE_RUN_PATH 
    ${GLFW3_RUNTIME_DIR}
)

SET(RUN_PATH 
	"PATH=%PATH%" ${CMAKE_MSVCIDE_RUN_PATH}
)

# Configuration
FILE(RELATIVE_PATH RESOURCE_PATH "${CMAKE_BINARY_DIR}" "${CMAKE_SOURCE_DIR}/resources")
                        
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

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
        "${CMAKE_SOURCE_DIR}/include"
        "${CMAKE_SOURCE_DIR}/include/imgui"
        "${CMAKE_SOURCE_DIR}/src"
        "${CMAKE_BINARY_DIR}"
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
    GLM::GLM
    GLFW3::GLFW3
    assimp::assimp
)

# Add Demos Sub
ADD_SUBDIRECTORY(demos)

SET_PROPERTY(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "HelloWorld")

# Packaging

FIND_PROGRAM(
    TAR_EXECUTABLE
    tar
)

IF (TAR_EXECUTABLE)
    SET(_PACKAGE "libtemporality-${PROJECT_VERSION}")

    FILE(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/${_PACKAGE}")
    
    ADD_CUSTOM_COMMAND(
        OUTPUT "${_PACKAGE}.tgz"
        DEPENDS ${PROJECT_NAME}
        COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX="" ${CMAKE_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} -E env 
            DESTDIR="${CMAKE_BINARY_DIR}/${_PACKAGE}" 
            ${CMAKE_COMMAND} --build . --target install
        COMMAND ${TAR_EXECUTABLE} -cvzf "${CMAKE_BINARY_DIR}/${_PACKAGE}.tgz" "${CMAKE_BINARY_DIR}/${_PACKAGE}/*"
    )

    ADD_CUSTOM_TARGET(
        tgz
        DEPENDS "${_PACKAGE}.tgz"
    )
ENDIF()

FIND_PROGRAM(
    DPKG_DEB_EXECUTABLE
    dpkg-deb
)

IF (DPKG_DEB_EXECUTABLE)
    SET(_PACKAGE "libtemporality-dev_${PROJECT_VERSION}-1")

    FILE(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/${_PACKAGE}/DEBIAN")

    CONFIGURE_FILE(
        "${CMAKE_SOURCE_DIR}/cmake/DEBIAN/control.in"
        "${CMAKE_BINARY_DIR}/${_PACKAGE}/DEBIAN/control"
        @ONLY
    )
    
    ADD_CUSTOM_COMMAND(
        OUTPUT "${_PACKAGE}.deb"
        DEPENDS ${PROJECT_NAME}
        COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX="/usr/local" ${CMAKE_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} -E env 
            DESTDIR="${CMAKE_BINARY_DIR}/${_PACKAGE}" 
            ${CMAKE_COMMAND} --build . --target install
        COMMAND ${DPKG_DEB_EXECUTABLE} --build "${CMAKE_BINARY_DIR}/${_PACKAGE}"
    )

    ADD_CUSTOM_TARGET(
        deb
        DEPENDS "${_PACKAGE}.deb"
    )
ENDIF()

INSTALL(TARGETS ${PROJECT_NAME}
    EXPORT TemporalityTargets
    INCLUDES DESTINATION include
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

INCLUDE(CMakePackageConfigHelpers)

WRITE_BASIC_PACKAGE_VERSION_FILE(
    "${CMAKE_BINARY_DIR}/TemporalityConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

INSTALL(EXPORT TemporalityTargets
    FILE TemporalityTargets.cmake
    NAMESPACE Temporality::
    DESTINATION lib/cmake/Temporality
)

CONFIGURE_PACKAGE_CONFIG_FILE(
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

INCLUDE(FindPkgConfig)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    DESTINATION include
    PATTERN "*.in" EXCLUDE
)
