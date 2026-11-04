export module big_int;

import std;
import doubly_linked_list;

export class BigInt final
{
private:
    using Digits = DoublyLinkedList<std::uint8_t>;

    int sign_ = 0;
    Digits body_;

    static void push_digit(Digits &digits, int digit)
    {
        digits.push_head(static_cast<std::uint8_t>(digit));
    }

    static bool less_magnitude(const Digits &lhs, const Digits &rhs)
    {
        if (lhs.size() != rhs.size()) return lhs.size() < rhs.size();

        return std::lexicographical_compare(lhs.cbegin(), lhs.cend(),
                                            rhs.cbegin(), rhs.cend());
    }
public:
    BigInt(long long number = 0)
    {
        sign_ = (number > 0) ? 1 : (number < 0) ? -1 : 0;

        unsigned long long n = (number < 0)
                                   ? 0ULL - static_cast<unsigned long long>(number)
                                   : static_cast<unsigned long long>(number);

        do
        {
            push_digit(body_, static_cast<int>(n % 10));
            n /= 10;
        } while (n > 0);
    }

    friend std::ostream &operator<<(std::ostream &os, const BigInt &number)
    {
        if (number.sign_ < 0) os << '-';

        for (auto it = number.body_.cbegin(); it != number.body_.cend(); ++it)
        {
            os << static_cast<int>(*it);
        }

        return os;
    }

    friend bool operator==(const BigInt &lhs, const BigInt &rhs) noexcept
    {
        return lhs.sign_ == rhs.sign_ &&
               std::equal(lhs.body_.cbegin(), lhs.body_.cend(),
                          rhs.body_.cbegin(), rhs.body_.cend());
    }
};