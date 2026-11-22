export module stats;

import std;

export struct Stats final
{
    std::uint64_t comparison_count_ = 0;
    std::uint64_t copy_count_ = 0;
};