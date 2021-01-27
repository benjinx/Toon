
SET(_target HelloWorld)

DEFINE_DEMO(${_target})

ADD_DEPENDENCIES(
    ${_target}
    TemporalityOpenGL
    TemporalitySTBI
    TemporalityTinyOBJ
)