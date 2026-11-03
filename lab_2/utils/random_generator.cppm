export module random_generator;

import std;

export namespace random_utils
{
    namespace detail
    {
        inline std::mt19937_64 &random_engine()
        {
            static std::mt19937_64 generator(std::random_device{}());
            return generator;
        }

        template <typename T>
        struct random_generator_traits;

        template <typename T>
        concept DistributableInt = std::is_same_v<T, short> ||
                                   std::is_same_v<T, int> ||
                                   std::is_same_v<T, long> ||
                                   std::is_same_v<T, long long> ||
                                   std::is_same_v<T, unsigned short> ||
                                   std::is_same_v<T, unsigned int> ||
                                   std::is_same_v<T, unsigned long> ||
                                   std::is_same_v<T, unsigned long long>;

        template <DistributableInt T>
        struct random_generator_traits<T>
        {
            static T generate(const T &min_value, const T &max_value)
            {
                std::uniform_int_distribution<T> distribution(min_value, max_value);
                return distribution(random_engine());
            }
        };

        template <typename T>
        concept DistributableReal = std::is_same_v<T, float> ||
                                    std::is_same_v<T, double> ||
                                    std::is_same_v<T, long double>;

        template <DistributableReal T>
        struct random_generator_traits<T>
        {
            static T generate(const T &min_value, const T &max_value)
            {
                std::uniform_real_distribution<T> distribution(min_value, max_value);
                return distribution(random_engine());
            }
        };

        template <typename T>
        void validate_range(const T &min_value, const T &max_value)
        {
            if (min_value > max_value)
            {
                throw std::invalid_argument("Min must be <= max");
            }
        }
    }

    template <typename T>
    T generate_random(const T &min_value, const T &max_value)
    {
        detail::validate_range(min_value, max_value);
        return detail::random_generator_traits<T>::generate(min_value, max_value);
    }
}