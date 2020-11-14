FILE(
    GLOB_RECURSE
    _public
    Public/*.h
    Public/*.hpp
)

FILE(
    GLOB_RECURSE
    _private
    Private/*.h
    Private/*.hpp
    Private/*.c
    Private/*.cpp
)

ADD_LIBRARY(TemporalityOpenGL SHARED ${_public} ${_private})

TARGET_COMPILE_DEFINITIONS(
    TemporalityOpenGL
    PUBLIC
        # Disable Visual Studio "not secure" warnings
        $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
    PRIVATE
        TEMPORALITY_OPENGL_EXPORT
)

TARGET_INCLUDE_DIRECTORIES(
    TemporalityOpenGL
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Private
)

TARGET_LINK_LIBRARIES(
    TemporalityOpenGL
    PUBLIC
        Temporality
        SDL2::SDL2
)

IF(MSVC)
    LIST(APPEND RUNTIME_SEARCH_PATH "${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)")
ELSE()
    LIST(APPEND RUNTIME_SEARCH_PATH "${CMAKE_CURRENT_BINARY_DIR}")
ENDIF()

SET(RUNTIME_SEARCH_PATH ${RUNTIME_SEARCH_PATH} PARENT_SCOPE)