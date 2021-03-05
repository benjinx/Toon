
SET(_target Lighting)

DEFINE_DEMO(${_target})

ADD_DEPENDENCIES(
    ${_target}
    ToonSDL2
    ToonVulkan
    ToonOpenGL
    ToonSTBI
    ToonTinyOBJ
)

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        ToonGLTF2
)