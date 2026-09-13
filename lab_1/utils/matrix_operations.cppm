export module math_ops_for_matrix;

import std;
import matrix;

export template <MatrixNumeric NumericType>
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

    return sum += right;
}

export template <MatrixNumeric NumericType>
Matrix<NumericType> operator-(const Matrix<NumericType> &left, const Matrix<NumericType> &right)
{
    left.validate_sum_subtraction(right);
    Matrix<NumericType> subtraction(left);

    return subtraction -= right;
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
Matrix<NumericType> operator*(const Matrix<NumericType> &left, const NumericType &scalar)
{
    Matrix<NumericType> product(left);

    return product *= scalar;
}

export template <MatrixNumeric NumericType>
Matrix<NumericType> operator*(const NumericType &scalar, const Matrix<NumericType> &right)
{
    Matrix<NumericType> product(right);

    return product *= scalar;
}

export template <MatrixNumeric NumericType>
Matrix<NumericType> operator/(const Matrix<NumericType> &left, const NumericType &scalar)
{
    left.validate_scalar_division(scalar);
    Matrix<NumericType> quotient(left);

    return quotient /= scalar;
}

Matrix<NumericType> get_element_minor(const std::size_t skip_row, const std::size_t skip_column) const
{
    if (!square_matrix())
    {
        throw std::logic_error("Only a square matrix has an element minor");
    }

    validate_index(skip_row, skip_column);

    Matrix<NumericType> element_minor(_rows - 1, _columns - 1, NumericType{0});

    std::size_t dest_row = 0;
    for (std::size_t row = 0; row < _rows; ++row)
    {
        if (row == skip_row)
        {
            continue;
        }

        std::size_t dest_column = 0;
        for (std::size_t column = 0; column < _columns; ++column)
        {
            if (column == skip_column)
            {
                continue;
            }

            element_minor[dest_row, dest_column] = (*this)[row, column];
            ++dest_column;
        }
        ++dest_row;
    }

    return element_minor;
}

NumericType get_determinant() const
{
    if (!square_matrix())
    {
        throw std::logic_error("Only a square matrix has a determinant");
    }

    if (_rows == 1)
    {
        return (*this)[0, 0];
    }

    NumericType determinant{};
    for (std::size_t column = 0; column < _columns; ++column)
    {
        const NumericType &element = (*this)[0, column];

        if (approximately_equal(element, NumericType{0}))
        {
            continue;
        }

        NumericType sign = (column % 2 == 0) ? NumericType{1} : NumericType{-1};
        determinant += sign * element * get_element_minor(0, column).get_determinant();
    }

    return determinant;
}

Matrix<NumericType> transpose() const
{
    Matrix<NumericType> transposed_matrix(_columns, _rows, 0);
    for (std::size_t row = 0; row < _rows; ++row)
    {
        for (std::size_t column = 0; column < _columns; ++column)
        {
            transposed_matrix[column, row] = (*this)[row, column];
        }
    }

    return transposed_matrix;
}