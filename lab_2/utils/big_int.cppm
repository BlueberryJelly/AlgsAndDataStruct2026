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
};