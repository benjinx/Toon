
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

ADD_LIBRARY(Temporality SHARED ${_public} ${_private})

TARGET_COMPILE_FEATURES(
    Temporality
    PRIVATE
        cxx_std_17
)

TARGET_INCLUDE_DIRECTORIES(
    Temporality
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Private
)

TARGET_LINK_LIBRARIES(
    Temporality
    PUBLIC
        glm::glm
        ${CMAKE_DL_LIBS}
)