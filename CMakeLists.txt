CMAKE_MINIMUM_REQUIRED(VERSION 3.2 FATAL_ERROR)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# Initialize Projects
project(Temporality)

add_subdirectory(depend)

# Allow for custom organization of files in VisualStudio
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# Adding Defines

if(CMAKE_COMPILER_IS_GNUCC OR ${CMAKE_C_COMPILER_ID} MATCHES "Clang")
    set(CMAKE_CXX_FLAGS " -std=c++11 -Wall -D_XOPEN_SOURCE=700")
elseif(MSVC)
endif()

find_package(OpenGL REQUIRED)

include_directories(BEFORE SYSTEM
    ${OPENGL_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
	${SOL2_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIRS}
    ${OPENAL_INCLUDE_DIRS}
	${ASSIMP_INCLUDE_DIRS}
)

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

# Source files
SET(COMMON_SRC
)

# Header files
SET(COMMON_H
)

# Core src
SET(CORE_SRC
	src/Application.cpp
	src/Application.h
	src/Camera.cpp
	src/Camera.h
	src/Common.h
	src/Cube.cpp
	src/Cube.h
	src/GameObject.cpp
	src/GameObject.h
	src/GameScene.cpp
	src/GameScene.h
	src/Main.cpp
	src/Scene.cpp
	src/Scene.h
	src/Window.cpp
	src/Window.h
)

# Debug src
SET(DEBUG_SRC
	src/Utils.cpp
	src/Utils.h
)

# Graphics src
SET(GRAPHICS_SRC
	src/Material.cpp
	src/Material.h
	src/Mesh.cpp
	src/Mesh.h
	src/Shader.cpp
	src/Shader.h
)

# Physics src
SET(PHYSICS_SRC
	src/Collision.cpp
	src/Collision.h
	src/Rigidbody.cpp
	src/Rigidbody.h
)

# Scripting src
SET(SCRIPTING_SRC
	src/ScriptHost.cpp
	src/ScriptHost.h
)

# Audio src
SET(AUDIO_SRC
)

# Animation src
SET(ANIMATION_SRC
)

# UI src
SET(UI_SRC
	src/UI.cpp
	src/UI.h
)

# Navigation/AI src
SET(AI_SRC
	src/State.cpp
	src/State.h
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

SET(TINY_OBJ_SRC
    src/tiny_obj_loader/tiny_obj_loader.h
)

SET(STB_SRC
    src/stb/stb_image.h
)

FILE(GLOB_RECURSE RESOURCES RELATIVE ${CMAKE_SOURCE_DIR} "Resources/*")
FOREACH(res IN ITEMS ${RESOURCES})
    GET_FILENAME_COMPONENT(path "${res}" DIRECTORY)
    FILE(TO_NATIVE_PATH ${path} path)
    SOURCE_GROUP("${path}" FILES ${res})
ENDFOREACH()
# Stop VS from trying to compile .obj files
SET_SOURCE_FILES_PROPERTIES(${RESOURCES} PROPERTIES HEADER_FILE_ONLY true)

SOURCE_GROUP("Source" FILES ${PROJECT_NAME} ${COMMON_SRC})
SOURCE_GROUP("Headers" FILES ${PROJECT_NAME} ${COMMON_H})
SOURCE_GROUP("Core" FILES ${PROJECT_NAME} ${CORE_SRC})
SOURCE_GROUP("Debug" FILES ${PROJECT_NAME} ${DEBUG_SRC})
SOURCE_GROUP("Graphics" FILES ${PROJECT_NAME} ${GRAPHICS_SRC})
SOURCE_GROUP("Physics" FILES ${PROJECT_NAME} ${PHYSICS_SRC})
SOURCE_GROUP("Scripting" FILES ${PROJECT_NAME} ${SCRIPTING_SRC})
SOURCE_GROUP("Audio" FILES ${PROJECT_NAME} ${AUDIO_SRC})
SOURCE_GROUP("Animation" FILES ${PROJECT_NAME} ${ANIMATION_SRC})
SOURCE_GROUP("UI" FILES ${PROJECT_NAME} ${UI_SRC})
SOURCE_GROUP("AI/Navigation" FILES ${PROJECT_NAME} ${AI_SRC})
SOURCE_GROUP("3rdParty\\STB" FILES ${PROJECT_NAME} ${STB_SRC})
SOURCE_GROUP("3rdParty\\Glad" FILES ${PROJECT_NAME} ${GLAD_SRC})
SOURCE_GROUP("3rdParty\\ImGui" FILES ${PROJECT_NAME} ${IMGUI_SRC})
SOURCE_GROUP("3rdParty\\TinyObjLoader" FILES ${PROJECT_NAME} ${TINY_OBJ_SRC})

# Add source to executables
add_executable(
	${PROJECT_NAME}
	${COMMON_SRC}
	${COMMON_H}
	${CORE_SRC}
	${DEBUG_SRC}
	${GRAPHICS_SRC}
	${PHYSICS_SRC}
	${SCRIPTING_SRC}
	${AUDIO_SRC}
	${ANIMATION_SRC}
	${UI_SRC}
	${AI_SRC}
	${STB_SRC}
	${GLAD_SRC}
	${IMGUI_SRC}
	${TINY_OBJ_SRC}
	${RESOURCES}
)

include_directories(${CMAKE_SOURCE_DIR}/src)
add_dependencies(${PROJECT_NAME} depend)

# Configuration-independant libraries
target_link_libraries(
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

add_custom_target(
    run
    COMMAND $<TARGET_FILE:${PROJECT_NAME}>
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
set_target_properties(
    run
    PROPERTIES FOLDER "Automation"
)

# Copy resources to build directory
ADD_CUSTOM_TARGET(copy-resources ALL)
ADD_DEPENDENCIES(${PROJECT_NAME} copy-resources)
ADD_CUSTOM_COMMAND(
    OUTPUT
    TARGET copy-resources PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E
        copy_directory ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources
)

SET_TARGET_PROPERTIES(
    copy-resources
    PROPERTIES FOLDER "Automation"
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
