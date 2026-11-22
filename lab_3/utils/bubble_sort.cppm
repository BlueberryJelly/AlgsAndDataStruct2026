export module bubble_sort;

import std;
import stats;
import doubly_linked_list;

export template <typename T, std::forward_iterator ForwardIt>
    requires std::sortable<ForwardIt>
Stats bubble_sort(ForwardIt first, ForwardIt last)
{
    Stats stats;

    if (first == last)
    {
        return stats;
    }

    DoublyLinkedList<T> list(first, last);
    stats.copy_count_ += list.size();

    for (auto stop = list.end(); stop != ++list.begin(); --stop)
    {
        auto current = list.begin();
        auto next = ++(list.begin());

        std::size_t swaps = 0;

        while (next != stop)
        {
            ++stats.comparison_count_;

            if (*current > *next)
            {
                T tmp = std::move(*current);
                *current = std::move(*next);
                *next = std::move(tmp);
                ++swaps;

                if constexpr (!(std::is_move_constructible_v<T> && std::is_move_assignable_v<T>))
                {
                    stats.copy_count_ += 3;
                }
            }

            ++current;
            ++next;
        }

        if (swaps == 0)
            break;
    }

    std::move(list.begin(), list.end(), first);
    stats.copy_count_ += list.size();

    return stats;
}