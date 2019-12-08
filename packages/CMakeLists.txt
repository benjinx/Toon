
FIND_PROGRAM(
    TAR_EXECUTABLE
    tar
)

IF (TAR_EXECUTABLE)
    SET(_package "libTemporality-${PROJECT_VERSION}")

    FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${_package})
    
    ADD_CUSTOM_COMMAND(
        OUTPUT "${_package}.tgz"
        DEPENDS ${PROJECT_NAME}
        COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX="${CMAKE_CURRENT_BINARY_DIR}/${_package}" ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target install
        COMMAND ${TAR_EXECUTABLE} -cvzf "${CMAKE_CURRENT_BINARY_DIR}/${_package}.tgz" "${CMAKE_CURRENT_BINARY_DIR}/${_package}/*"
    )

    ADD_CUSTOM_TARGET(
        tgz
        DEPENDS "${_package}.tgz"
    )
ENDIF()

FIND_PROGRAM(
    DPKG_DEB_EXECUTABLE
    dpkg-deb
)

IF (DPKG_DEB_EXECUTABLE)
    SET(_package "libTemporality-dev_${PROJECT_VERSION}-1")

    FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${_package}/DEBIAN)

    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/DEBIAN/control.in
        ${CMAKE_CURRENT_BINARY_DIR}/${_package}/DEBIAN/control
        @ONLY
    )
    
    ADD_CUSTOM_COMMAND(
        OUTPUT "${_package}.deb"
        DEPENDS ${PROJECT_NAME}
        COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX="${CMAKE_CURRENT_BINARY_DIR}/${_package}/usr/local" ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target install
        COMMAND ${DPKG_DEB_EXECUTABLE} --build "${CMAKE_CURRENT_BINARY_DIR}/${_package}"
    )

    ADD_CUSTOM_TARGET(
        deb
        DEPENDS "${_package}.deb"
    )
ENDIF()
