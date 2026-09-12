export module math_ops_for_matrix;

import std;
import matrix;

export template <typename NumericType>
bool operator==(const Matrix<NumericType> &left, const Matrix<NumericType> &right) noexcept
{
    if (!left.same_size(right))
    {
        return false;
    }

    std::size_t size = left.get_rows() * left.get_columns();
    for (std::size_t index = 0; index < size; ++index)
    {
        if (left.get_data()[index] != right.get_data()[index])
        {
            return false;
        }
    }

    return true;
}

export template <typename NumericType>
bool operator!=(const Matrix<NumericType> &left, const Matrix<NumericType> &right) noexcept
{
    return !(left == right);
}

export template <typename NumericType>
Matrix<NumericType> operator+(const Matrix<NumericType> &left, const Matrix<NumericType> &right)
{
    left.validate_sum_subtraction(right);
    Matrix<NumericType> sum(left);
    sum += right;

    return sum;
}

export template <typename NumericType>
Matrix<NumericType> operator-(const Matrix<NumericType> &left, const Matrix<NumericType> &right)
{
    left.validate_sum_subtraction(right);
    Matrix<NumericType> subtraction(left);
    subtraction -= right;

    return subtraction;
}