
MACRO(SET_SOURCE_GROUPS base_path files)
    FOREACH(file IN ITEMS ${files})
        FILE(RELATIVE_PATH file_path ${base_path} ${file})
        GET_FILENAME_COMPONENT(file_path ${file_path} DIRECTORY)
        FILE(TO_NATIVE_PATH ${file_path} file_path)
        SOURCE_GROUP(${file_path} FILES ${file})
    ENDFOREACH()
ENDMACRO()