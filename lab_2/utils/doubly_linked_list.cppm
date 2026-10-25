export module doubly_linked_list;

import std;
import random_generator;

struct NodeBase
{
    NodeBase *previous_ = nullptr;
    NodeBase *next_ = nullptr;
};

template <typename T>
struct Node final : public NodeBase
{
    T value_;

    explicit Node(const T &value) : value_(value) {}
    explicit Node(T &&value) : value_(std::move(value)) {}
};

export template <typename T>
class DoublyLinkedList final
{
private:
    NodeBase sentinel_;
    std::size_t size_ = 0;

    bool empty() const noexcept
    {
        return sentinel_.next_ == &sentinel_;
    }

    void free_data() noexcept
    {
        while (!empty())
        {
            pop_tail();
        }
    }

public:
    DoublyLinkedList() noexcept
    {
        sentinel_.next_ = &sentinel_;
        sentinel_.previous_ = &sentinel_;
    }

    DoublyLinkedList(const T &min_value, const T &max_value, std::size_t size) : DoublyLinkedList()
    {
        try
        {
            for (std::size_t iteration = 0; iteration < size; ++iteration)
            {
                push_tail(random_utils::generate_random<T>(min_value, max_value));
            }
        }
        catch (...)
        {
            free_data();
            throw;
        }
    }

    DoublyLinkedList(const DoublyLinkedList<T> &other) : DoublyLinkedList()
    {
        for (const NodeBase *curr = other.sentinel_.next_; curr != &other.sentinel_; curr = curr->next_)
        {
            push_tail(static_cast<const Node<T> *>(curr)->value_);
        }
    }

    DoublyLinkedList(DoublyLinkedList<T> &&other) noexcept : DoublyLinkedList()
    {
        if (!other.empty())
        {
            sentinel_.next_ = other.sentinel_.next_;
            sentinel_.previous_ = other.sentinel_.previous_;
            size_ = other.size_;

            sentinel_.next_->previous_ = &sentinel_;
            sentinel_.previous_->next_ = &sentinel_;

            other.sentinel_.next_ = &other.sentinel_;
            other.sentinel_.previous_ = &other.sentinel_;
            other.size_ = 0;
        }
    }

    DoublyLinkedList<T> &operator=(const DoublyLinkedList<T> &other)
    {
        if (this != &other)
        {
            DoublyLinkedList<T> tmp(other);
            *this = std::move(tmp);
        }
        return *this;
    }

    DoublyLinkedList<T> &operator=(DoublyLinkedList<T> &&other) noexcept
    {
        if (this != &other)
        {
            free_data();

            if (!other.empty())
            {
                sentinel_.next_ = other.sentinel_.next_;
                sentinel_.previous_ = other.sentinel_.previous_;
                size_ = other.size_;

                sentinel_.next_->previous_ = &sentinel_;
                sentinel_.previous_->next_ = &sentinel_;

                other.sentinel_.next_ = &other.sentinel_;
                other.sentinel_.previous_ = &other.sentinel_;
                other.size_ = 0;
            }
        }
        return *this;
    }

    ~DoublyLinkedList() noexcept
    {
        free_data();
    }

    template <typename U>
        requires(std::constructible_from<T, U &&> &&
                 !std::is_same_v<std::decay_t<U>, DoublyLinkedList<T>>)
    void push_tail(U &&value)
    {
        Node<T> *new_node = new Node<T>(std::forward<U>(value));

        new_node->previous_ = sentinel_.previous_;
        new_node->next_ = &sentinel_;

        sentinel_.previous_->next_ = new_node;
        sentinel_.previous_ = new_node;

        ++size_;
    }

    void push_tail(const DoublyLinkedList<T> &other)
    {
        if (this == &other || other.empty())
            return;
        DoublyLinkedList<T> tmp(other);
        push_tail(std::move(tmp));
    }

    void push_tail(DoublyLinkedList<T> &&other) noexcept
    {
        if (this == &other || other.empty())
            return;

        NodeBase *other_first = other.sentinel_.next_;
        NodeBase *other_last = other.sentinel_.previous_;

        other_last->next_ = &sentinel_;
        sentinel_.previous_->next_ = other_first;
        other_first->previous_ = sentinel_.previous_;
        sentinel_.previous_ = other_last;

        size_ += other.size_;

        other.sentinel_.next_ = &other.sentinel_;
        other.sentinel_.previous_ = &other.sentinel_;
        other.size_ = 0;
    }

    void pop_tail() noexcept
    {
        if (empty())
            return;

        NodeBase *tmp = sentinel_.previous_;
        sentinel_.previous_ = tmp->previous_;
        tmp->previous_->next_ = &sentinel_;

        delete static_cast<Node<T> *>(tmp);
        --size_;
    }

    template <typename U>
        requires(std::constructible_from<T, U &&> &&
                 !std::is_same_v<std::decay_t<U>, DoublyLinkedList<T>>)
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
        if (this == &other || other.head_ == nullptr)
            return;

        DoublyLinkedList<T> tmp(other);
        push_head(std::move(tmp));
    }

    void push_head(DoublyLinkedList<T> &&other) noexcept
    {
        if (this == &other || other.head_ == nullptr)
            return;

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
        if (head_ == nullptr)
            return;

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

        while (current != nullptr)
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

    const T &operator[](std::size_t index) const
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

            while (index > 0)
            {
                current = current->previous_;
                --index;
            }
        }
        else
        {
            current = head_;

            while (index > 0)
            {
                current = current->next_;
                --index;
            }
        }

        return current->value_;
    }

    T &operator[](std::size_t index)
    {
        return const_cast<T &>(std::as_const(*this)[index]);
    }

    template <typename U>
    friend std::ostream &operator<<(std::ostream &, const DoublyLinkedList<U> &);
};

template <typename T>
std::ostream &operator<<(std::ostream &os, const DoublyLinkedList<T> &list)
{
    const Node<T> *current = list.head_;

    if (current != nullptr)
    {
        os << current->value_;
        current = current->next_;
    }

    while (current != nullptr)
    {
        os << ", " << current->value_;
        current = current->next_;
    }

    os << "\n";
    return os;
}