CMAKE_MINIMUM_REQUIRED(VERSION 3.2 FATAL_ERROR)

PROJECT(thirdparty)

LIST(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/../cmake")

INCLUDE(ExternalProject)

# Load Dependencies

# glm
FIND_PACKAGE(GLM QUIET)

IF (NOT GLM_FOUND)
	SET(_GLM_DIR "${CMAKE_BINARY_DIR}/glm")

	# glm
	ExternalProject_Add(thirdparty-glm
		GIT_REPOSITORY      "https://github.com/g-truc/glm.git"
		GIT_TAG             "0.9.9.3"
		GIT_SHALLOW         TRUE
		SOURCE_DIR          "${_GLM_DIR}"
		CONFIGURE_COMMAND   ""
		BUILD_COMMAND      	""
		UPDATE_COMMAND      ""
		INSTALL_COMMAND     ""
		TEST_COMMAND        ""
	)
ENDIF()

# glfw
FIND_PACKAGE(GLFW3 QUIET)

IF (NOT GLFW3_FOUND)
    SET(_GLFW3_DIR "${CMAKE_BINARY_DIR}/glfw3")
    SET(_GLFW3_SOURCE_DIR "${CMAKE_BINARY_DIR}/glfw3_src")
    SET(_GLFW3_BINARY_DIR "${CMAKE_BINARY_DIR}/glfw3_bin")

    ExternalProject_Add(thirdparty-glfw
        GIT_REPOSITORY      "https://github.com/glfw/glfw"
        GIT_TAG             "3.2.1"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          "${_GLFW3_SOURCE_DIR}"
        BINARY_DIR          "${_GLFW3_BINARY_DIR}"
        CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${_GLFW3_DIR} 
                            -DGLFW_BUILD_EXAMPLES=OFF
                            -DGLFW_BUILD_TESTS=OFF
                            -DGLFW_BUILD_DOCS=OFF
                            -DGLFW_INSTALL=ON
                            -DBUILD_SHARED_LIBS=ON
        UPDATE_COMMAND      ""
        BUILD_COMMAND       ""
        INSTALL_COMMAND     ${CMAKE_COMMAND} --build . --target install --config Release
        TEST_COMMAND        ""
    )
ENDIF()
