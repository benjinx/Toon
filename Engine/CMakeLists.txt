
INCLUDE(Module)

SET(_target TemporalityEngine)

MODULE(${_target} ENGINE)

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        glm::glm
        nlohmann_json::nlohmann_json
        ${CMAKE_DL_LIBS}
)
