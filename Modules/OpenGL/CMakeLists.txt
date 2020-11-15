
SET(_target TemporalityOpenGL)

MODULE(${_target} OPENGL)

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        SDL2::SDL2
)