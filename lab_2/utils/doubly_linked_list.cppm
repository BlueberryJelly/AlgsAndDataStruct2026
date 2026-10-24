export module doubly_linked_list;

import std;
import random_generator;

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
        try
        {
            for(std::size_t iteration = 0; iteration < size; ++iteration)
            {
                push_tail(random_utils::generate_random<T>(min_value, max_value));
            }
        }
        catch(...)
        {
            free_data();
            tail_ = nullptr;
            size_ = 0;
            throw;
        }
    }

    DoublyLinkedList(const DoublyLinkedList<T> &other)
    {
        if (other.head_ == nullptr) return;

        Node<T> *current = nullptr;
        Node<T> *previous = nullptr;
        try
        {
            for(const Node<T> *other_iterator = other.head_; 
                other_iterator != nullptr; 
                other_iterator = other_iterator->next_)
            {
                current = new Node<T>(other_iterator->value_);
                current->previous_ = previous;
                if (previous == nullptr)
                {
                    head_ = current;
                }
                else
                {
                    previous->next_ = current;
                }
                previous = current;
            }
            tail_ = previous;
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

    template<typename U>
        requires std::constructible_from<T, U&&> &&
             !std::is_same_v<std::decay_t<U>, DoublyLinkedList<T>>
    void push_tail(U &&value)
    {
        Node<T> *new_node = new Node<T>(std::forward<U>(value));

        if (head_ == nullptr)
        {
            head_ = new_node;
        }
        else
        {
            tail_->next_ = new_node;
            new_node->previous_ = tail_;
        }

        tail_ = new_node;
        ++size_;
    }

    void push_tail(const DoublyLinkedList<T> &other)
    {
        if (this == &other || other.head_ == nullptr) return;

        DoublyLinkedList<T> tmp(other);
        push_tail(std::move(tmp));
    }

    void push_tail(DoublyLinkedList<T> &&other) noexcept
    {
        if (this == &other || other.head_ == nullptr) return;

        if (head_ == nullptr)
        {
            head_ = other.head_;
        }
        else
        {
            other.head_->previous_ = tail_;
            tail_->next_ = other.head_;
        }

        tail_ = other.tail_;
        size_ += other.size_;

        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
    }

    void pop_tail() noexcept
    {
        if (head_ == nullptr) return;

        tail_ = tail_->previous_;

        if (tail_ == nullptr)
        {
            delete head_;
            head_ = nullptr; 
        }
        else
        {
            delete tail_->next_;
            tail_->next_ = nullptr;
        }

        --size_;
    }

    template<typename U>
        requires std::constructible_from<T, U&&> &&
             !std::is_same_v<std::decay_t<U>, DoublyLinkedList<T>>
    void push_head(U &&value)
    {
        Node<T> *new_node = new Node<T>(std::forward<U>(value));

        if (head_ == nullptr)
        {
            tail_ = new_node;
        }
        else
        {
            head_->previous_ = new_node;
            new_node->next_ = head_;
        }

        head_ = new_node;
        ++size_;
    }

    void push_head(const DoublyLinkedList<T> &other)
    {
        if (this == &other || other.head_ == nullptr) return;

        DoublyLinkedList<T> tmp(other);
        push_head(std::move(tmp));
    }

    void push_head(DoublyLinkedList<T> &&other) noexcept
    {
        if (this == &other || other.head_ == nullptr) return;

        if (head_ == nullptr)
        {
            tail_ = other.tail_;
        }
        else
        {
            other.tail_->next_ = head_;
            head_->previous_ = other.tail_;
        }

        head_ = other.head_;
        size_ += other.size_;

        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
    }

    void pop_head() noexcept
    {
        if (head_ == nullptr) return;

        head_ = head_->next_;

        if (head_ == nullptr)
        {
            delete tail_;
            tail_ = nullptr;
        }
        else
        {
            delete head_->previous_;
            head_->previous_ = nullptr;
        }

        --size_;
    }

    void delete_node(const T &value)
    {
        Node<T> *current = head_;

        while(current != nullptr)
        {
            if (current->value_ == value)
            {
                Node<T> *tmp = current;
                current = current->next_;

                if (tmp->previous_ != nullptr)
                {
                    tmp->previous_->next_ = tmp->next_;
                }
                else
                {
                    head_ = tmp->next_;
                }
                
                if (tmp->next_ != nullptr)
                {
                    tmp->next_->previous_ = tmp->previous_;
                }
                else
                {
                    tail_ = tmp->previous_;
                }

                delete tmp;
                --size_;
            }
            else
            {
                current = current->next_;
            }
        }
    }

    const T& operator[](std::size_t index) const
    {
        if (index >= size_)
        {
            throw std::out_of_range("Index out of list.");
        }
        
        const Node<T> *current;

        if (index > size_ / 2)
        {
            current = tail_;
            index = size_ - 1 - index;

            while(index > 0)
            {
                current = current->previous_;
                --index;
            }
        }
        else
        {
            current = head_;

            while(index > 0)
            {
                current = current->next_;
                --index;
            }
        }

        return current->value_;
    }

    T& operator[](std::size_t index)
    {
        return const_cast<T&>(std::as_const(*this)[index]);
    }
};