
INCLUDE(DefineModule)
INCLUDE(CompileShaders)
INCLUDE(SetSourceGroups)

SET(_target ToonEngine)

DEFINE_MODULE(${_target} ENGINE)

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        glm::glm
        GSL::GSL
        nlohmann_json::nlohmann_json
        cflags::cflags
        ${CMAKE_DL_LIBS}
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

LIST(APPEND TOON_ASSET_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/Assets
    ${CMAKE_CURRENT_BINARY_DIR}/Assets
)

SET(TOON_ASSET_PATH ${TOON_ASSET_PATH} PARENT_SCOPE)

COMPILE_SHADERS("${_shaders_in}" _shaders_out)

TARGET_SOURCES(
    ${_target}
    PRIVATE
        ${_shaders_in}
        ${_shaders_out}
)

SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_shaders_in}")
SET_SOURCE_GROUPS(${CMAKE_CURRENT_BINARY_DIR} "${_shaders_out}")