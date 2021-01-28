
SET(_target ToonOpenGL)

DEFINE_MODULE(${_target} OPENGL)

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        ToonSDL2
)