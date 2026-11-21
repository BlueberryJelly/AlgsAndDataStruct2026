export module stats;

import std;

export struct Stats final
{
    std::uint32_t comparison_count_ = 0;
    std::uint32_t copy_count_ = 0;
};