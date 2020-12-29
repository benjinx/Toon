
INCLUDE(SetSourceGroups)

MACRO(MODULE _target _prefix)
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
        ${_public}
        ${_private}
    )

    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_public}")
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_private}")
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_assets}")

    IF(NOT _target STREQUAL "TemporalityEngine")
        TARGET_LINK_LIBRARIES(
            ${_target}
            PRIVATE
                TemporalityEngine
        )
    ENDIF()

    TARGET_INCLUDE_DIRECTORIES(
        ${_target}
        PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/Private
    )

    STRING(LENGTH "${CMAKE_SOURCE_DIR}/" SOURCE_PATH_LENGTH)

    TARGET_COMPILE_DEFINITIONS(
        ${_target}
        PUBLIC
            # Disable VS "not secure" warnings
            $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
        PRIVATE
            TEMPORALITY_SOURCE_PATH_LENGTH=${SOURCE_PATH_LENGTH}
    )

    TARGET_COMPILE_OPTIONS(
        ${_target}
        PUBLIC
            # Configure VS to use C++20
            $<$<CXX_COMPILER_ID:MSVC>: /std:c++latest>

            # Disable unknown pragmas warning, C++ exceptions
            $<$<CXX_COMPILER_ID:GNU>:   -Wall -Wno-unknown-pragmas -fno-exceptions>
            $<$<CXX_COMPILER_ID:Clang>: -Wall -Wno-unknown-pragmas -fno-exceptions>
            $<$<CXX_COMPILER_ID:MSVC>:  /MP /wd4068 /EHsc- /GR->
    )

    SET_TARGET_PROPERTIES(
        ${_target}
        PROPERTIES
            DEFINE_SYMBOL "TEMPORALITY_${_prefix}_EXPORT"
    )

    FILE(RELATIVE_PATH folder ${CMAKE_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/..")
    SET_TARGET_PROPERTIES(
        ${_target}
        PROPERTIES
            FOLDER "${folder}"
    )

    IF(MSVC)
        LIST(APPEND RUNTIME_SEARCH_PATH "${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)")
    ELSE()
        LIST(APPEND RUNTIME_SEARCH_PATH "${CMAKE_CURRENT_BINARY_DIR}")
    ENDIF()

    SET(RUNTIME_SEARCH_PATH ${RUNTIME_SEARCH_PATH} PARENT_SCOPE)

ENDMACRO()