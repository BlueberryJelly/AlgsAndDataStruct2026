import std;
import matrix;
import matrix_operations;
import inverse_matrix;

template <MatrixNumeric NumericType>
NumericType make_bound(double value)
{
    if constexpr (std::same_as<NumericType, std::complex<float>> ||
                  std::same_as<NumericType, std::complex<double>>)
    {
        using Real = typename NumericType::value_type;
        return NumericType(static_cast<Real>(value), static_cast<Real>(value));
    }
    else
    {
        return static_cast<NumericType>(value);
    }
}

template <MatrixNumeric NumericType>
void demonstrate(const std::string &label)
{
    std::cout << "===================================================\n";
    std::cout << "Demonstration for type: " << label << "\n";
    std::cout << "===================================================\n\n";

    const NumericType lower = make_bound<NumericType>(-10);
    const NumericType upper = make_bound<NumericType>(10);

    Matrix<NumericType> a(3, 3, lower, upper);
    std::cout << "Random matrix A (3x3):\n"
              << a << "\n";

    Matrix<NumericType> filled(3, 3, NumericType{2});
    std::cout << "Matrix filled with value 2:\n"
              << filled << "\n";

    Matrix<NumericType> b(a);
    std::cout << "B = copy of A:\n"
              << b << "\n";

    std::cout << "A.equal(B): " << std::boolalpha << a.equal(b) << "\n";
    std::cout << "A == B: " << (a == b) << "\n";
    std::cout << "A != B: " << (a != b) << "\n\n";

    Matrix<NumericType> c(std::move(b));
    std::cout << "C = moved from B:\n"
              << c << "\n";

    Matrix<NumericType> d(3, 3, NumericType{0});
    d = a;
    std::cout << "D = A (copy assignment):\n"
              << d << "\n";

    Matrix<NumericType> e(3, 3, NumericType{0});
    e = std::move(d);
    std::cout << "E = std::move(D) (move assignment):\n"
              << e << "\n";

    std::cout << "A[0, 0] = " << a[0, 0] << "\n";
    std::cout << "A.at(1, 1) = " << a.at(1, 1) << "\n";
    a[0, 0] = make_bound<NumericType>(100);
    std::cout << "A after A[0,0] = 100:\n"
              << a << "\n";

    std::cout << "A.same_size(C): " << a.same_size(c) << "\n\n";

    Matrix<NumericType> sum_assign(a);
    sum_assign += c;
    std::cout << "A += C ->\n"
              << sum_assign << "\n";

    Matrix<NumericType> sub_assign(a);
    sub_assign -= c;
    std::cout << "A -= C ->\n"
              << sub_assign << "\n";

    Matrix<NumericType> mul_assign(a);
    mul_assign *= NumericType{2};
    std::cout << "A *= 2 ->\n"
              << mul_assign << "\n";

    Matrix<NumericType> div_assign(a);
    div_assign /= NumericType{2};
    std::cout << "A /= 2 ->\n"
              << div_assign << "\n";

    Matrix<NumericType> sum = a + c;
    std::cout << "A + C =\n"
              << sum << "\n";

    Matrix<NumericType> diff = a - c;
    std::cout << "A - C =\n"
              << diff << "\n";

    a.multiplyable(c);
    std::cout << "A.multiplyable(C) OK (without exception)\n";
    Matrix<NumericType> prod = a * c;
    std::cout << "A * C =\n"
              << prod << "\n";

    Matrix<NumericType> scaled1 = a * NumericType{3};
    std::cout << "A * 3 =\n"
              << scaled1 << "\n";

    Matrix<NumericType> scaled2 = NumericType{3} * a;
    std::cout << "3 * A =\n"
              << scaled2 << "\n";

    Matrix<NumericType> divided = a / NumericType{2};
    std::cout << "A / 2 =\n"
              << divided << "\n";

    std::cout << "A.get_rows() = " << a.get_rows() << ", A.get_columns() = " << a.get_columns() << "\n";
    std::cout << "A.get_data()[0] = " << a.get_data()[0] << "\n";
    std::cout << "A.get_epsilon() = " << a.get_epsilon() << "\n\n";

    std::cout << "square_matrix(A): " << square_matrix(a) << "\n";
    std::cout << "trace(A) = " << trace(a) << "\n\n";

    Matrix<NumericType> minor = get_element_minor(a, 0, 0);
    std::cout << "Minor of A (skip row 0, col 0):\n"
              << minor << "\n";

    NumericType det = get_determinant(a);
    std::cout << "get_determinant(A) = " << det << "\n\n";

    Matrix<NumericType> transposed = transpose(a);
    std::cout << "transpose(A) =\n"
              << transposed << "\n";

    try
    {
        Matrix<NumericType> inv = inverse(a);
        std::cout << "inverse(A) =\n"
                  << inv << "\n";

        Matrix<NumericType> identity_check = a * inv;
        std::cout << "A * inverse(A) (shuold be near I):\n"
                  << identity_check << "\n";
    }
    catch (const std::exception &ex)
    {
        std::cout << "inverse(A) failed: " << ex.what() << "\n\n";
    }

    std::cout << "--- Exception checking ---\n";

    try
    {
        Matrix<NumericType> bad(0, 3, NumericType{0});
    }
    catch (const std::exception &ex)
    {
        std::cout << "Expected (Prohibited size): " << ex.what() << "\n";
    }

    try
    {
        a.at(10, 10);
    }
    catch (const std::exception &ex)
    {
        std::cout << "Expected (Incorrect index(indexes) for matrix): " << ex.what() << "\n";
    }

    try
    {
        Matrix<NumericType> mismatched(2, 2, NumericType{0});
        auto bad_sum = a + mismatched;
    }
    catch (const std::exception &ex)
    {
        std::cout << "Expected (It's impossible to add or subtract): " << ex.what() << "\n";
    }

    try
    {
        Matrix<NumericType> mismatched(4, 4, NumericType{0});
        auto bad_prod = a * mismatched;
    }
    catch (const std::exception &ex)
    {
        std::cout << "Expected (It's impossible to multiply): " << ex.what() << "\n";
    }

    try
    {
        Matrix<NumericType> zero_div(a);
        zero_div /= NumericType{0};
    }
    catch (const std::exception &ex)
    {
        std::cout << "Expected (Division by zero): " << ex.what() << "\n";
    }

    try
    {
        Matrix<NumericType> singular(2, 2, NumericType{0});
        auto singular_inverse = inverse(singular);
    }
    catch (const std::exception &ex)
    {
        std::cout << "Expected (Matrix is singular, inverse does not exist): " << ex.what() << "\n";
    }

    std::cout << "\n\n";
}

int main()
{
    demonstrate<int>("int");
    demonstrate<double>("double");
    demonstrate<std::complex<double>>("std::complex<double>");

    return 0;
}