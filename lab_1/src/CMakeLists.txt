set(LAB_NAME lab_1)

add_executable(${LAB_NAME} main.cpp)
target_link_libraries(${LAB_NAME}
    PRIVATE
    matrix
    matrix_operations
    inverse_matrix)