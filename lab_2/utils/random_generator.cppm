export module random_generator;

import std;

export namespace random_utils
{
    namespace detail
    {
        inline std::mt19937_64 &random_engine(std::uint64_t seed)
        {
            static std::mt19937_64 generator(seed);
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
            static T generate(const T &min_value, const T &max_value, std::uint64_t seed)
            {
                std::uniform_int_distribution<T> distribution(min_value, max_value);
                return distribution(random_engine(seed));
            }
        };

        template <std::floating_point T>
        struct random_generator_traits<T>
        {
            static T generate(const T &min_value, const T &max_value, std::uint64_t seed)
            {
                std::uniform_real_distribution<T> distribution(min_value, max_value);
                return distribution(random_engine(seed));
            }
        };

        template <typename T>
        void validate_range(const T &min_value, const T &max_value)
        {
            if (min_value > max_value)
            {
                throw std::invalid_argument("Min must be <= max.");
            }
        }
    }

    template <typename T>
    T generate_random(const T &min_value, const T &max_value, std::uint64_t seed)
    {
        detail::validate_range(min_value, max_value);
        return detail::random_generator_traits<T>::generate(min_value, max_value, seed);
    }
}