export module big_int;

import std;
import doubly_linked_list;

export class BigInt final
{
private:
    using Digits = DoublyLinkedList<std::uint8_t>;

    int sign_ = 0;
    Digits body_;

    BigInt(int sign, Digits body) : sign_(sign), body_(std::move(body))
    {
        normalize();
    }

    static void push_digit(Digits &digits, int digit)
    {
        digits.push_head(static_cast<std::uint8_t>(digit));
    }

    void normalize()
    {
        while (body_.size() > 1 && *body_.cbegin() == 0)
        {
            body_.pop_head();
        }

        if (body_.size() == 1 && *body_.cbegin() == 0) sign_ = 0;
    }

    static bool less_magnitude(const Digits &lhs, const Digits &rhs)
    {
        if (lhs.size() != rhs.size()) return lhs.size() < rhs.size();

        return std::lexicographical_compare(lhs.cbegin(), lhs.cend(),
                                            rhs.cbegin(), rhs.cend());
    }

    static Digits combine(const Digits &big, const Digits &small, int factor)
    {
        Digits result;
        auto big_it = big.cend();
        auto small_it = small.cend();
        int carry = 0;

        while (big_it != big.cbegin())
        {
            --big_it;

            int small_digit = 0;
            if (small_it != small.cbegin())
            {
                --small_it;
                small_digit = *small_it;
            }

            const int total = carry + *big_it + factor * small_digit;
            carry = (total < 0) ? -1 : total / 10;
            push_digit(result, total - carry * 10);
        }

        if (carry > 0) push_digit(result, carry);

        return result;
    }

    static Digits multiply_digit(const Digits &digits, int digit, std::size_t shift)
    {
        Digits result;

        for (std::size_t i = 0; i < shift; ++i)
        {
            push_digit(result, 0);
        }

        int carry = 0;
        auto it = digits.cend();

        while (it != digits.cbegin())
        {
            --it;
            carry += *it * digit;
            push_digit(result, carry % 10);
            carry /= 10;
        }

        if (carry > 0) push_digit(result, carry);

        return result;
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

    explicit BigInt(std::string_view text)
    {
        const bool negative = text.starts_with('-');
        if (negative) text.remove_prefix(1);

        const auto is_digit = [](char c)
        { return c >= '0' && c <= '9'; };

        if (text.empty() || !std::ranges::all_of(text, is_digit))
        {
            throw std::invalid_argument("Invalid number string.");
        }

        for (const char c : std::views::reverse(text))
        {
            push_digit(body_, c - '0');
        }

        sign_ = negative ? -1 : 1;
        normalize();
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

    friend BigInt operator+(const BigInt &lhs, const BigInt &rhs)
    {
        const bool lhs_smaller = less_magnitude(lhs.body_, rhs.body_);
        const BigInt &big = lhs_smaller ? rhs : lhs;
        const BigInt &small = lhs_smaller ? lhs : rhs;
        const int factor = (lhs.sign_ == rhs.sign_) ? 1 : -1;

        return BigInt(big.sign_, combine(big.body_, small.body_, factor));
    }

    friend BigInt operator*(const BigInt &lhs, const BigInt &rhs)
    {
        BigInt result;
        auto it = rhs.body_.cend();

        for (std::size_t shift = 0; it != rhs.body_.cbegin(); ++shift)
        {
            --it;
            result = result + BigInt(1, multiply_digit(lhs.body_, *it, shift));
        }

        result.sign_ *= lhs.sign_ * rhs.sign_;

        return result;
    }
};