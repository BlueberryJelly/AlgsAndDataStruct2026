import std;
import stats;
import doubly_linked_list;
import bubble_sort;
import comb_sort;

std::ostream &operator<<(std::ostream &os, const Stats &stats)
{
    os << "Число сравнений:" << '\n';
    os << '\t' << stats.comparison_count_ << '\n';
    os << "Число копирований:" << '\n';
    os << '\t' << stats.copy_count_ << '\n';

    return os;
}

int main()
{
    return 0;
}