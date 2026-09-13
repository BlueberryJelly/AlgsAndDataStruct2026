export module matrix_operations;

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

export template <MatrixNumeric NumericType>
Matrix<NumericType> get_element_minor(const Matrix<NumericType> &matrix,
                                      const std::size_t skip_row,
                                      const std::size_t skip_column)
{
    if (!matrix.square_matrix())
    {
        throw std::logic_error("Only a square matrix has an element minor");
    }

    matrix.validate_index(skip_row, skip_column);

    Matrix<NumericType> element_minor(matrix.get_rows() - 1,
                                      matrix.get_columns() - 1,
                                      NumericType{0});

    std::size_t dest_row = 0;
    for (std::size_t row = 0; row < matrix.get_rows(); ++row)
    {
        if (row == skip_row)
        {
            continue;
        }

        std::size_t dest_column = 0;
        for (std::size_t column = 0; column < matrix.get_columns(); ++column)
        {
            if (column == skip_column)
            {
                continue;
            }

            element_minor[dest_row, dest_column] = matrix[row, column];
            ++dest_column;
        }
        ++dest_row;
    }

    return element_minor;
}

export template <MatrixNumeric NumericType>
NumericType get_determinant(const Matrix<NumericType> &matrix)
{
    if (!matrix.square_matrix())
    {
        throw std::logic_error("Only a square matrix has a determinant");
    }

    if (matrix.get_rows() == 1)
    {
        return matrix[0, 0];
    }

    NumericType determinant(0);
    for (std::size_t column = 0; column < matrix.get_columns(); ++column)
    {
        const NumericType &element = matrix[0, column];

        if (std::abs(element - NumericType{0}) <= matrix.get_epsilon())
        {
            continue;
        }

        NumericType sign = (column % 2 == 0) ? NumericType{1} : NumericType{-1};
        determinant += sign * element * get_determinant(get_element_minor(matrix, 0, column));
    }

    return determinant;
}

export template <MatrixNumeric NumericType>
Matrix<NumericType> transpose(const Matrix<NumericType> &matrix)
{
    Matrix<NumericType> transposed(matrix.get_columns(), matrix.get_rows(), NumericType{0});
    for (std::size_t row = 0; row < matrix.get_rows(); ++row)
    {
        for (std::size_t column = 0; column < matrix.get_columns(); ++column)
        {
            transposed[column, row] = matrix[row, column];
        }
    }

    return transposed;
}