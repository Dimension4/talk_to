#pragma once

#include <type_traits>
#include <utility>

namespace tt
{
    namespace detail
    {
        template <typename T>
        auto is_container(int) -> decltype(
            void(),
            cbegin(std::declval<T>()) == cend(std::declval<T>()),
            begin(std::declval<T>()) == end(std::declval<T>()),
            std::true_type{});

        template <typename T>
        std::false_type is_container(...);
    }

    template <typename T>
    constexpr bool is_container_v = decltype(detail::is_container<T>(0))::value;


    template <typename Instantiation, template <typename...> typename Template>
    struct is_instantiation_of : std::false_type {};

    template <template <typename...> typename Instantiation, typename ...Args>
    struct is_instantiation_of<Instantiation<Args...>, Instantiation> : std::true_type {};

    template <typename Instantiation, template <typename...> typename Template>
    constexpr bool is_instantiation_of_v = is_instantiation_of<Instantiation, Template>::value;

    template <typename T>
    using remove_cvref_t = std::remove_reference_t<std::remove_cv_t<T>>;
}
