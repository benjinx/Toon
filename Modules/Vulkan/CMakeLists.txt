
SET(_target ToonVulkan)

DEFINE_MODULE(${_target} VULKAN)

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        ToonSDL2
)