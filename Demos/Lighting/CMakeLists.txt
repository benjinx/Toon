
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