
INCLUDE(CompileShaders)
INCLUDE(SetSourceGroups)

MACRO(DEFINE_DEMO _target)
    FILE(GLOB_RECURSE
        _sources
        "Source/*.h"
        "Source/*.hpp"
        "Source/*.c"
        "Source/*.cpp"
    )

    FILE(GLOB_RECURSE
        _sources_in
        "Source/*.in"
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
    ### Shader Processing
    ###

    FILE(
        GLOB_RECURSE
        _shaders_in
        Assets/Shaders/*.glsl
        #Assets/Shaders/*.hlsl
    )

    LIST(INSERT TOON_ASSET_PATH 0
        ${CMAKE_CURRENT_SOURCE_DIR}/Assets
        ${CMAKE_CURRENT_BINARY_DIR}/Assets
    )

    COMPILE_SHADERS("${_shaders_in}" _shaders_out)

    ###
    ### Asset Processing
    ###

    FILE(GLOB_RECURSE
        _assets
        Assets/*
    )

    LIST(INSERT TOON_ASSET_PATH 0
        ${CMAKE_CURRENT_SOURCE_DIR}/Assets/
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

    ADD_EXECUTABLE(
        ${_target}
        ${_sources}
        ${_sources_in}
        ${_sources_out}
        ${_shaders_in}
        ${_shaders_out}
        ${_assets}
    )

    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_sources}")
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_sources_in}")
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_BINARY_DIR} "${_sources_out}")
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_shaders_in}")
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_shaders_out}")
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_assets}")

    TARGET_LINK_LIBRARIES(
        ${_target}
        PRIVATE
            ToonEngine
    )

    TARGET_INCLUDE_DIRECTORIES(
        ${_target}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/Source
            ${CMAKE_CURRENT_BINARY_DIR}/Source
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
    )

    FILE(RELATIVE_PATH folder ${CMAKE_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/..")
    SET_TARGET_PROPERTIES(
        ${_target}
        PROPERTIES
            FOLDER "${folder}"
    )

    STRING(REPLACE " " "\\ " TOON_MODULE_PATH "${TOON_MODULE_PATH}")
    STRING(REPLACE " " "\\ " TOON_ASSET_PATH "${TOON_ASSET_PATH}")

    # TODO: Improve
    SET(_ext "")
    IF(WIN32)
        SET(_ext ".exe")
    ENDIF()

    FILE(RELATIVE_PATH _executable ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/${_target}${_ext})
    FILE(TO_NATIVE_PATH ${_executable} _executable)

    EXECUTE_PROCESS(
        COMMAND ${Python3_EXECUTABLE}
            ${CMAKE_SOURCE_DIR}/Scripts/generate-launch-targets.py
            ${CMAKE_CURRENT_SOURCE_DIR}/${_target}.toonproj
            ${CMAKE_BINARY_DIR}
            ${_executable}
            "${TOON_ASSET_PATH}"
            "${TOON_MODULE_PATH}"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
ENDMACRO()