import std;
import doubly_linked_list;
import big_int;

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
        switch (operation)
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
    std::cout << VerifyOperation(BigInt(100), BigInt(-1), BigInt(100 - 1), '+') << '\n';
    std::cout << VerifyOperation(BigInt(-100), BigInt(1), BigInt(-100 + 1), '+') << '\n';
    std::cout << VerifyOperation(BigInt(1000), BigInt(-999), BigInt(1000 - 999), '+') << '\n';
    std::cout << VerifyOperation(BigInt(5), BigInt(-5), BigInt(5 - 5), '+') << '\n';
    std::cout << VerifyOperation(BigInt(0), BigInt(-7), BigInt(0 - 7), '+') << '\n';

    std::cout << VerifyOperation(BigInt(12), BigInt(12), BigInt(12 * 12), '*') << '\n';
    std::cout << VerifyOperation(BigInt(-12), BigInt(12), BigInt(-12 * 12), '*') << '\n';
    std::cout << VerifyOperation(BigInt(-12), BigInt(-12), BigInt(-12 * (-12)), '*') << '\n';
    std::cout << VerifyOperation(BigInt(100), BigInt(-100), BigInt(100 * (-100)), '*') << '\n';
    std::cout << VerifyOperation(BigInt(0), BigInt(-5), BigInt(0 * (-5)), '*') << '\n';

    long long min_int = std::numeric_limits<int>::min();
    long long max_int = std::numeric_limits<int>::max();

    long long overflow = min_int + min_int;
    std::cout << VerifyOperation(BigInt(min_int), BigInt(min_int), BigInt(overflow), '+') << '\n';

    overflow = max_int + max_int;
    std::cout << VerifyOperation(BigInt(max_int), BigInt(max_int), BigInt(overflow), '+') << '\n';

    overflow = min_int * max_int;
    std::cout << VerifyOperation(BigInt(min_int), BigInt(max_int), BigInt(overflow), '*') << '\n';

    overflow = max_int * max_int;
    std::cout << VerifyOperation(BigInt(max_int), BigInt(max_int), BigInt(overflow), '*') << '\n';

    return 0;
}