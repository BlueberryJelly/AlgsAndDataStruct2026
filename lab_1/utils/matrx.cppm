export module matrix;

import std;

export template <typename Type>
struct is_complex : std::false_type
{
};

export template <typename Type>
struct is_complex<std::complex<Type>> : std::true_type
{
};

export template <typename NumericType>
concept Numeric = std::integral<NumericType> ||
                  std::floating_point<NumericType> ||
                  is_complex<std::remove_cv_t<NumericType>>::value;

export template <typename NumericType>
class Matrix final
{
private:
    std::size_t _rows = 0;
    std::size_t _columns = 0;
    NumericType *_matrix = nullptr;

    void free_matrix() noexcept
    {
        delete[] _matrix;
        _matrix = nullptr;
        _rows = 0;
        _columns = 0;
    }

public:
    Matrix(const std::size_t rows, const std::size_t columns, const NumericType &value)
    {
        if (columns != 0 && rows > std::numeric_limits<std::size_t>::max() / columns)
        {
            throw std::length_error("Size overflow");
        }

        _rows = rows;
        _columns = columns;
        _matrix = new NumericType[_rows * _columns];

        try
        {
            std::fill(_matrix, _matrix + _rows * _columns, value);
        }
        catch (...)
        {
            free_matrix();
            throw;
        }
    }

    ~Matrix() noexcept
    {
        free_matrix();
    }
};