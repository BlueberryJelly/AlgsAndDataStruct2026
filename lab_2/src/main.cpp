import std;
import doubly_linked_list;

class BigInt final
{
private:
    int sign_ = 0;
    DoublyLinkedList<std::uint8_t> body_;

    void fill_to_size(std::size_t size)
    {
        while (body_.size() != size)
        {
            body_.push_head(static_cast<std::uint8_t>(0));
        }
    }

public:
    BigInt() noexcept = default;

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

    friend std::ostream &operator<<(std::ostream &os, const BigInt &number)
    {
        if (number.sign_ == -1) os << '-';

        auto iter = number.body_.begin();
        auto stop = number.body_.end();

        while (iter != stop)
        {
            os << static_cast<int>(*iter);
            ++iter;
        }

        return os;
    }

    friend bool operator==(const BigInt &lhs, const BigInt &rhs) noexcept
    {
        if (lhs.sign_ != rhs.sign_ ||
            lhs.body_.size() != rhs.body_.size())
        {
            return false;
        }

        auto l_it = lhs.body_.begin();
        auto r_it = rhs.body_.begin();

        while (l_it != lhs.body_.end())
        {
            if (*l_it != *r_it)
            {
                return false;
            }

            ++l_it;
            ++r_it;
        }

        return true;
    }

    friend bool operator!=(const BigInt &lhs, const BigInt &rhs) noexcept
    {
        return !(lhs == rhs);
    }

    friend BigInt abs(const BigInt &number)
    {
        if (number.sign_ == -1)
        {
            BigInt result = number;
            result.sign_ = 1;
            return result;
        }

        return number;
    }

    friend BigInt operator+(const BigInt &, const BigInt &);
};

/*BigInt operator+(const BigInt &lhs, const BigInt &rhs)
{
    if (lhs == BigInt(0)) return rhs;

    if (rhs == BigInt(0)) return lhs;

    //дополнить нулями вначале до одного размера lhs или rhs

    if (lhs.sign_ == rhs.sign_)
    {
    }
    lhs.sign_ == 1 rhs.sign_ == -1 (модуль 1 - модуль 2) оставляем знак получившего ся числа
    lhs.sign_ == -1 rhs.sign_ == 1 (модуль 1 - модуль 2) меняем знак на противоположный
    lhs.sign_ == rhs.sign_ (модуль 1 + модуль 2) снак как у слагаемых
}*/

int main()
{
    std::cout << BigInt(100) << '\n';
    std::cout << BigInt(-123456789) << '\n';
    std::cout << BigInt(-0) << '\n';
    std::cout << BigInt(5) << '\n';

    std::cout << (BigInt(5) == BigInt(5)) << '\n';
    std::cout << (BigInt(5) == BigInt(-5)) << '\n';
    std::cout << (BigInt(5) == BigInt(10)) << '\n';
    std::cout << (BigInt(1112) == BigInt(1113)) << '\n';
    std::cout << (BigInt(1112) == BigInt(-1113)) << '\n';
    std::cout << (BigInt(1112) == BigInt(1112)) << '\n';

    std::cout << abs(BigInt(-100)) << '\n';
    std::cout << abs(BigInt(0)) << '\n';
    std::cout << abs(BigInt(100)) << '\n';

    return 0;
}