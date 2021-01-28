
SET(_target HelloWorld)

DEFINE_DEMO(${_target})

ADD_DEPENDENCIES(
    ${_target}
    ToonOpenGL
    ToonSTBI
    ToonTinyOBJ
)