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

    BigInt(long long number)
    {
        if (number == 0)
        {
            sign_ = 0;
            body_.push_head(static_cast<std::uint8_t>(0));
            return;
        }

        sign_ = (number > 0) ? 1 : -1;
        unsigned long long n = std::abs(number);

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

    friend BigInt operator+(BigInt lhs, BigInt rhs)
    {
        if (lhs.sign_ == 0) return rhs;

        if (rhs.sign_ == 0) return lhs;

        if (lhs.body_.size() > rhs.body_.size())
        {
            rhs.fill_to_size(lhs.body_.size());
        }
        else
        {
            lhs.fill_to_size(rhs.body_.size());
        }

        const bool same_sign = lhs.sign_ == rhs.sign_;

        if (!same_sign && std::lexicographical_compare(lhs.body_.begin(), lhs.body_.end(),
                                                       rhs.body_.begin(), rhs.body_.end()))
        {
            std::swap(lhs, rhs);
        }

        BigInt result;
        result.sign_ = lhs.sign_;

        auto l_it = lhs.body_.end();
        auto r_it = rhs.body_.end();

        auto stop = lhs.body_.begin();

        const int rhs_sign = same_sign ? 1 : -1;
            int carry = 0;

            while (l_it != stop)
            {
                --l_it;
                --r_it;

            const int total = carry + static_cast<int>(*l_it) + rhs_sign * static_cast<int>(*r_it);
            carry = (total < 0) ? -1 : total / 10;
            result.body_.push_head(static_cast<std::uint8_t>(total - carry * 10));
            }

            while (carry > 0)
            {
                result.body_.push_head(static_cast<std::uint8_t>(carry % 10));
                carry /= 10;
            }

        if (!same_sign)
        {
            while (result.body_.size() > 1 && *result.body_.begin() == 0)
            {
                result.body_.pop_head();
            }

            if (*result.body_.begin() == 0) result.sign_ = 0;
        }

        return result;
    }

    friend BigInt operator*(const BigInt &lhs, const BigInt &rhs)
    {
        if (lhs.sign_ == 0 || rhs.sign_ == 0) return BigInt(0);

        BigInt result;
        std::size_t shift = 0;

        auto r_it = rhs.body_.end();

        while (r_it != rhs.body_.begin())
        {
            --r_it;

            if (*r_it != 0)
            {
                BigInt partial;
                partial.sign_ = 1;

                for (std::size_t i = 0; i < shift; ++i)
                {
                    partial.body_.push_head(static_cast<std::uint8_t>(0));
                }

                int carry = 0;
                auto l_it = lhs.body_.end();

                while (l_it != lhs.body_.begin())
                {
                    --l_it;

                    carry += static_cast<int>(*l_it) * static_cast<int>(*r_it);
                    partial.body_.push_head(static_cast<std::uint8_t>(carry % 10));
                    carry /= 10;
                }

                if (carry > 0) partial.body_.push_head(static_cast<std::uint8_t>(carry));

                result = std::move(result) + std::move(partial);
            }

            ++shift;
        }

        result.sign_ = lhs.sign_ * rhs.sign_;

        return result;
    }
};

struct VerifyOperation final
{
    BigInt lhs_;
    BigInt rhs_;
    BigInt expected_;
    BigInt result_;
    char operation_;
    bool ok_;

    VerifyOperation() = delete;

    VerifyOperation(const BigInt &lhs, const BigInt &rhs, const BigInt &expected, char operation)
        : lhs_(lhs), rhs_(rhs), expected_(expected), operation_(operation)
    {
        switch(operation)
        {
            case '+':
                result_ = lhs_ + rhs_;
                break;
            case '*':
                result_ = lhs_ * rhs_;
                break;
            default:
                throw std::invalid_argument("Only addition and multiplication are available.");
        }

        ok_ = (result_ == expected_);
    }
};

std::ostream &operator<<(std::ostream &os, const VerifyOperation &verification)
{
    os << verification.lhs_ << ' ' << verification.operation_ << ' ' << verification.rhs_ << '\n';
    os << "Result:" << ' ';
    os << verification.result_ << '\n';
    os << "Expected:" << ' ';
    os << verification.expected_ << '\n';
    os << "Verified:" << ' ';
    os << (verification.ok_ ? "Yes" : "No") << '\n';

    return os;
}

int main()
{
    std::cout << BigInt(100) << '\n';
    std::cout << BigInt(-1234567899) << '\n';
    std::cout << BigInt(-01) << '\n';
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

    std::cout << (BigInt(-100) + BigInt(-100)) << '\n';
    std::cout << (BigInt(100) + BigInt(100)) << '\n';
    std::cout << (BigInt(2147483647) + BigInt(2147483647)) << '\n';
    std::cout << (BigInt(-10) + BigInt(-1000)) << '\n';

    return 0;
}