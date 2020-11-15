INCLUDE(SetSourceGroups)

MACRO(DEMO _target)
    FILE(
        GLOB_RECURSE
        _sources
        Source/*.h
        Source/*.hpp
        Source/*.c
        Source/*.cpp
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

    ADD_EXECUTABLE(
        ${_target}
        ${_sources}
        ${_assets}
    )

    TARGET_LINK_LIBRARIES(
        ${_target}
        PRIVATE
            TemporalityEngine
    )

    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_sources}")
    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_assets}")

    TARGET_INCLUDE_DIRECTORIES(
        ${_target}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/Source
    )

    TARGET_COMPILE_DEFINITIONS(
        ${_target}
        PUBLIC
        # Disable VS "not secure" warnings
        $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
    )

    TARGET_COMPILE_OPTIONS(
        ${_target}
        PUBLIC
            # Disable unknown pragmas warning, C++ exceptions
            $<$<CXX_COMPILER_ID:GNU>:   -Wall -Wno-unknown-pragmas -fno-exceptions>
            $<$<CXX_COMPILER_ID:Clang>: -Wall -Wno-unknown-pragmas -fno-exceptions>
            $<$<CXX_COMPILER_ID:MSVC>:  /MP /wd4068 /EHsc- /GR->
    )

    SET_TARGET_PROPERTIES(
        ${_target}
        PROPERTIES
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
    )

    FILE(RELATIVE_PATH folder ${CMAKE_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/..")
    SET_TARGET_PROPERTIES(
        ${_target}
        PROPERTIES
            FOLDER "${folder}"
    )

    SET_TARGET_PROPERTIES(
        ${_target}
        PROPERTIES
            VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${RUNTIME_SEARCH_PATH};$<$<CONFIG:Debug>:${RUNTIME_SEARCH_PATH_DEBUG}>;$<$<CONFIG:Release>:${RUNTIME_SEARCH_PATH_RELEASE}>"
    )

    STRING(JOIN ":" LD_LIBRARY_PATH ${RUNTIME_SEARCH_PATH})

    IF(NOT MSVC)
        ADD_CUSTOM_TARGET(
            run-${_target}
            COMMAND ${CMAKE_COMMAND} -E env "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}" $<TARGET_FILE:${_target}>
            DEPENDS ${_target}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )

        SET_TARGET_PROPERTIES(
            run-${_target}
            PROPERTIES
                FOLDER "Automation"
        )

        IF(gdb_COMMAND)
            ADD_CUSTOM_TARGET(
                gdb-${_target}
                COMMAND ${CMAKE_COMMAND} -E env "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}" gdb --args $<TARGET_FILE:${_target}>
                DEPENDS ${_target}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            )

            SET_TARGET_PROPERTIES(
                gdb-${_target}
                PROPERTIES
                    FOLDER "Automation"
            )
        ENDIF()

        IF(valgrind_COMMAND)
            ADD_CUSTOM_TARGET(
                valgrind-${_target}
                COMMAND ${CMAKE_COMMAND} -E env "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}" valgrind $<TARGET_FILE:${_target}>
                DEPENDS ${_target}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            )

            SET_TARGET_PROPERTIES(
                valgrind-${_target}
                PROPERTIES
                    FOLDER "Automation"
            )
        ENDIF()
    ENDIF()
ENDMACRO()