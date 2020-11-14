
SUBDIRLIST(DEMO_LIST ${CMAKE_CURRENT_SOURCE_DIR})
SET(RUN_DEMO_LIST "")

FOREACH(demo ${DEMO_LIST})
    ADD_SUBDIRECTORY(${demo})

    ADD_DEPENDENCIES(
        ${demo}
        ${PROJECT_NAME}
    )
    SET_TARGET_PROPERTIES(
        ${demo}
        PROPERTIES FOLDER "Demos"
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
    IF(NOT DEFINED ENV{CI})
        ADD_CUSTOM_TARGET(
            run-${demo}
            COMMAND $<TARGET_FILE:${demo}>
            DEPENDS ${demo}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
        SET_TARGET_PROPERTIES(
            run-${demo}
            PROPERTIES FOLDER "Automation"
        )
        ADD_CUSTOM_TARGET(
            valgrind-${demo}
            COMMAND valgrind $<TARGET_FILE:${demo}>
            DEPENDS ${demo}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
        SET_TARGET_PROPERTIES(
            valgrind-${demo}
            PROPERTIES FOLDER "Automation"
        )
    ENDIF()
    LIST(APPEND RUN_DEMO_LIST "run-${demo}")
ENDFOREACH()

ADD_CUSTOM_TARGET(
    demos
    DEPENDS ${DEMO_LIST}
)
SET_TARGET_PROPERTIES(
    demos
    PROPERTIES FOLDER "Demos"
)

IF(NOT DEFINED ENV{CI})
    ADD_CUSTOM_TARGET(
        run-demos
        DEPENDS ${RUN_DEMO_LIST}
    )
    SET_TARGET_PROPERTIES(
        run-demos
        PROPERTIES FOLDER "Automation"
    )
ENDIF()

SET(DEMO_LIST "${DEMO_LIST}" PARENT_SCOPE)
