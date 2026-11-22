export module comb_sort;

import std;
import stats;

export template <std::forward_iterator ForwardIt, typename Compare>
    requires std::sortable<ForwardIt, Compare>
Stats comb_sort(ForwardIt first, ForwardIt last, float factor, Compare compare)
{
    if (!(factor > 0.f && factor < 1.f))
    {
        throw std::invalid_argument("Factor must be in (0, 1).");
    }

    Stats stats;

    using Diff = std::iter_difference_t<ForwardIt>;

    const Diff size = std::distance(first, last);

    if (size == 0 || size == 1) return stats;

    Diff step = size;
    bool swapped = false;

    while (step > 1 || swapped)
    {
        if (step > 1) 
        {
            const Diff shrunk = static_cast<Diff>(static_cast<float>(step) * factor);
            step = std::max<Diff>(1, std::min<Diff>(shrunk, step - 1));
        }

        swapped = false;

        auto current = first;
        auto next = std::next(first, step);

        while (next != last)
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
    }

    return stats;
}