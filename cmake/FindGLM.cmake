# FindGLM.cmake
#
# Finds the OpenGL Mathematics library
#
# This will define the following variables
#
#   GLM_FOUND
#   GLM_INCLUDE_DIR
#
# and the following imported targets
#
#   GLM::GLM
#
# The following variables can be set as arguments
#
#   GLM_ROOT_DIR
#

FIND_PACKAGE(PkgConfig QUIET)
PKG_CHECK_MODULES(_GLM_PC QUIET glm)

FIND_PATH(
    GLM_INCLUDE_DIR
    NAMES glm/glm.hpp
    PATHS ${GLM_ROOT_DIR} ${_GLM_PC_INCLUDE_DIRS} 
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    GLM
    REQUIRED_VARS GLM_INCLUDE_DIR
)

IF(GLM_FOUND)
    ADD_LIBRARY(GLM::GLM INTERFACE IMPORTED)
    SET_TARGET_PROPERTIES(
        GLM::GLM PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${GLM_INCLUDE_DIR}"
    )
ENDIF()