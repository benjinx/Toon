
INCLUDE(CompileShaders)
INCLUDE(SetSourceGroups)

MACRO(DEFINE_DEMO _target)
    FILE(
        GLOB_RECURSE
        _sources
        Source/*.h
        Source/*.hpp
        Source/*.c
        Source/*.cpp
    )

    ###
    ### Shader Processing
    ###

    FILE(
        GLOB_RECURSE
        _shaders_in
        Assets/Shaders/*.glsl
        #Assets/Shaders/*.hlsl
    )

    LIST(INSERT ASSET_PATH 0
        ${CMAKE_CURRENT_SOURCE_DIR}/Assets
        ${CMAKE_CURRENT_BINARY_DIR}/Assets
    )

    ##COMPILE_SHADERS("${_shaders_in}" _shaders_out)

    ###
    ### Asset Processing
    ###

    FILE(GLOB_RECURSE
        _assets
        Assets/*
    )

    LIST(INSERT ASSET_PATH 0
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
        ${_shaders_in}
        ${_shaders_out}
        ${_assets}
    )

    SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_sources}")
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
            $<$<CXX_COMPILER_ID:GNU>:   -Wall -Wno-unknown-pragmas -fno-exceptions>
            $<$<CXX_COMPILER_ID:Clang>: -Wall -Wno-unknown-pragmas -fno-exceptions>
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

    IF(MSVC)
        SET_TARGET_PROPERTIES(
            ${_target}
            PROPERTIES
                VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                VS_DEBUGGER_ENVIRONMENT "PATH=${RUNTIME_SEARCH_PATH};$<$<CONFIG:Debug>:${RUNTIME_SEARCH_PATH_DEBUG};>$<$<CONFIG:Release>:${RUNTIME_SEARCH_PATH_RELEASE};>$ENV{PATH}\nTOON_ASSET_PATH=${ASSET_PATH}"
        )

        ADD_CUSTOM_TARGET(
            run-${_target}
            COMMAND ${CMAKE_COMMAND} -E env "PATH=${RUNTIME_SEARCH_PATH};$<$<CONFIG:Debug>:${RUNTIME_SEARCH_PATH_DEBUG};>$<$<CONFIG:Release>:${RUNTIME_SEARCH_PATH_RELEASE};>$ENV{PATH}" "TOON_ASSET_PATH=${ASSET_PATH}" $<TARGET_FILE:${_target}>
            DEPENDS ${_target}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )

        SET_TARGET_PROPERTIES(
            run-${_target}
            PROPERTIES 
                FOLDER "Automation"
        )

        # Append debug target to .vscode/launch.json if .vscode/ exists
        IF(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/.vscode AND Python3_Interpreter_FOUND)
            SET(_launch_json ${CMAKE_SOURCE_DIR}/.vscode/launch.json)
            SET(_graphics_drivers "")

            IF("OpenGL" IN_LIST REQUIRED_MODULES)
                LIST(APPEND _graphics_drivers "OpenGL")
            ENDIF()

            IF("Vulkan" IN_LIST REQUIRED_MODULES)
                LIST(APPEND _graphics_drivers "Vulkan")
            ENDIF()

            IF("DirectX" IN_LIST REQUIRED_MODULES)
                LIST(APPEND _graphics_drivers "DirectX")
            ENDIF()

            FOREACH(_driver ${_graphics_drivers})
                ADD_CUSTOM_COMMAND(
                    TARGET ${_target} POST_BUILD
                    BYPRODUCTS ${_launch_json}
                    COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Scripts/add-vscode-launch-target.py ${_launch_json} "${_target} (${_driver}, $<CONFIG>)" $<TARGET_FILE:${_target}> ${CMAKE_CURRENT_BINARY_DIR} "PATH=${RUNTIME_SEARCH_PATH};$<$<CONFIG:Debug>:${RUNTIME_SEARCH_PATH_DEBUG};>$<$<CONFIG:Release>:${RUNTIME_SEARCH_PATH_RELEASE};>$ENV{PATH}" "TOON_ASSET_PATH=${ASSET_PATH}" "TOON_GRAPHICS_DRIVER=${_driver}"
                )
            ENDFOREACH()
        ENDIF()
    ELSE()

        STRING(JOIN ":" LD_LIBRARY_PATH ${RUNTIME_SEARCH_PATH})
        STRING(JOIN ":" ASSET_PATH ${ASSET_PATH})

        ADD_CUSTOM_TARGET(
            run-${_target}
            COMMAND ${CMAKE_COMMAND} -E env "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}" "TOON_ASSET_PATH=${ASSET_PATH}" $<TARGET_FILE:${_target}>
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
                COMMAND ${CMAKE_COMMAND} -E env "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}" "TOON_ASSET_PATH=${ASSET_PATH}" gdb --args $<TARGET_FILE:${_target}>
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
                COMMAND ${CMAKE_COMMAND} -E env "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}" "TOON_ASSET_PATH=${ASSET_PATH}" valgrind $<TARGET_FILE:${_target}>
                DEPENDS ${_target}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            )

            SET_TARGET_PROPERTIES(
                valgrind-${_target}
                PROPERTIES
                    FOLDER "Automation"
            )
        ENDIF()

        IF(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/.vscode)
            SET(_launch_json ${CMAKE_SOURCE_DIR}/.vscode/launch.json)
            SET(_graphics_drivers "")

            IF("OpenGL" IN_LIST REQUIRED_MODULES)
                LIST(APPEND _graphics_drivers "OpenGL")
            ENDIF()

            IF("Vulkan" IN_LIST REQUIRED_MODULES)
                LIST(APPEND _graphics_drivers "Vulkan")
            ENDIF()

            FOREACH(_driver ${_graphics_drivers})
                ADD_CUSTOM_COMMAND(
                    TARGET ${_target} POST_BUILD
                    BYPRODUCTS ${_launch_json}
                    COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Scripts/add-vscode-launch-target.py ${_launch_json} "${_target} \\(${_driver}\\)" $<TARGET_FILE:${_target}> ${CMAKE_CURRENT_BINARY_DIR} "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}" "TOON_ASSET_PATH=${ASSET_PATH}" "TOON_GRAPHICS_DRIVER=${_driver}"
                )
            ENDFOREACH()
        ENDIF()
    ENDIF()
ENDMACRO()