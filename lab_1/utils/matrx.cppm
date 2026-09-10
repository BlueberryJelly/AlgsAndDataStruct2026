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
};