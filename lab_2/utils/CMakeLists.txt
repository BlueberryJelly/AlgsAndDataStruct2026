function(add_cpp_module_library TARGET_NAME MODULE_FILE)
    add_library(${TARGET_NAME} STATIC)

    target_sources(${TARGET_NAME}
        PUBLIC
        FILE_SET CXX_MODULES
        FILES
        ${MODULE_FILE}
    )
endfunction()

add_cpp_module_library(random_generator random_generator.cppm)
add_cpp_module_library(doubly_linked_list doubly_linked_list.cppm)