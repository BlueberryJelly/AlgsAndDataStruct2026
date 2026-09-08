set(LAB_NAME lab_1)

add_executable(${LAB_NAME} main.cpp)
target_link_libraries(${LAB_NAME}
    PRIVATE
    matrix
    inverse_matrix)