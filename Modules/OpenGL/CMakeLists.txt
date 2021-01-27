
SET(_target TemporalityOpenGL)

DEFINE_MODULE(${_target} OPENGL)

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        TemporalitySDL2
)