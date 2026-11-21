import std;
import stats;
import bubble_sort;

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