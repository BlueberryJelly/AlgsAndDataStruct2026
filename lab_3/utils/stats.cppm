export module stats;

import std;

export struct Stats final
{
    std::size_t comparison_count_ = 0;
    std::size_t copy_count_ = 0;
};