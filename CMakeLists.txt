CMAKE_MINIMUM_REQUIRED(VERSION 3.2 FATAL_ERROR)

# Initialize Projects
PROJECT(Temporality VERSION 0.2)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
INCLUDE("cmake/Macros.cmake")
INCLUDE("cmake/Compiler.cmake")

# Allow for custom organization of files in VisualStudio
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# Configuration
FILE(RELATIVE_PATH RESOURCE_PATH "${CMAKE_BINARY_DIR}" "${CMAKE_SOURCE_DIR}/resources")
						
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

CONFIGURE_FILE(
	${CMAKE_SOURCE_DIR}/src/Config.hpp.in
	${CMAKE_BINARY_DIR}/Config.hpp
)

# AI
SET(AI_SRC
	src/State.cpp
	src/State.hpp
)

# Audio src
SET(AUDIO_SRC
)

# Core src
SET(CORE_SRC
	src/App.cpp
	src/App.hpp
	src/Camera.cpp
	src/Camera.hpp
	src/Window.cpp
	src/Window.hpp
)

# Game src
SET(GAME_SRC
	src/Cube.cpp
	src/Cube.hpp
	src/GameObject.cpp
	src/GameObject.hpp
	src/Light.cpp
	src/Light.hpp
	src/Scene.cpp
	src/Scene.hpp
)

# Graphics src
SET(GRAPHICS_SRC
	src/Material.cpp
	src/Material.hpp
	src/Mesh.cpp
	src/Mesh.hpp
	src/Shader.cpp
	src/Shader.hpp
)

# Input src
SET(INPUT_SRC
)

# Interface src
SET(INTERFACE_SRC
	src/UI.cpp
	src/UI.hpp
	src/DevUI.cpp
	src/DevUI.hpp
)

# Network src
SET(NETWORK_SRC
)

# Scripting src
SET(SCRIPTING_SRC
	src/ScriptHost.cpp
	src/ScriptHost.hpp
)

# Utilities src
SET(UTILITIES_SRC
	src/Utils.cpp
	src/Utils.hpp
)

# Third-party source files
SET(GLAD_SRC
    src/KHR/khrplatform.h
    src/glad/glad.h
	src/glad/glad.c
)

SET(IMGUI_SRC
    src/imgui/imgui.h
    src/imgui/imgui_internal.h
    src/imgui/imgui_impl_glfw_gl3.h
	src/imgui/imgui.cpp
	src/imgui/imgui_draw.cpp
	src/imgui/imgui_demo.cpp
	src/imgui/imgui_impl_glfw_gl3.cpp
)

SET(STB_SRC
    src/stb/stb_image.h
)

SOURCE_GROUP("AI" FILES ${PROJECT_NAME} ${AI_SRC})
SOURCE_GROUP("Audio" FILES ${PROJECT_NAME} ${AUDIO_SRC})
SOURCE_GROUP("Core" FILES ${PROJECT_NAME} ${CORE_SRC})
SOURCE_GROUP("Game" FILES ${PROJECT_NAME} ${GAME_SRC})
SOURCE_GROUP("Graphics" FILES ${PROJECT_NAME} ${GRAPHICS_SRC})
SOURCE_GROUP("Input" FILES ${PROJECT_NAME} ${INPUT_SRC})
SOURCE_GROUP("Interface" FILES ${PROJECT_NAME} ${INTERFACE_SRC})
SOURCE_GROUP("Network" FILES ${PROJECT_NAME} ${NETWORK_SRC})
SOURCE_GROUP("Scripting" FILES ${PROJECT_NAME} ${SCRIPTING_SRC})
SOURCE_GROUP("Utilities" FILES ${PROJECT_NAME} ${UTILITIES_SRC})

SOURCE_GROUP("3rdParty\\STB" FILES ${PROJECT_NAME} ${STB_SRC})
SOURCE_GROUP("3rdParty\\Glad" FILES ${PROJECT_NAME} ${GLAD_SRC})
SOURCE_GROUP("3rdParty\\ImGui" FILES ${PROJECT_NAME} ${IMGUI_SRC})

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/src
	${CMAKE_BINARY_DIR}
)

# Add source to executables
ADD_LIBRARY(
	${PROJECT_NAME}
	${CMAKE_BINARY_DIR}/Config.hpp
	${AI_SRC}
	${AUDIO_SRC}
	${CORE_SRC}
	${GAME_SRC}
	${GRAPHICS_SRC}
	${INPUT_SRC}
	${INTERFACE_SRC}
	${NETWORK_SRC}
	${SCRIPTING_SRC}
	${UTILITIES_SRC}
	${STB_SRC}
	${GLAD_SRC}
	${IMGUI_SRC}
	${RESOURCES}
)

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
    GIT_REPOSITORY		"https://github.com/g-truc/glm.git"
    GIT_TAG				"0.9.9.3"
    GIT_SHALLOW			TRUE
    SOURCE_DIR			"${CMAKE_BINARY_DIR}/depend/glm"
	CONFIGURE_COMMAND	""
	BUILD_COMMAND		""
    UPDATE_COMMAND		""
    INSTALL_COMMAND		""
    TEST_COMMAND		""
)

INCLUDE_DIRECTORIES(
    "${CMAKE_BINARY_DIR}/depend/glm"
)

ADD_DEPENDENCIES(${PROJECT_NAME} depend-glm)

# glfw
ExternalProject_Add(depend-glfw
    GIT_REPOSITORY		"https://github.com/glfw/glfw"
    GIT_TAG				"3.2.1"
    GIT_SHALLOW			TRUE
    SOURCE_DIR			"${CMAKE_BINARY_DIR}/depend/glfw"
	BINARY_DIR      	"${CMAKE_BINARY_DIR}/depend/build/glfw"
	CMAKE_ARGS			-DGLFW_BUILD_EXAMPLES=OFF
						-DGLFW_BUILD_TESTS=OFF
						-DGLFW_BUILD_DOCS=OFF
						-DGLFW_INSTALL=OFF
                        -DBUILD_SHARED_LIBS=ON
						-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=${CMAKE_BINARY_DIR}
						-DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${CMAKE_BINARY_DIR}
                        -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=${CMAKE_BINARY_DIR}
    UPDATE_COMMAND		""
    INSTALL_COMMAND		""
    TEST_COMMAND		""
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

# assimp
ExternalProject_Add(depend-assimp
    GIT_REPOSITORY  "https://github.com/assimp/assimp"
    GIT_TAG         "master"
    GIT_SHALLOW     TRUE
    SOURCE_DIR      "${CMAKE_BINARY_DIR}/depend/assimp"
    BINARY_DIR      "${CMAKE_BINARY_DIR}/depend/build/assimp"
    CMAKE_ARGS      -DASSIMP_BUILD_ASSIMP_TOOLS=OFF
                    -DASSIMP_BUILD_SAMPLES=OFF
                    -DASSIMP_BUILD_TESTS=OFF
                    -DASSIMP_BUILD_ZLIB=${TRUE_WHEN_WIN32}
                    -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=${CMAKE_BINARY_DIR}
                    -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${CMAKE_BINARY_DIR}
                    -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=${CMAKE_BINARY_DIR}
    UPDATE_COMMAND  ""
    INSTALL_COMMAND ""
    TEST_COMMAND    ""
)

INCLUDE_DIRECTORIES(
    "${CMAKE_BINARY_DIR}/depend/assimp/code"
    "${CMAKE_BINARY_DIR}/depend/assimp/include"
    "${CMAKE_BINARY_DIR}/depend/build/assimp/include"
)

IF(WIN32)
    LIST(APPEND _DEPEND_LIBRARIES_DEBUG "assimp-${MSVC_PREFIX}-mt.lib;IrrXML.lib;zlibstaticd.lib")
    LIST(APPEND _DEPEND_LIBRARIES_RELEASE "assimp-${MSVC_PREFIX}-mt.lib;IrrXML.lib;zlibstatic.lib")
ELSE()
    LIST(APPEND _DEPEND_LIBRARIES "libassimp.so")
ENDIF()

ADD_DEPENDENCIES(${PROJECT_NAME} depend-assimp)

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

SET_PROPERTY(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "HelloWorld")
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
