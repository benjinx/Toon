
INCLUDE(DefineModule)

###
### Include Modules
###

FOREACH(_module ${REQUIRED_MODULES})
    ADD_SUBDIRECTORY(${_module})
ENDFOREACH()

# Update the global TOON_ASSET_PATH
SET(TOON_ASSET_PATH ${TOON_ASSET_PATH} PARENT_SCOPE)

# Update the global TOON_MODULE_PATH to include paths to the module's libraries
SET(TOON_MODULE_PATH ${TOON_MODULE_PATH} PARENT_SCOPE)
