
INCLUDE(Module)

SET(_target TemporalityEngine)

MODULE(${_target} ENGINE)

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        glm::glm
        GSL::GSL
        nlohmann_json::nlohmann_json
        ${CMAKE_DL_LIBS}
)

LIST(APPEND ASSET_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/Assets/
)

SET(ASSET_PATH ${ASSET_PATH} PARENT_SCOPE)