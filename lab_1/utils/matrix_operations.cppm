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
Matrix<NumericType> operator*(const Matrix<NumericType> &left, const NumericType &scalar)
{
    Matrix<NumericType> product(left);
    product *= scalar;

    return product;
}

export template <MatrixNumeric NumericType>
Matrix<NumericType> operator*(const NumericType &scalar, const Matrix<NumericType> &right)
{
    Matrix<NumericType> product(right);
    product *= scalar;

    return product;
}

export template <MatrixNumeric NumericType>
Matrix<NumericType> operator/(const Matrix<NumericType> &left, const NumericType &scalar)
{
    left.validate_scalar_division(scalar);
    Matrix<NumericType> quotient(left);
    quotient /= scalar;

    return quotient;
}

export template <MatrixNumeric NumericType>
std::ostream &operator<<(std::ostream &oss, const Matrix<NumericType> &matrix)
{
    for (std::size_t row = 0; row < matrix.get_rows(); ++row)
    {
        for (std::size_t column = 0; column < matrix.get_columns(); ++column)
        {
            oss << matrix[row, column];
            if (column + 1 == matrix.get_columns())
            {
                continue;
            }
            else
            {
                oss << ", ";
            }
        }
        oss << "\n";
    }

    return oss;
}

bool square_matrix(const Matrix<NumericType> &matrix) noexcept
{
    return matrix.get_rows() == matrix.get_columns();
}

NumericType trace(const Matrix<NumericType> &matrix)
{
    if (!square_matrix(matrix))
    {
        throw std::logic_error("Only a square matrix has a trace");
    }

    NumericType trace{0};
    for (std::size_t index = 0; index < matrix.get_rows(); ++index)
    {
        trace += matrix[index, index];
    }

    return trace;
}

export template <MatrixNumeric NumericType>
Matrix<NumericType> get_element_minor(const Matrix<NumericType> &matrix,
                                      const std::size_t skip_row,
                                      const std::size_t skip_column)
{
    if (!square_matrix(matrix))
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
    if (!square_matrix(matrix))
    {
        throw std::logic_error("Only a square matrix has a determinant");
    }

    if (matrix.get_rows() == 1)
    {
        return matrix[0, 0];
    }
    else if (matrix.get_rows() == 2)
    {
        return matrix[0, 0] * matrix[1, 1] - matrix[0, 1] * matrix[1, 0];
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