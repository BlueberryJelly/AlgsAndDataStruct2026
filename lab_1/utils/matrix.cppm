export module matrix;

import std;

export template <typename Type>
concept MatrixNumeric = (std::integral<Type> && !std::same_as<Type, bool>) ||
                        std::floating_point<Type> ||
                        std::same_as<Type, std::complex<float>> ||
                        std::same_as<Type, std::complex<double>>;

export template <MatrixNumeric NumericType>
class Matrix final
{
private:
    std::size_t _rows = 0;
    std::size_t _columns = 0;
    NumericType *_data = nullptr;
    static constexpr double _epsilon = 1e-9;

    static void validate_dimensions(const std::size_t rows, const std::size_t columns)
    {
        if (rows == 0 || columns == 0)
        {
            throw std::length_error("Prohibited size");
        }

        if (rows > std::numeric_limits<std::size_t>::max() / columns)
        {
            throw std::length_error("Size overflow");
        }
    }

    static void validate_range(const NumericType &min, const NumericType &max)
    {
        if constexpr (std::same_as<NumericType, std::complex<float>> ||
                      std::same_as<NumericType, std::complex<double>>)
        {
            if (min.real() > max.real() || min.imag() > max.imag())
            {
                throw std::invalid_argument("Min must be <= max component-wise");
            }
        }
        else
        {
            if (min > max)
            {
                throw std::invalid_argument("Min must be <= max");
            }
        }
    }

    void allocate_uninitialized(const std::size_t rows, const std::size_t columns)
    {
        validate_dimensions(rows, columns);
        _data = new NumericType[rows * columns];
        _rows = rows;
        _columns = columns;
    }

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

    static std::mt19937_64 &random_engine()
    {
        static std::mt19937_64 generator(std::random_device{}());
        return generator;
    }

    static NumericType generate_random_value(std::mt19937_64 &generator,
                                             const NumericType &min,
                                             const NumericType &max)
    {
        if constexpr (std::floating_point<NumericType>)
        {
            std::uniform_real_distribution<NumericType> distribution(min, max);
            return distribution(generator);
        }
        else if constexpr (std::same_as<NumericType, std::complex<float>> ||
                           std::same_as<NumericType, std::complex<double>>)
        {
            using ValueType = typename NumericType::value_type;
            std::uniform_real_distribution<ValueType> real_distribution(min.real(), max.real());
            std::uniform_real_distribution<ValueType> imag_distribution(min.imag(), max.imag());
            return NumericType(real_distribution(generator), imag_distribution(generator));
        }
        else
        {
            using DistType = std::conditional_t<(sizeof(NumericType) < sizeof(short)), int, NumericType>;
            std::uniform_int_distribution<DistType> distribution(static_cast<DistType>(min),
                                                                 static_cast<DistType>(max));
            return static_cast<NumericType>(distribution(generator));
        }
    }

    template <typename Operation>
    Matrix<NumericType> &apply_elementwise(const Matrix<NumericType> &other,
                                           Operation operation)
    {
        std::size_t size = _rows * _columns;
        for (std::size_t index = 0; index < size; ++index)
        {
            operation(_data[index], other._data[index]);
        }
        return *this;
    }

    template <typename Operation>
    Matrix<NumericType> &apply_scalar_elementwise(const NumericType &factor,
                                                  Operation operation)
    {
        std::size_t size = _rows * _columns;
        for (std::size_t index = 0; index < size; ++index)
        {
            operation(_data[index], factor);
        }
        return *this;
    }

    static bool approximately_equal(const NumericType &left, const NumericType &right) noexcept
    {
        if constexpr (std::floating_point<NumericType> ||
                      std::same_as<NumericType, std::complex<float>> ||
                      std::same_as<NumericType, std::complex<double>>)
        {
            return std::abs(left - right) <= static_cast<decltype(std::abs(left - right))>(_epsilon);
        }
        else
        {
            return left == right;
        }
    }

public:
    Matrix() = delete;

    Matrix(const std::size_t rows, const std::size_t columns,
           const NumericType &value)
    {
        allocate_uninitialized(rows, columns);

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

    Matrix(const std::size_t rows, const std::size_t columns,
           const NumericType &min, const NumericType &max)
    {
        validate_range(min, max);
        allocate_uninitialized(rows, columns);

        try
        {
            auto &generator = random_engine();
            std::size_t size = _rows * _columns;
            for (std::size_t index = 0; index < size; ++index)
            {
                _data[index] = generate_random_value(generator, min, max);
            }
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

        return apply_elementwise(other, [](NumericType &left, const NumericType &right)
                                 { left += right; });
    }

    Matrix<NumericType> &operator-=(const Matrix<NumericType> &other)
    {
        validate_sum_subtraction(other);

        return apply_elementwise(other, [](NumericType &left, const NumericType &right)
                                 { left -= right; });
    }

    Matrix<NumericType> &operator*=(const NumericType &factor)
    {
        return apply_scalar_elementwise(factor, [](NumericType &left, const NumericType &right)
                                        { left *= right; });
    }

    Matrix<NumericType> &operator/=(const NumericType &factor)
    {
        if constexpr (std::integral<NumericType>)
        {
            if (factor == 0)
            {
                throw std::invalid_argument("Division by zero");
            }
        }

        return apply_scalar_elementwise(factor, [](NumericType &left, const NumericType &right)
                                        { left /= right; });
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

    bool equal(const Matrix<NumericType> &other) const noexcept
    {
        if (!same_size(other))
        {
            return false;
        }

        std::size_t size = _rows * _columns;
        for (std::size_t index = 0; index < size; ++index)
        {
            if (!approximately_equal(_data[index], other._data[index]))
            {
                return false;
            }
        }

        return true;
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

    double get_epsilon() const noexcept
    {
        return _epsilon;
    }

    bool square_matrix() const noexcept
    {
        return _rows == _columns;
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
};