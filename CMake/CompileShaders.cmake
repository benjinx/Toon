
MACRO(COMPILE_SHADERS shaders_in shaders_out)
    IF(VulkanSDK_FOUND)
        FOREACH(file ${shaders_in})
            GET_FILENAME_COMPONENT(shader_ext ${file} LAST_EXT)
            IF(shader_ext STREQUAL ".glsl")
                GET_FILENAME_COMPONENT(shader_name ${file} NAME_WLE)
                GET_FILENAME_COMPONENT(shader_path ${file} DIRECTORY)
                STRING(REPLACE
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    shader_out_path
                    ${shader_path}
                )

                FILE(MAKE_DIRECTORY ${shader_out_path})

                SET(shader_cflags "")
                FOREACH(dir ${TOON_ASSET_PATH})
                    SET(shader_cflags ${shader_cflags} -I"${dir}/Shaders/")
                ENDFOREACH()

                SET(shader_out "${shader_out_path}/${shader_name}.spv")

                GET_FILENAME_COMPONENT(shader_type ${shader_name} LAST_EXT)
                STRING(SUBSTRING ${shader_type} 1 -1 shader_type)

                IF(shader_type STREQUAL "inc")
                    CONTINUE()
                ELSE()
                    SET(shader_cflags ${shader_cflags} -fshader-stage=${shader_type})
                ENDIF()

                ADD_CUSTOM_COMMAND(
                    OUTPUT ${shader_out}
                    COMMAND ${VulkanSDK_glslc_PROGRAM} ${shader_cflags} -o ${shader_out} ${file}
                    DEPENDS ${file}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                )

                LIST(APPEND ${shaders_out} ${shader_out})
            ENDIF()
        ENDFOREACH()
    ENDIF()
ENDMACRO()