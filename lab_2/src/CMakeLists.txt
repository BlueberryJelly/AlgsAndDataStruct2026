set(LAB_NAME lab_2)

add_executable(${LAB_NAME} main.cpp)
target_link_libraries(${LAB_NAME}
    PRIVATE
    doubly_linked_list
)