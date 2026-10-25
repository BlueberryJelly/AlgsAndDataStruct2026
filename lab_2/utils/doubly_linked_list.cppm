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

template <typename T>
class ConstIterator final
{
private:
    const NodeBase *iterator_ = nullptr;

public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T *;
    using reference = const T &;

    ConstIterator() noexcept = default;
    explicit ConstIterator(const NodeBase *start) : iterator_(start) {}

    ConstIterator<T> &operator++() noexcept
    {
        iterator_ = iterator_->next_;
        return *this;
    }

    ConstIterator<T> operator++(int) noexcept
    {
        ConstIterator<T> tmp = *this;
        ++(*this);
        return tmp;
    }

    ConstIterator<T> &operator--() noexcept
    {
        iterator_ = iterator_->previous_;
        return *this;
    }

    ConstIterator<T> operator--(int) noexcept
    {
        ConstIterator<T> tmp = *this;
        --(*this);
        return tmp;
    }

    const T &operator*() const noexcept
    {
        return static_cast<const Node<T> *>(iterator_)->value_;
    }

    const T *operator->() const noexcept
    {
        return &(static_cast<const Node<T> *>(iterator_)->value_);
    }

    bool operator==(const ConstIterator &other) const noexcept
    {
        return iterator_ == other.iterator_;
    }

    bool operator!=(const ConstIterator &other) const noexcept
    {
        return iterator_ != other.iterator_;
    }
};

template <typename T>
class Iterator final
{
private:
    NodeBase *iterator_ = nullptr;

public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;

    Iterator() noexcept = default;
    explicit Iterator(NodeBase *start) : iterator_(start) {}

    operator ConstIterator<T>() const noexcept
    {
        return ConstIterator<T>(iterator_);
    }

    Iterator<T> &operator++() noexcept
    {
        iterator_ = iterator_->next_;
        return *this;
    }

    Iterator<T> operator++(int) noexcept
    {
        Iterator<T> tmp = *this;
        ++(*this);
        return tmp;
    }

    Iterator<T> &operator--() noexcept
    {
        iterator_ = iterator_->previous_;
        return *this;
    }

    Iterator<T> operator--(int) noexcept
    {
        Iterator<T> tmp = *this;
        --(*this);
        return tmp;
    }

    T &operator*() const noexcept
    {
        return static_cast<Node<T> *>(iterator_)->value_;
    }

    T *operator->() const noexcept
    {
        return &(static_cast<Node<T> *>(iterator_)->value_);
    }

    bool operator==(const Iterator &other) const noexcept
    {
        return iterator_ == other.iterator_;
    }

    bool operator!=(const Iterator &other) const noexcept
    {
        return iterator_ != other.iterator_;
    }
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
        sentinel_.previous_ = &sentinel_;
        sentinel_.next_ = &sentinel_;
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
            sentinel_.previous_ = other.sentinel_.previous_;
            other.sentinel_.previous_ = &other.sentinel_;
            sentinel_.next_ = other.sentinel_.next_;
            other.sentinel_.next_ = &other.sentinel_;
            size_ = other.size_;
            other.size_ = 0;

            sentinel_.previous_->next_ = &sentinel_;
            sentinel_.next_->previous_ = &sentinel_;
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
                sentinel_.previous_ = other.sentinel_.previous_;
                other.sentinel_.previous_ = &other.sentinel_;
                sentinel_.next_ = other.sentinel_.next_;
                other.sentinel_.next_ = &other.sentinel_;
                size_ = other.size_;
                other.size_ = 0;

                sentinel_.previous_->next_ = &sentinel_;
                sentinel_.next_->previous_ = &sentinel_;
            }
        }
        return *this;
    }

    ~DoublyLinkedList() noexcept
    {
        free_data();
    }

    Iterator<T> begin() noexcept { return Iterator<T>(sentinel_.next_); }
    Iterator<T> end() noexcept { return Iterator<T>(&sentinel_); }

    ConstIterator<T> begin() const noexcept { return ConstIterator<T>(sentinel_.next_); }
    ConstIterator<T> end() const noexcept { return ConstIterator<T>(&sentinel_); }

    ConstIterator<T> cbegin() const noexcept { return ConstIterator<T>(sentinel_.next_); }
    ConstIterator<T> cend() const noexcept { return ConstIterator<T>(&sentinel_); }

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
        if (this == &other || other.empty()) return;
        
        DoublyLinkedList<T> tmp(other);
        push_tail(std::move(tmp));
    }

    void push_tail(DoublyLinkedList<T> &&other) noexcept
    {
        if (this == &other || other.empty()) return;

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
        if (empty()) return;

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

        new_node->next_ = sentinel_.next_;
        new_node->previous_ = &sentinel_;

        sentinel_.next_->previous_ = new_node;
        sentinel_.next_ = new_node;

        ++size_;
    }

    void push_head(const DoublyLinkedList<T> &other)
    {
        if (this == &other || other.empty()) return;

        DoublyLinkedList<T> tmp(other);
        push_head(std::move(tmp));
    }

    void push_head(DoublyLinkedList<T> &&other) noexcept
    {
        if (this == &other || other.empty()) return;

        NodeBase *other_first = other.sentinel_.next_;
        NodeBase *other_last = other.sentinel_.previous_;

        other_first->previous_ = &sentinel_;
        sentinel_.next_->previous_ = other_last;
        other_last->next_ = sentinel_.next_;
        sentinel_.next_ = other_first;

        size_ += other.size_;

        other.sentinel_.next_ = &other.sentinel_;
        other.sentinel_.previous_ = &other.sentinel_;
        other.size_ = 0;
    }

    void pop_head() noexcept
    {
        if (empty()) return;

        NodeBase *tmp = sentinel_.next_;
        sentinel_.next_ = tmp->next_;
        tmp->next_->previous_ = &sentinel_;

        delete static_cast<Node<T> *>(tmp);
        --size_;
    }

    void delete_node(const T &value)
    {
        NodeBase *current = sentinel_.next_;

        while (current != &sentinel_)
        {
            Node<T> *node = static_cast<Node<T> *>(current);
            NodeBase *next_node = current->next_;

            if (node->value_ == value)
            {
                current->previous_->next_ = current->next_;
                current->next_->previous_ = current->previous_;
                delete node;
                --size_;
            }

            current = next_node;
        }
    }

    const T &operator[](std::size_t index) const
    {
        if (index >= size_)
        {
            throw std::out_of_range("Index out of list.");
        }

        const NodeBase *current;

        if (index < size_ / 2)
        {
            current = sentinel_.next_;
            for (std::size_t i = 0; i < index; ++i) current = current->next_;
        }
        else
        {
            current = sentinel_.previous_;
            for (std::size_t i = size_ - 1; i > index; --i) current = current->previous_;
        }

        return static_cast<const Node<T> *>(current)->value_;
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
    const NodeBase *current = list.sentinel_.next_;

    if (current != &list.sentinel_)
    {
        os << static_cast<const Node<T> *>(current)->value_;
        current = current->next_;
    }

    while (current != &list.sentinel_)
    {
        os << ", " << static_cast<const Node<T> *>(current)->value_;
        current = current->next_;
    }

    os << "\n";
    return os;
}