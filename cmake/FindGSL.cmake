# FindGSL.cmake
#
# Finds Microsoft's Guidlines Support Library
#
# This will define the following variables
#
#   GSL_FOUND
#   GSL_INCLUDE_DIR
#
# and the following imported targets
#
#   GSL::GSL
#
# The following variables can be set as arguments
#
#   GSL_ROOT_DIR
#

FIND_PATH(
    GSL_INCLUDE_DIR
    NAMES gsl/gsl
    PATHS 
        ${GSL_ROOT_DIR}
    PATH_SUFFIXES 
        include
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    GSL
    REQUIRED_VARS 
        GSL_INCLUDE_DIR
)

IF(GSL_FOUND)
    ADD_LIBRARY(GSL::GSL INTERFACE IMPORTED)
    SET_TARGET_PROPERTIES(
        GSL::GSL PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${GSL_INCLUDE_DIR}"
    )
ENDIF()