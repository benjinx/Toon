
SET(_target ToonSDL2)

DEFINE_MODULE(${_target} SDL2)

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        SDL2::SDL2
)