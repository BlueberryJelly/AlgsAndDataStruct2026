export module bubble_sort;

import std;
import stats;
import doubly_linked_list;

export template <std::forward_iterator ForwardIt, typename Compare>
    requires std::sortable<ForwardIt, Compare>
Stats bubble_sort(ForwardIt first, ForwardIt last, Compare compare)
{
    using T = std::iter_value_t<ForwardIt>;
    
    Stats stats;

    if (first == last) return stats;

    DoublyLinkedList<T> list(first, last);
    stats.copy_count_ += list.size();

    auto second = ++(list.begin());

    for (auto stop = list.end(); stop != second; --stop)
    {
        auto current = list.begin();
        auto next = second;
        bool swapped = false;

        while (next != stop)
        {
            ++stats.comparison_count_;

            if (std::invoke(compare, *next, *current))
            {
                std::iter_swap(current, next);
                stats.copy_count_ += 3;
                swapped = true;
            }

            ++current;
            ++next;
        }

        if (!swapped) break;
    }

    std::move(list.begin(), list.end(), first);
    stats.copy_count_ += list.size();

    return stats;
}