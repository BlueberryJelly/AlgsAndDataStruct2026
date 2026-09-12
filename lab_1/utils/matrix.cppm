export module matrix;

import std;

export template <typename NumericType>
class Matrix final
{
private:
    std::size_t _rows = 0;
    std::size_t _columns = 0;
    NumericType *_data = nullptr;

    void free_data() noexcept
    {
        delete[] _data;
        _data = nullptr;
    }

    void free_matrix() noexcept
    {
        free_data();
        _rows = 0;
        _columns = 0;
    }

    void copy_data(const Matrix<NumericType> &matrix)
    {
        std::size_t size = matrix._rows * matrix._columns;
        _data = new NumericType[size];
        for (std::size_t index = 0; index < size; ++index)
        {
            _data[index] = matrix._data[index];
        }
    }

    void copy_matrix(const Matrix<NumericType> &matrix)
    {
        copy_data(matrix);
        _rows = matrix._rows;
        _columns = matrix._columns;
    }

    void move_data(Matrix<NumericType> &&matrix) noexcept
    {
        _data = matrix._data;
        matrix._data = nullptr;
        matrix._rows = 0;
        matrix._columns = 0;
    }

    void move_matrix(Matrix<NumericType> &&matrix) noexcept
    {
        _rows = matrix._rows;
        _columns = matrix._columns;
        move_data(std::move(matrix));
    }

    void validate_index(const std::size_t row, const std::size_t column) const
    {
        if (row >= _rows || column >= _columns)
        {
            throw std::out_of_range("Incorrect index(indexes) for matrix");
        }
    }

public:
    Matrix() = delete;

    Matrix(const std::size_t rows, const std::size_t columns,
           const NumericType &value)
    {
        if (rows == 0 || columns == 0)
        {
            throw std::length_error("Prohibited size");
        }

        if (rows > std::numeric_limits<std::size_t>::max() / columns)
        {
            throw std::length_error("Size overflow");
        }

        _data = new NumericType[rows * columns];
        _rows = rows;
        _columns = columns;

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

    Matrix(const Matrix<NumericType> &other)
        : _rows(other._rows), _columns(other._columns)
    {
        try
        {
            copy_data(other);
        }
        catch (...)
        {
            free_matrix();
            throw;
        }
    }

    Matrix(Matrix<NumericType> &&other) noexcept
        : _rows(other._rows), _columns(other._columns)
    {
        move_data(std::move(other));
    }

    ~Matrix() noexcept
    {
        free_data();
    }

    Matrix<NumericType> &operator=(Matrix<NumericType> &&other) noexcept
    {
        if (this != &other)
        {
            free_matrix();
            move_matrix(std::move(other));
        }

        return *this;
    }

    Matrix<NumericType> &operator=(const Matrix<NumericType> &other)
    {
        if (this != &other)
        {
            Matrix<NumericType> copy(std::move(*this));

            try
            {
                copy_matrix(other);
            }
            catch (...)
            {
                *this = std::move(copy);
                throw;
            }
        }

        return *this;
    }

    const NumericType &operator[](const std::size_t row, const std::size_t column) const noexcept
    {
        return _data[row * _columns + column];
    }

    NumericType &operator[](const std::size_t row, const std::size_t column) noexcept
    {
        return _data[row * _columns + column];
    }

    const NumericType &at(const std::size_t row, const std::size_t column) const
    {
        validate_index(row, column);

        return _data[row * _columns + column];
    }

    NumericType &at(const std::size_t row, const std::size_t column)
    {
        validate_index(row, column);

        return _data[row * _columns + column];
    }

    bool same_size(const Matrix<NumericType> &other) const noexcept
    {
        return _rows == other._rows && _columns == other._columns;
    }

    void validate_sum_subtraction(const Matrix<NumericType> &other) const
    {
        if (!same_size(other))
        {
            throw std::logic_error("It's impossible to subtract or add");
        }
    }

    Matrix<NumericType> &operator+=(const Matrix<NumericType> &other)
    {
        validate_sum_subtraction(other);
        std::size_t size = _rows * _columns;
        for (std::size_t index = 0; index < size; ++index)
        {
            _data[index] += other._data[index];
        }

        return *this;
    }

    Matrix<NumericType> &operator-=(const Matrix<NumericType> &other)
    {
        validate_sum_subtraction(other);
        std::size_t size = _rows * _columns;
        for (std::size_t index = 0; index < size; ++index)
        {
            _data[index] -= other._data[index];
        }

        return *this;
    }

    Matrix<NumericType> &operator*=(const NumericType &factor) noexcept
    {
        std::size_t size = _rows * _columns;
        for (std::size_t index = 0; index < size; ++index)
        {
            _data[index] *= factor;
        }

        return *this;
    }

    Matrix<NumericType> &operator/=(const NumericType &factor) noexcept
    {
        std::size_t size = _rows * _columns;
        for (std::size_t index = 0; index < size; ++index)
        {
            _data[index] /= factor;
        }

        return *this;
    }

    void multiplyable(const Matrix<NumericType> &other) const
    {
        if (_columns != other._rows)
        {
            throw std::logic_error("It's impossible to multiply");
        }
    }

    NumericType trace() const
    {
        if (!square_matrix())
        {
            throw std::logic_error("Only a square matrix has a trace");
        }

        NumericType trace{};
        for (std::size_t index = 0; index < _rows; ++index)
        {
            trace += (*this)[index, index];
        }

        return trace;
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

    bool square_matrix() const noexcept
    {
        return _rows == _columns;
    }
};