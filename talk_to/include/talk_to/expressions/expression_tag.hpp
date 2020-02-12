#pragma once

#include <type_traits>

namespace tt::expressions
{
    struct expression_tag {};

    template <typename T>
    constexpr bool is_expression_v = std::is_base_of_v<expression_tag, T>;
}
