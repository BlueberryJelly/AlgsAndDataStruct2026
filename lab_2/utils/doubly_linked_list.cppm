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

    void free_data() noexcept
    {
        while (_head != nullptr)
        {
            Node<Value>* tmp = _head;
            _head = _head->next;
            delete tmp;
        }
    }

    void free_list() noexcept
    {
        free_data();
        _head = nullptr;
        _size = 0;
    }

public:
};