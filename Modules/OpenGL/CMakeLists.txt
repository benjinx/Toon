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