export module doubly_linked_list;

import std;

template<typename T>
struct Node final
{
    Node<T> *previous_ = nullptr;
    Node<T> *next_ = nullptr;
    T value_;

    explicit Node(const T &value) : value_(value) {}

    explicit Node(T &&value) : value_(std::move(value)) {}
};

export template<typename T>
class DoublyLinkedList final
{
private:
    Node<T> *head_ = nullptr;
    Node<T> *tail_ = nullptr;
    std::size_t size_ = 0;

    void free_data() noexcept
    {
        while (head_ != nullptr)
        {
            Node<T> *tmp = head_;
            head_ = head_->next_;
            delete tmp;
        }
    }

public:
    DoublyLinkedList() noexcept = default;

    DoublyLinkedList(const T &min_value, const T &max_value, std::size_t size)
    {

    }

    DoublyLinkedList(const DoublyLinkedList<T> &other)
    {
        if (other.head_ == nullptr) return;

        Node<T> *this_current = nullptr;
        Node<T> *this_previous = nullptr;
        try
        {
            for(const Node<T> *other_iterator = other.head_; 
                other_iterator != nullptr; 
                other_iterator = other_iterator->next_)
                {
                this_current = new Node<T>(other_iterator->value_);
                this_current->previous_ = this_previous;
                if (this_previous == nullptr)
                {
                    head_ = this_current;
                }
                else
                {
                    this_previous->next_ = this_current;
                }
                this_previous = this_current;
            }
            tail_ = this_previous;
        }
        catch(...)
        {
            free_data();
            throw;
        }
        size_ = other.size_;
    }

    DoublyLinkedList(DoublyLinkedList<T> &&other) noexcept
     : head_(other.head_), tail_(other.tail_), size_(other.size_)
    {
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
    }

    DoublyLinkedList<T>& operator=(const DoublyLinkedList<T> &other)
    {
        if (this != &other)
        {
            DoublyLinkedList<T> tmp(other);
            *this = std::move(tmp);
        }

        return *this;
    }

    DoublyLinkedList<T>& operator=(DoublyLinkedList<T> &&other) noexcept
    {
        if (this != &other)
        {
            free_data();
            head_ = other.head_;
            other.head_ = nullptr;
            tail_ = other.tail_;
            other.tail_ = nullptr;
            size_ = other.size_;
            other.size_ = 0;
        }

        return *this;
    }

    ~DoublyLinkedList() noexcept
    {
        free_data();
    }
};