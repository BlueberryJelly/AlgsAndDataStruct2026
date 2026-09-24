export module matrix;

import std;

template <typename T>
struct extract_real_type
{
    using type = T;
};

template <typename T>
struct extract_real_type<std::complex<T>>
{
    using type = T;
};

template <typename T>
using extract_real_type_t = typename extract_real_type<T>::type;

template <typename T>
struct is_complex : std::false_type {};

template <typename T>
struct is_complex<std::complex<T>> : std::true_type {};

template <typename T>
concept IsComplex = is_complex<T>::value; 

export template <typename Type>
concept MatrixNumeric = std::same_as<Type, int> ||
                        std::floating_point<Type> ||
                        IsComplex<Type>;

export template <MatrixNumeric NumericType>
class Matrix final
{
private:
    std::size_t _rows = 0;
    std::size_t _columns = 0;
    std::size_t _size = 0;
    NumericType *_data = nullptr;

    using RealType = extract_real_type_t<NumericType>;
    static constexpr RealType _epsilon = std::numeric_limits<RealType>::epsilon();

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
        if constexpr (IsComplex<NumericType>)
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
        std::size_t size = rows * columns;
        _data = new NumericType[size];
        _rows = rows;
        _columns = columns;
        _size = size;
    }

    void free_data() noexcept
    {
        delete[] _data;
        _data = nullptr;
    }

    void free_matrix() noexcept
    {
        if (_data != nullptr)
        {
            free_data();
        }
        _rows = 0;
        _columns = 0;
        _size = 0;
    }

    void move_data(Matrix<NumericType> &&source) noexcept
    {
        _data = source._data;
        source._data = nullptr;
        source.free_matrix();
    }

    void move_matrix(Matrix<NumericType> &&source) noexcept
    {
        _rows = source._rows;
        _columns = source._columns;
        _size = source._size;
        move_data(std::move(source));
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
        else if constexpr (IsComplex<NumericType>)
        {
            std::uniform_real_distribution<RealType> real_distribution(min.real(), max.real());
            std::uniform_real_distribution<RealType> imag_distribution(min.imag(), max.imag());
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

public:
    Matrix() = delete;

    Matrix(const std::size_t rows, const std::size_t columns,
           const NumericType &value)
    {
        allocate_uninitialized(rows, columns);

        try
        {
            std::fill(_data, _data + _size, value);
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
            for (std::size_t index = 0; index < _size; ++index)
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
    {
        allocate_uninitialized(other._rows, other._columns);

        try
        {
            for (std::size_t index = 0; index < _size; ++index)
            {
                _data[index] = other._data[index];
            }
        }
        catch (...)
        {
            free_matrix();
            throw;
        }
    }

    Matrix(Matrix<NumericType> &&other) noexcept
        : _rows(other._rows), _columns(other._columns), _size(other._size)
    {
        move_data(std::move(other));
    }

    ~Matrix() noexcept
    {
        free_data();
    }

    Matrix<NumericType> &operator=(const Matrix<NumericType> &other)
    {
        Matrix<NumericType> copy(other);
        std::swap(*this, copy);

        return *this;
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

    const NumericType &operator[](const std::size_t row, const std::size_t column) const noexcept
    {
        return _data[row * _columns + column];
    }

    NumericType &operator[](const std::size_t row, const std::size_t column) noexcept
    {
        return _data[row * _columns + column];
    }

    void validate_index(const std::size_t row, const std::size_t column) const
    {
        if (row >= _rows || column >= _columns)
        {
            throw std::out_of_range("Incorrect index(indexes) for matrix");
        }
    }

    const NumericType &at(const std::size_t row, const std::size_t column) const
    {
        validate_index(row, column);

        return (*this)[row, column];
    }

    NumericType &at(const std::size_t row, const std::size_t column)
    {
        validate_index(row, column);

        return (*this)[row, column];
    }

    bool same_size(const Matrix<NumericType> &other) const noexcept
    {
        return _rows == other._rows && _columns == other._columns;
    }

    void validate_sum_subtraction(const Matrix<NumericType> &other) const
    {
        if (!same_size(other))
        {
            throw std::logic_error("It's impossible to add or subtract");
        }
    }

    Matrix<NumericType> &operator+=(const Matrix<NumericType> &other)
    {
        validate_sum_subtraction(other);

        for (std::size_t index = 0; index < _size; ++index)
        {
            _data[index] += other._data[index];
        }

        return *this;
    }

    Matrix<NumericType> &operator-=(const Matrix<NumericType> &other)
    {
        validate_sum_subtraction(other);

        for (std::size_t index = 0; index < _size; ++index)
        {
            _data[index] -= other._data[index];
        }

        return *this;
    }

    Matrix<NumericType> &operator*=(const NumericType &scalar)
    {
        for (std::size_t index = 0; index < _size; ++index)
        {
            _data[index] *= scalar;
        }

        return *this;
    }

    void validate_scalar_division(const NumericType &scalar) const
    {
        if (std::abs(scalar - NumericType{0}) <= _epsilon)
        {
            throw std::invalid_argument("Division by zero");
        }
    }

    Matrix<NumericType> &operator/=(const NumericType &scalar)
    {
        validate_scalar_division(scalar);

        for (std::size_t index = 0; index < _size; ++index)
        {
            _data[index] /= scalar;
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

    bool equal(const Matrix<NumericType> &other) const noexcept
    {
        if (!same_size(other))
        {
            return false;
        }

        for (std::size_t index = 0; index < _size; ++index)
        {
            if constexpr (std::floating_point<NumericType> || 
                IsComplex<NumericType>)
            {
                if (std::abs(_data[index] - other._data[index]) > _epsilon)
                {
                    return false;
                }
            }
            else
            {
                if (_data[index] != other._data[index])
                {
                    return false;
                }
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

    RealType get_epsilon() const noexcept
    {
        return _epsilon;
    }
};