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

    void copy_data(const Matrix<NumericType> &matrix)
    {
        _data = new NumericType[_rows * _columns];
        for (std::size_t index = 0; index < _rows * _columns; ++index)
        {
            _data[index] = matrix._data[index];
        }
    }

    void copy_matrix(const Matrix<NumericType> &matrix)
    {
        _rows = matrix._rows;
        _columns = matrix._columns;
        copy_data(matrix);
    }

    void validate_index(const std::size_t row, const std::size_t column)
    {
        if (row >= _rows || column >= _columns)
        {
            throw std::invalid_argument("Incorrect indexe(s) for matrix");
        }
    }

public:
    Matrix() = delete;

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

    explicit Matrix(const Matrix<NumericType> &other)
        : _rows(other._rows), _columns(other._columns)
    {
        copy_data();
    }

    explicit Matrix(Matrix<NumericType> &&other)
        : _rows(other._rows), _columns(other._columns)
    {
        _data = other._data;
        other._data = nullptr;
        other._rows = 0;
        other._columns = 0;
    }

    ~Matrix() noexcept
    {
        free_matrix();
    }

    const NumericType &operator[](const std::size_t row, const std::size_t column) const
    {
        validate_index(row, column);
        return _data[row * _columns + column];
    }

    NumericType &operator[](const std::size_t row, const std::size_t column)
    {
        validate_index(row, column);
        return _data[row * _columns + column];
    }

    std::size_t get_rows() const noexcept
    {
        return _rows;
    }

    std::size_t get_columns() const noexcept
    {
        return _columns;
    }

    const NumericType *get_data() const noexcept
    {
        return _data;
    }
};