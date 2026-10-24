export module random_generator;

import std;

export namespace random_utils
{
    namespace detail
    {
        inline std::mt19937_64 &random_engine()
        {
            thread_local std::mt19937_64 generator{[]{
                std::random_device rd;
                std::seed_seq seed{rd(), rd(), rd(), rd()};
                return std::mt19937_64{seed};
            }()};
            return generator;
        }

        template <typename T>
        struct random_generator_traits;

        template <std::integral T>
        struct random_generator_traits<T>
        {
            static T generate(const T &min_value, const T &max_value)
            {
                std::uniform_int_distribution<T> distribution(min_value, max_value);
                return distribution(random_engine());
            }
        };

        template <std::floating_point T>
        struct random_generator_traits<T>
        {
            static T generate(const T &min_value, const T &max_value)
            {
                std::uniform_real_distribution<T> distribution(min_value, max_value);
                return distribution(random_engine());
            }
        };
    }

    template <typename T>
    T generate_random(const T &min_value, const T &max_value)
    {
        return detail::random_generator_traits<T>::generate(min_value, max_value);
    }
}