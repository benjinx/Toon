CMAKE_MINIMUM_REQUIRED(VERSION 3.2 FATAL_ERROR)

CMAKE_POLICY(SET CMP0048 NEW)

IF(NOT ${CMAKE_VERSION} VERSION_LESS "3.13")
    CMAKE_POLICY(SET CMP0076 NEW)
ENDIF()

IF (POLICY CMP0072)
  CMAKE_POLICY(SET CMP0072 NEW)
ENDIF()

# Initialize Projects
PROJECT(Temporality)

LIST(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
INCLUDE("cmake/Macros.cmake")
INCLUDE("cmake/Compiler.cmake")

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
        "${CMAKE_BINARY_DIR}/depend"
        "${CMAKE_BINARY_DIR}"
)

# Folders to filters
FOREACH(file IN ITEMS ${_ENGINE_INCLUDES} ${_ENGINE_SOURCES})
    FILE(RELATIVE_PATH file_path ${CMAKE_SOURCE_DIR} ${file})
    GET_FILENAME_COMPONENT(file_path ${file_path} DIRECTORY)
    FILE(TO_NATIVE_PATH ${file_path} file_path)
    SOURCE_GROUP(${file_path} FILES ${file})
ENDFOREACH()

# Load Dependencies
INCLUDE(ExternalProject)

SET(_DEPEND_LIBRARIES)
SET(_DEPEND_LIBRARIES_DEBUG)
SET(_DEPEND_LIBRARIES_RELEASE)

IF(WIN32)
    SET(TRUE_WHEN_WIN32 "ON")
    SET(FALSE_WHEN_WIN32 "OFF")
ELSE()
    SET(TRUE_WHEN_WIN32 "OFF")
    SET(FALSE_WHEN_WIN32 "ON")
ENDIF()

# opengl
FIND_PACKAGE(OpenGL REQUIRED)

INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIRS})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${OPENGL_LIBRARIES})

# glm
ExternalProject_Add(depend-glm
    GIT_REPOSITORY        "https://github.com/g-truc/glm.git"
    GIT_TAG                "0.9.9.3"
    GIT_SHALLOW            TRUE
    SOURCE_DIR            "${CMAKE_BINARY_DIR}/depend/glm"
    CONFIGURE_COMMAND    ""
    BUILD_COMMAND        ""
    UPDATE_COMMAND        ""
    INSTALL_COMMAND        ""
    TEST_COMMAND        ""
)

INCLUDE_DIRECTORIES(
    "${CMAKE_BINARY_DIR}/depend/glm"
)

ADD_DEPENDENCIES(${PROJECT_NAME} depend-glm)

# glfw
ExternalProject_Add(depend-glfw
    GIT_REPOSITORY        "https://github.com/glfw/glfw"
    GIT_TAG                "3.2.1"
    GIT_SHALLOW            TRUE
    SOURCE_DIR            "${CMAKE_BINARY_DIR}/depend/glfw"
    BINARY_DIR          "${CMAKE_BINARY_DIR}/depend/build/glfw"
    CMAKE_ARGS            -DGLFW_BUILD_EXAMPLES=OFF
                        -DGLFW_BUILD_TESTS=OFF
                        -DGLFW_BUILD_DOCS=OFF
                        -DGLFW_INSTALL=OFF
                        -DBUILD_SHARED_LIBS=ON
                        -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=${CMAKE_BINARY_DIR}
                        -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${CMAKE_BINARY_DIR}
                        -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=${CMAKE_BINARY_DIR}
    UPDATE_COMMAND        ""
    INSTALL_COMMAND        ""
    TEST_COMMAND        ""
)

INCLUDE_DIRECTORIES(
    "${CMAKE_BINARY_DIR}/depend/glfw/include"
    "${CMAKE_BINARY_DIR}/depend/build/glfw/include"
)

IF(WIN32)
    LIST(APPEND _DEPEND_LIBRARIES_DEBUG "glfw3dll.lib")
    LIST(APPEND _DEPEND_LIBRARIES_RELEASE "glfw3dll.lib")
ELSE()
    LIST(APPEND _DEPEND_LIBRARIES "libglfw.so")
ENDIF()

ADD_DEPENDENCIES(${PROJECT_NAME} depend-glfw)

# Link all libraries
FOREACH(lib IN ITEMS ${_DEPEND_LIBRARIES})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} "${CMAKE_BINARY_DIR}/${lib}")
ENDFOREACH()

FOREACH(debug_lib IN ITEMS ${_DEPEND_LIBRARIES_DEBUG})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} debug "${CMAKE_BINARY_DIR}/Debug/${debug_lib}")
ENDFOREACH()

FOREACH(release_lib IN ITEMS ${_DEPEND_LIBRARIES_RELEASE})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} optimized "${CMAKE_BINARY_DIR}/Release/${release_lib}")
ENDFOREACH()

ADD_CUSTOM_TARGET(
    run
    COMMAND $<TARGET_FILE:${PROJECT_NAME}>
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
SET_TARGET_PROPERTIES(
    run
    PROPERTIES FOLDER "Automation"
)

# Add Demos Sub
ADD_SUBDIRECTORY(demos)

SET_PROPERTY(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "HelloWorld")
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")

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