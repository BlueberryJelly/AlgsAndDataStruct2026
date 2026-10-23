export module doubly_linked_list;

import std;

export template<typename Value>
struct Node final
{
    Node* previous = nullptr;
    Value value{};
    Node* next = nullptr;
};

export template<typename Value>
class DoublyLinkedList final
{
private:
    Node<Value> *_head = nullptr;
    std::size_t _size = 0;

public:
};