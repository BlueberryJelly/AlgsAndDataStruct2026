export module matrix;

import std;

export template <typename NumericType>
class Matrix final
{
private:
    std::size_t _rows = 0;
    std::size_t _columns = 0;
    NumericType *_data = nullptr;

    void free_matrix() noexcept
    {
        delete[] _data;
        _data = nullptr;
        _rows = 0;
        _columns = 0;
    }

    void validate_index(std::size_t row, std::size_t column)
    {
        if (row >= _rows || column >= columns)
        {
            throw std::invalid_argument("Incorrect indexe(s) for matrix");
        }
    }

public:
    Matrix(const std::size_t rows, const std::size_t columns,
           const NumericType &value)
    {
        if (columns != 0 &&
            rows > std::numeric_limits<std::size_t>::max() / columns)
        {
            throw std::length_error("Size overflow");
        }

        _rows = rows;
        _columns = columns;
        _data = new NumericType[_rows * _columns];

        try
        {
            std::fill(_data, _data + _rows * _columns, value);
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

    const NumericType &operator[](const std::size_t row, const std::size_t column) const noexcept
    {
        validate_index(row, column);
        return _data[row * _columns + column];
    }

    NumericType &operator[](const std::size_t row, const std::size_t column) noexcept
    {
        validate_index(row, column);
        return _data[row * _columns + column];
    }
};