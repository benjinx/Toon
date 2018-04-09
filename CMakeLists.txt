CMAKE_MINIMUM_REQUIRED(VERSION 3.2 FATAL_ERROR)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
INCLUDE("cmake/Macros.cmake")

# Initialize Projects
PROJECT(Temporality)

# Allow for custom organization of files in VisualStudio
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# Configuration

IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/x64/Debug")
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/x64/Release")
ELSEIF(CMAKE_SIZEOF_VOID_P EQUAL 4)
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug")
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release")
ENDIF()

IF(CMAKE_COMPILER_IS_GNUCC OR ${CMAKE_C_COMPILER_ID} MATCHES "Clang")
    SET(CMAKE_CXX_FLAGS " -std=c++11 -Wall -D_XOPEN_SOURCE=700")
ELSEIF(MSVC)

ENDIF()

# Load Dependencies
ADD_SUBDIRECTORY(depend)

FIND_PACKAGE(OpenGL REQUIRED)

INCLUDE_DIRECTORIES(BEFORE SYSTEM
    ${CMAKE_SOURCE_DIR}/src
    ${OPENGL_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${SOL2_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIRS}
    ${OPENAL_INCLUDE_DIRS}
    ${ASSIMP_INCLUDE_DIRS}
)

ADD_SUBDIRECTORY(demos)

SET(COMMON_LIBS
    ${OPENGL_LIBRARIES}
    ${LUA_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${OPENAL_LIBRARIES}
    ${ASSIMP_LIBRARIES}
)

SET(COMMON_LIBS_DEBUG
    ${LUA_LIBRARIES_DEBUG}
    ${GLFW_LIBRARIES_DEBUG}
    ${OPENAL_LIBRARIES_DEBUG}
    ${ASSIMP_LIBRARIES_DEBUG}
)

SET(COMMON_LIBS_RELEASE
    ${LUA_LIBRARIES_RELEASE}
    ${GLFW_LIBRARIES_RELEASE}
    ${OPENAL_LIBRARIES_RELEASE}
    ${ASSIMP_LIBRARIES_RELEASE}
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
	src/Application.cpp
	src/Application.hpp
	src/Camera.cpp
	src/Camera.hpp
	src/Common.hpp
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
)

# Network src
SET(NETWORK_SRC
)

# Physics src
SET(PHYSICS_SRC
	src/Collision.cpp
	src/Collision.hpp
	src/Rigidbody.cpp
	src/Rigidbody.hpp
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

FILE(RELATIVE_PATH RESOURCES_DIR "${CMAKE_BINARY_DIR}" "${CMAKE_SOURCE_DIR}/resources")
ADD_DEFINITIONS(-DRESOURCES_DIR="${RESOURCES_DIR}/")

FILE(GLOB_RECURSE RESOURCES RELATIVE ${CMAKE_SOURCE_DIR} "resources/*")
FOREACH(res IN ITEMS ${RESOURCES})
    GET_FILENAME_COMPONENT(path "${res}" DIRECTORY)
    FILE(TO_NATIVE_PATH ${path} path)
    SOURCE_GROUP("${path}" FILES ${res})
ENDFOREACH()

# Stop VS from trying to compile .obj files
SET_SOURCE_FILES_PROPERTIES(${RESOURCES} PROPERTIES HEADER_FILE_ONLY true)

SOURCE_GROUP("AI" FILES ${PROJECT_NAME} ${AI_SRC})
SOURCE_GROUP("Audio" FILES ${PROJECT_NAME} ${AUDIO_SRC})
SOURCE_GROUP("Core" FILES ${PROJECT_NAME} ${CORE_SRC})
SOURCE_GROUP("Game" FILES ${PROJECT_NAME} ${GAME_SRC})
SOURCE_GROUP("Graphics" FILES ${PROJECT_NAME} ${GRAPHICS_SRC})
SOURCE_GROUP("Input" FILES ${PROJECT_NAME} ${INPUT_SRC})
SOURCE_GROUP("Interface" FILES ${PROJECT_NAME} ${INTERFACE_SRC})
SOURCE_GROUP("Network" FILES ${PROJECT_NAME} ${NETWORK_SRC})
SOURCE_GROUP("Physics" FILES ${PROJECT_NAME} ${PHYSICS_SRC})
SOURCE_GROUP("Scripting" FILES ${PROJECT_NAME} ${SCRIPTING_SRC})
SOURCE_GROUP("Utilities" FILES ${PROJECT_NAME} ${UTILITIES_SRC})

SOURCE_GROUP("3rdParty\\STB" FILES ${PROJECT_NAME} ${STB_SRC})
SOURCE_GROUP("3rdParty\\Glad" FILES ${PROJECT_NAME} ${GLAD_SRC})
SOURCE_GROUP("3rdParty\\ImGui" FILES ${PROJECT_NAME} ${IMGUI_SRC})

# Add source to executables
ADD_LIBRARY(
	${PROJECT_NAME}
	${AI_SRC}
	${AUDIO_SRC}
	${CORE_SRC}
	${GAME_SRC}
	${GRAPHICS_SRC}
	${INPUT_SRC}
	${INTERFACE_SRC}
	${NETWORK_SRC}
	${PHYSICS_SRC}
	${SCRIPTING_SRC}
	${UTILITIES_SRC}
	${STB_SRC}
	${GLAD_SRC}
	${IMGUI_SRC}
	${RESOURCES}
)

ADD_DEPENDENCIES(${PROJECT_NAME} depend)

# Configuration-independant libraries
TARGET_LINK_LIBRARIES(
    ${PROJECT_NAME}
    ${COMMON_LIBS}
)

# Debug-only libraries
FOREACH(debug_lib IN ITEMS ${COMMON_LIBS_DEBUG})
    TARGET_LINK_LIBRARIES(
        ${PROJECT_NAME}
        debug ${debug_lib}
    )
ENDFOREACH()

# Release-only libraries
FOREACH(release_lib IN ITEMS ${COMMON_LIBS_RELEASE})
    TARGET_LINK_LIBRARIES(
        ${PROJECT_NAME}
        optimized ${release_lib}
    )
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

SET_PROPERTY(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "HelloWorld")
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
