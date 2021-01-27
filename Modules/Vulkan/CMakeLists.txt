
SET(_target TemporalityVulkan)

DEFINE_MODULE(${_target} VULKAN)

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        TemporalitySDL2
)