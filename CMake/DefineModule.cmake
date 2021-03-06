
INCLUDE(SetSourceGroups)

MACRO(DEFINE_MODULE _target _prefix)
    FILE(GLOB_RECURSE
        _sources
        "Public/*.h"
        "Public/*.hpp"
        "Private/*.h"
        "Private/*.hpp"
        "Private/*.c"
        "Private/*.cpp"
    )

    FILE(GLOB_RECURSE
        _sources_in
        "Public/*.in"
        "Private/*.in"
    )

    ###
    ### Template Files
    ###

    FOREACH(file ${_sources_in})
        STRING(REPLACE 
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}
            file_out
            ${file}
        )

        string(REGEX MATCH "^(.*)\\.[^.]*$" file_out ${file_out})
        set(file_out ${CMAKE_MATCH_1})

        CONFIGURE_FILE(${file} ${file_out})
        LIST(APPEND _sources_out ${file_out})
    ENDFOREACH()

    ###
    ### Asset Processing
    ###

    FILE(GLOB_RECURSE
        _assets
        Assets/*
    )

    # Stop VS from trying to "build" our .obj model files
    SET_SOURCE_FILES_PROPERTIES(
        ${_assets}
        PROPERTIES
            HEADER_FILE_ONLY TRUE
    )

    ###
    ### Target Configuration
    ###

    ADD_LIBRARY(
        ${_target} SHARED
        ${_sources}
        ${_sources_in}
        ${_sources_out}
    )

    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_sources}")
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_sources_in}")
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_BINARY_DIR} "${_sources_out}")
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_assets}")

    IF(NOT _target STREQUAL "ToonEngine")
        TARGET_LINK_LIBRARIES(
            ${_target}
            PRIVATE
                ToonEngine
        )
    ENDIF()

    TARGET_INCLUDE_DIRECTORIES(
        ${_target}
        PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/Public>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/Private
            ${CMAKE_CURRENT_BINARY_DIR}/Private
    )

    STRING(LENGTH "${CMAKE_SOURCE_DIR}/" SOURCE_PATH_LENGTH)

    TARGET_COMPILE_DEFINITIONS(
        ${_target}
        PUBLIC
            # Disable VS "not secure" warnings
            $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
        PRIVATE
            TOON_SOURCE_PATH_LENGTH=${SOURCE_PATH_LENGTH}
    )

    TARGET_COMPILE_OPTIONS(
        ${_target}
        PRIVATE
            # Configure VS to use C++20, since it ignores CXX_STANDARD
            $<$<CXX_COMPILER_ID:MSVC>: /std:c++latest>

            # Force windows to use UTF-8
            $<$<CXX_COMPILER_ID:MSVC>: /utf-8>

            # Disable unknown pragmas warning, C++ exceptions
            $<$<CXX_COMPILER_ID:GNU>:   -Wall -Wno-unknown-pragmas>
            $<$<CXX_COMPILER_ID:Clang>: -Wall -Wno-unknown-pragmas>
            $<$<CXX_COMPILER_ID:MSVC>:  /MP /wd4068 /EHsc->
    )

    TARGET_LINK_OPTIONS(
        ${_target}
        PUBLIC
            # Fix windows bug in looking for python38.lib
            $<$<CXX_COMPILER_ID:MSVC>:/NODEFAULTLIB:python38.lib>
    )

    SET_TARGET_PROPERTIES(
        ${_target}
        PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
            DEFINE_SYMBOL "TOON_${_prefix}_EXPORT"
    )

    FILE(RELATIVE_PATH folder ${CMAKE_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/..")
    SET_TARGET_PROPERTIES(
        ${_target}
        PROPERTIES
            FOLDER "${folder}"
    )

    LIST(APPEND TOON_MODULE_PATH ${CMAKE_CURRENT_BINARY_DIR})
    SET(TOON_MODULE_PATH ${TOON_MODULE_PATH} PARENT_SCOPE)
    

ENDMACRO()