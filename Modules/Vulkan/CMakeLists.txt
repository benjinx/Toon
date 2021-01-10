
SET(_target TemporalityVulkan)

MODULE(${_target} VULKAN)

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        TemporalitySDL2
)