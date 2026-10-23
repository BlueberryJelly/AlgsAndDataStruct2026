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
    DoublyLinkedList() noexcept = default;

    DoublyLinkedList(const DoublyLinkedList &other)
    {
        if (other._head == nullptr)
        {
            _head = nullptr;
            _size = 0;
            return;
        }

        _size = other._size;
        Node<Value> *other_iterator = other._head;

        try
        {
            Node<Value> *this_iterator = new Node<Value>();
            _head = this_iterator;
            this_iterator->previous = nullptr;
            while(other_iterator != nullptr)
            {
                this_iterator->value = other_iterator->value;

                if (other_iterator->next != nullptr)
                {
                    this_iterator->next = new Node<Value>();
                    this_iterator->next->previous = this_iterator;
                    this_iterator = this_iterator->next;
                }
                else
                {
                    this_iterator->next = nullptr;
                }

                other_iterator = other_iterator->next;
            }
        }
        catch(...)
        {
            free_list();
            throw;
        }
    }

};