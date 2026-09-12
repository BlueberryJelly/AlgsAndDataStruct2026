export module math_ops_for_matrix;

import std;
import matrix;

export template <MatrixType NumericType>
bool operator==(const Matrix<NumericType> &left, const Matrix<NumericType> &right) noexcept
{
    return left.equal(right);
}

export template <MatrixNumeric NumericType>
bool operator!=(const Matrix<NumericType> &left, const Matrix<NumericType> &right) noexcept
{
    return !(left == right);
}

export template <MatrixNumeric NumericType>
Matrix<NumericType> operator+(const Matrix<NumericType> &left, const Matrix<NumericType> &right)
{
    left.validate_sum_subtraction(right);
    Matrix<NumericType> sum(left);
    sum += right;

    return sum;
}

export template <MatrixNumeric NumericType>
Matrix<NumericType> operator-(const Matrix<NumericType> &left, const Matrix<NumericType> &right)
{
    left.validate_sum_subtraction(right);
    Matrix<NumericType> subtraction(left);
    subtraction -= right;

    return subtraction;
}

export template <MatrixNumeric NumericType>
Matrix<NumericType> operator*(const Matrix<NumericType> &left, const Matrix<NumericType> &right)
{
    left.multiplyable(right);
    Matrix<NumericType> product(left.get_rows(), right.get_columns(), NumericType{});
    for (std::size_t i = 0; i < left.get_rows(); ++i)
    {
        for (std::size_t k = 0; k < left.get_columns(); ++k)
        {
            for (std::size_t j = 0; j < right.get_columns(); ++j)
            {
                product[i, j] += left[i, k] * right[k, j];
            }
        }
    }

    return product;
}

export template <MatrixNumeric NumericType>
Matrix<NumericType> operator*(const Matrix<NumericType> &left, const NumericType &right)
{
    Matrix<NumericType> product(left);
    product *= right;

    return product;
}

export template <MatrixNumeric NumericType>
Matrix<NumericType> operator*(const NumericType &left, const Matrix<NumericType> &right)
{
    Matrix<NumericType> product(right);
    product *= left;

    return product;
}

export template <MatrixNumeric NumericType>
Matrix<NumericType> operator/(const Matrix<NumericType> &left, const NumericType &right)
{
    Matrix<NumericType> quotient(left);
    quotient /= right;

    return quotient;
}