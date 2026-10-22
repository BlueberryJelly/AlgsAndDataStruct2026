export module doubly_linked_list;

import std;

export template<typename Value>
struct Node final
{
    Node* previous = nullptr;
    Value value{};
    Node* next = nullptr;
};