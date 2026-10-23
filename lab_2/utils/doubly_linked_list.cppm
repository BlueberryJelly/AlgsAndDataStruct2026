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

public:
    DoublyLinkedList() noexcept = default;

    DoublyLinkedList(const DoublyLinkedList<Value> &other)
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
            free_data();
            _size = 0;
            throw;
        }
    }

    DoublyLinkedList(DoublyLinkedList<Value> &&other) noexcept
     : _head(other._head), _size(other._size)
    {
        other._head = nullptr;
        other._size = 0;
    }

    DoublyLinkedList<Value>& operator=(const DoublyLinkedList<Value>& other)
    {
        if (this != &other)
        {
            DoublyLinkedList<Value> tmp(other);
            *this = std::move(tmp);
        }

        return *this;
    }

    DoublyLinkedList<Value>& operator=(DoublyLinkedList<Value> &&other) noexcept
    {
        if (this != &other)
        {
            free_data();
            _head = other._head;
            other._head = nullptr;
            _size = other._size;
            other._size = 0;
        }

        return *this;
    }

    ~DoublyLinkedList() noexcept
    {
        free_data();
    }
};