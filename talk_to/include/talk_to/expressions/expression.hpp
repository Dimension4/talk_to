#pragma once

#include "../type_traits.hpp"

namespace tt::expressions
{
    struct expression {};
    struct value_expression : expression {};

    template <typename T>
    constexpr bool is_expression_v = std::is_base_of_v<expression, remove_cvref_t<T>>;

    template <typename T>
    constexpr bool is_value_expression_v = std::is_base_of_v<value_expression, remove_cvref_t<T>>;

}
