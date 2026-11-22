set(LAB_NAME lab_3)

add_executable(${LAB_NAME} main.cpp)
target_link_libraries(${LAB_NAME}
    PRIVATE
    doubly_linked_list
    bubble_sort
    comb_sort
)
