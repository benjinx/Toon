cmake_minimum_required(VERSION 3.8)

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
    set(CMAKE_CXX_FLAGS "/NODEFAULTLIB:MSVCRT")
endif()

find_package(OpenGL REQUIRED)

include_directories(BEFORE SYSTEM
    ${OPENGL_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIRS}
)

SET(COMMON_LIBS
    ${OPENGL_LIBRARIES}
    ${LUA_LIBRARIES}
    ${GLFW_LIBRARIES}
)
SET(COMMON_LIBS_DEBUG
    ${LUA_LIBRARIES_DEBUG}
    ${GLFW_LIBRARIES_DEBUG}
)
SET(COMMON_LIBS_RELEASE
    ${LUA_LIBRARIES_RELEASE}
    ${GLFW_LIBRARIES_RELEASE}
)

# Source files
SET(COMMON_SRC
	src/Application.cpp
	src/Camera.cpp
	src/Main.cpp
	src/Material.cpp
	src/Mesh.cpp
	src/Model.cpp
	src/Shader.cpp
	src/UI.cpp
	src/Utils.cpp
	src/Window.cpp
)

# Header files
SET(COMMON_H
	src/Application.h
	src/Camera.h
    src/Common.h
	src/Material.h
	src/Mesh.h
	src/Model.h
	src/Shader.h
	src/UI.h
	src/Utils.h
	src/Window.h
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

SOURCE_GROUP("Source" FILES ${PROJECT_NAME} ${COMMON_SRC})
SOURCE_GROUP("Headers" FILES ${PROJECT_NAME} ${COMMON_H})
SOURCE_GROUP("3rdParty\\STB" FILES ${PROJECT_NAME} ${STB_SRC})
SOURCE_GROUP("3rdParty\\Glad" FILES ${PROJECT_NAME} ${GLAD_SRC})
SOURCE_GROUP("3rdParty\\ImGui" FILES ${PROJECT_NAME} ${IMGUI_SRC})
SOURCE_GROUP("3rdParty\\TinyObjLoader" FILES ${PROJECT_NAME} ${TINY_OBJ_SRC})

# Add source to executables
add_executable(
	${PROJECT_NAME}
	${COMMON_SRC}
	${COMMON_H}
    ${STB_SRC}
	${GLAD_SRC}
	${IMGUI_SRC}
	${TINY_OBJ_SRC}
)

include_directories(${CMAKE_SOURCE_DIR}/src)
add_dependencies(${PROJECT_NAME} depend)

# Configuration-independant libraries
target_link_libraries(
    ${PROJECT_NAME}
    ${COMMON_LIBS}
)

# Debug-only libraries
foreach(debug_lib IN ITEMS ${COMMON_LIBS_DEBUG})
    target_link_libraries(
        ${PROJECT_NAME}
        debug ${debug_lib}
    )
endforeach()

# Release-only libraries
foreach(release_lib IN ITEMS ${COMMON_LIBS_RELEASE})
    target_link_libraries(
        ${PROJECT_NAME}
        optimized ${release_lib}
    )
endforeach()

add_custom_target(
    run
    COMMAND ${PROJECT_BINARY_DIR}/${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
set_target_properties(
    run
    PROPERTIES FOLDER "Automation"
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
