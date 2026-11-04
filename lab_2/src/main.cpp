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

static void check(const BigInt &lhs, const BigInt &rhs, const BigInt &expected, Operation operation)
{
    std::cout << VerifyOperation(lhs, rhs, expected, operation) << '\n';
}

int main()
{
    check(100, -1, 100 - 1, Operation::sum);
    check(-100, 1, -100 + 1, Operation::sum);
    check(1000, -999, 1000 - 999, Operation::sum);
    check(5, -5, 5 - 5, Operation::sum);
    check(0, -7, 0 - 7, Operation::sum);

    check(12, 12, 12 * 12, Operation::multiply);
    check(-12, 12, -12 * 12, Operation::multiply);
    check(-12, -12, -12 * -12, Operation::multiply);
    check(100, -100, 100 * -100, Operation::multiply);
    check(0, -5, 0 * -5, Operation::multiply);

    constexpr long long min_int = std::numeric_limits<int>::min();
    constexpr long long max_int = std::numeric_limits<int>::max();

    check(min_int, min_int, min_int + min_int, Operation::sum);
    check(max_int, max_int, max_int + max_int, Operation::sum);
    check(min_int, max_int, min_int * max_int, Operation::multiply);
    check(max_int, max_int, max_int * max_int, Operation::multiply);

    const BigInt huge("99999999999999999999");

    check(huge, 1, BigInt("100000000000000000000"), Operation::sum);
    check(huge, huge, BigInt("99999999999999999998"
                             "00000000000000000001"),
          Operation::multiply);

    return 0;
}