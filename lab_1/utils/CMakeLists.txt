function(add_cpp_module_library TARGET_NAME MODULE_FILE)
    add_library(${TARGET_NAME} STATIC)
    
    target_sources(${TARGET_NAME}
        PUBLIC
        FILE_SET CXX_MODULES
        FILES
        ${MODULE_FILE}
    )
endfunction()

add_cpp_module_library(matrix matrix.cppm)
add_cpp_module_library(inverse_matrix inverse_matrix.cppm)