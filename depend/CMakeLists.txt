
IF(NOT DEFINED CMAKE_SUPPRESS_DEVELOPER_WARNINGS)
    SET(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")
ENDIF()

ADD_SUBDIRECTORY(glm)
SET(GLM_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/glm" PARENT_SCOPE)

ADD_SUBDIRECTORY(lua)
SET(LUA_LIBRARIES liblua PARENT_SCOPE)
SET(LUA_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/lua/src" "${CMAKE_BINARY_DIR}/depend/lua" PARENT_SCOPE)

ADD_SUBDIRECTORY(glfw)
SET(GLFW_LIBRARIES glfw PARENT_SCOPE)
SET(GLFW_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/glfw/include")

SET(DEPEND_IGNORE
    glm_dummy
    lua wlua luac
    glfwinfo docs uninstall heightmap gears boing particles wave simple splitview reopen monitors iconify 
    gamma icon tearing empty events msaa cursor threads clipboard title joysticks sharing windows timeout
)

SET(DEPEND
    liblua glfw
    ${DEPEND_IGNORE}
)

FOREACH(depend ITEMS ${DEPEND})
    IF(TARGET ${depend})
        SET_TARGET_PROPERTIES(
            ${depend}
            PROPERTIES FOLDER "depend")
    ENDIF()
ENDFOREACH()

FOREACH(depend ITEMS ${DEPEND})
    IF(TARGET ${depend})
        SET_TARGET_PROPERTIES(
            ${depend}
            PROPERTIES
            EXCLUDE_FROM_ALL 1
            EXCLUDE_FROM_DEFAULT_BUILD 1)
    ENDIF()
ENDFOREACH()
