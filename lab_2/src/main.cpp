import std;
import doubly_linked_list;
import big_int;

enum class Operation : bool
{
    sum,
    multiply
};

BigInt apply(const BigInt &lhs, const BigInt &rhs, Operation operation)
{
    switch (operation)
    {
    case Operation::sum:
        return lhs + rhs;
    case Operation::multiply:
        return lhs * rhs;
    default:
        throw std::invalid_argument("Only addition and multiplication are available.");
    }
}

struct VerifyOperation final
{
    const BigInt lhs_;
    const BigInt rhs_;
    const BigInt expected_;
    const Operation operation_;
    const BigInt result_;
    const bool ok_;

    VerifyOperation(const BigInt &lhs, const BigInt &rhs, const BigInt &expected, Operation operation)
        : lhs_(lhs), rhs_(rhs), expected_(expected), operation_(operation),
          result_(apply(lhs, rhs, operation)), ok_(result_ == expected) {}
};

std::ostream &operator<<(std::ostream &os, const VerifyOperation &v)
{
    char operation = (v.operation_ == Operation::sum) ? '+' : '*';
    os << v.lhs_ << ' ' << operation << ' ' << v.rhs_ << '\n';
    os << "Result:   " << v.result_ << '\n';
    os << "Expected: " << v.expected_ << '\n';
    os << "Verified: " << (v.ok_ ? "Yes" : "No") << '\n';

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