
SET(_target HelloWorld)

DEMO(${_target})

ADD_DEPENDENCIES(
    ${_target}
    TemporalityOpenGL
)