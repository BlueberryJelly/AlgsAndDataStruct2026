export module inverse_matrix;

import std;
import matrix;
import matrix_operations;

export template <MatrixNumeric NumericType>
Matrix<NumericType> inverse(const Matrix<NumericType> &matrix)
{
    if (!square_matrix(matrix))
    {
        throw std::logic_error("Only a square matrix has an inverse matrix");
    }

    NumericType determinant = get_determinant(matrix);

    if (std::abs(determinant - NumericType{0}) <= matrix.get_epsilon())
    {
        throw std::logic_error("Matrix is singular, inverse does not exist");
    }

    if (matrix.get_rows() == 1)
    {
        return Matrix<NumericType>(1, 1, NumericType{1} / determinant);
    }

    Matrix<NumericType> inversed(matrix.get_rows(), matrix.get_columns(), NumericType{0});
    for (std::size_t row = 0; row < inversed.get_rows(); ++row)
    {
        for (std::size_t column = 0; column < inversed.get_columns(); ++column)
        {
            NumericType sign = ((row + column) % 2 == 0) ? NumericType{1} : NumericType{-1};
            inversed[row, column] = sign * get_determinant(get_element_minor(matrix, row, column));
        }
    }

    inversed = transpose(inversed);

    return inversed /= determinant;
}