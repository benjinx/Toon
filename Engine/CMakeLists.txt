
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

SET_TARGET_PROPERTIES(
    Temporality PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

TARGET_COMPILE_OPTIONS(
    Temporality
    PUBLIC
        # Disable unknown pragmas warning, C++ exceptions
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wno-unknown-pragmas -fno-exceptions>
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Wno-unknown-pragmas -fno-exceptions>
        $<$<CXX_COMPILER_ID:MSVC>:/MP /wd4068 /EHsc- /GR- /std:c++17>
)

IF(MSVC)
    SET_TARGET_PROPERTIES(
        Temporality
        PROPERTIES 
            VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${RUNTIME_SEARCH_PATH}"
            # TODO SHADER_INCLUDE_PATH
    )
ELSE()
ENDIF()

TARGET_COMPILE_DEFINITIONS(
    Temporality
    PUBLIC
        # Disable Visual Studio "not secure" warnings
        $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
    PRIVATE
        TEMPORALITY_ENGINE_EXPORT
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

IF(MSVC)
    LIST(APPEND RUNTIME_SEARCH_PATH "${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)")
ELSE()
    LIST(APPEND RUNTIME_SEARCH_PATH "${CMAKE_CURRENT_BINARY_DIR}")
ENDIF()

SET(RUNTIME_SEARCH_PATH ${RUNTIME_SEARCH_PATH} PARENT_SCOPE)