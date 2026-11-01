import std;
import doubly_linked_list;

class BigInt final
{
private:
    int sign_ = 0;
    DoublyLinkedList<std::uint8_t> body_;

public:
    BigInt(int number)
    {
        if (number == 0)
        {
            sign_ = 0;
            body_.push_head(static_cast<std::uint8_t>(0));
            return;
        }

        sign_ = (number > 0) ? 1 : -1;
        long long n = std::abs(static_cast<long long>(number));

        while (n > 0)
        {
            body_.push_head(static_cast<std::uint8_t>(n % 10));
            n /= 10;
        }
    }

    friend std::ostream &operator<<(std::ostream &, const BigInt &);
};

std::ostream &operator<<(std::ostream &os, const BigInt &number)
{
    if (number.sign_ == -1) os << '-';

    auto iter = number.body_.begin();
    auto stop = number.body_.end();
    
    if (iter == stop) { os << '0'; return os; }

    while (iter != stop)
    {
        os << static_cast<int>(*iter);
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