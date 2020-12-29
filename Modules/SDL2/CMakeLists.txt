
SET(_target TemporalitySDL2)

MODULE(${_target} SDL2)

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        SDL2::SDL2
)