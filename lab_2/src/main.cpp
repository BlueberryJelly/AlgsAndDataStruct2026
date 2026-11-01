import std;
import doubly_linked_list;

class BigInt final
{
private:
    int sign_ = 0;
    DoublyLinkedList<int> body_;

public:
    BigInt(long long number)
    {
        if (number == 0)
        {
            sign_ = 0;
            body_.push_head(0);
            return;
        }

        sign_ = (number > 0) ? 1 : -1;
        number = std::abs(number);

        while (number > 0)
        {
            body_.push_head(static_cast<int>(number % 10));
            number /= 10;
        }
    }

    friend std::ostream &operator<<(std::ostream &, const BigInt &);
};

std::ostream &operator<<(std::ostream &os, const BigInt &number)
{
    if (number.sign_ == -1) os << '-';

    auto iter = number.body_.begin();
    auto stop = number.body_.end();
    
    if (iter == stop) { os << 0; return os; }

    while (iter != stop)
    {
        os << *iter;
        ++iter;
    }
    return os;
}

int main()
{
    std::cout << BigInt(100) << '\n';
    std::cout << BigInt(-123456789) << '\n';
    std::cout << BigInt(-0) << '\n';
    std::cout << BigInt(5) << '\n';
    return 0;
}