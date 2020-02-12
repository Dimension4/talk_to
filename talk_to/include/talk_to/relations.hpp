#pragma once

#include <type_traits>
#include <string_view>

#include "expressions/expression_tree.hpp"

namespace tt
{
    template <typename Lhs, typename Rhs>
    using enable_relational_operator_t = std::enable_if_t<tt::expressions::is_expression_v<Lhs> || tt::expressions::is_expression_v<Rhs>>;

    template <typename T>
    [[nodiscard]] constexpr auto rel(T&& value) noexcept
    {
        return expressions::to_expr(std::forward<T>(value));
    }
}


template <typename Lhs, typename Rhs, typename = tt::enable_relational_operator_t<Lhs, Rhs>>
[[nodiscard]] constexpr auto operator==(Lhs&& lhs, Rhs&& rhs)
{
    using namespace tt::expressions;
    return construct_tree(op_equal_to{}, std::forward<Lhs>(lhs), std::forward<Rhs>(rhs));
}

template <typename Lhs, typename Rhs, typename = tt::enable_relational_operator_t<Lhs, Rhs>>
[[nodiscard]] constexpr auto operator!=(Lhs&& lhs, Rhs&& rhs)
{
    using namespace tt::expressions;
    return construct_tree(op_not_equal_to{}, std::forward<Lhs>(lhs), std::forward<Rhs>(rhs));
}

template <typename Lhs, typename Rhs, typename = tt::enable_relational_operator_t<Lhs, Rhs>>
[[nodiscard]] constexpr auto operator<(Lhs&& lhs, Rhs&& rhs)
{
    using namespace tt::expressions;
    return construct_tree(op_less{}, std::forward<Lhs>(lhs), std::forward<Rhs>(rhs));
}

template <typename Lhs, typename Rhs, typename = tt::enable_relational_operator_t<Lhs, Rhs>>
[[nodiscard]] constexpr auto operator<=(Lhs&& lhs, Rhs&& rhs)
{
    using namespace tt::expressions;
    return construct_tree(op_less_equal{}, std::forward<Lhs>(lhs), std::forward<Rhs>(rhs));
}

template <typename Lhs, typename Rhs, typename = tt::enable_relational_operator_t<Lhs, Rhs>>
[[nodiscard]] constexpr auto operator>(Lhs&& lhs, Rhs&& rhs)
{
    using namespace tt::expressions;
    return construct_tree(op_greater{}, std::forward<Lhs>(lhs), std::forward<Rhs>(rhs));
}

template <typename Lhs, typename Rhs, typename = tt::enable_relational_operator_t<Lhs, Rhs>>
[[nodiscard]] constexpr auto operator>=(Lhs&& lhs, Rhs&& rhs)
{
    using namespace tt::expressions;
    return construct_tree(op_greater_equal{}, std::forward<Lhs>(lhs), std::forward<Rhs>(rhs));
}