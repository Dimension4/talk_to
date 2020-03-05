#pragma once

#include <functional>
#include <type_traits>

#include "binary_operation.hpp"
#include "constant_value.hpp"
#include "op_codes.hpp"

namespace tt::expressions
{
    template <typename Operator, typename Lhs, typename Rhs>
    [[nodiscard]] constexpr decltype(auto) left_most(binary_operation<Operator, Lhs, Rhs> const& expr)
    {
        return left_most(expr.lhs);
    }

    template <typename Expr, typename = std::enable_if_t<is_value_expression_v<Expr>>>
    [[nodiscard]] constexpr decltype(auto) left_most(Expr&& expr)
    {
        return std::forward<Expr>(expr);
    }

    template <typename Operator, typename Lhs, typename Rhs>
    [[nodiscard]] constexpr decltype(auto) right_most(binary_operation<Operator, Lhs, Rhs> const& expr)
    {
        return right_most(expr.rhs);
    }

    template <typename Expr, typename = std::enable_if_t<is_value_expression_v<Expr>>>
    [[nodiscard]] constexpr decltype(auto) right_most(Expr&& expr)
    {
        return std::forward<Expr>(expr);
    }

    template <typename Operator, typename Lhs, typename Rhs>
    [[nodiscard]] constexpr auto construct_tree(Operator&& op, Lhs&& lhs, Rhs&& rhs)
    {
        return binary_operation(std::forward<Operator>(op), to_expr(std::forward<Lhs>(lhs)), to_expr(std::forward<Rhs>(rhs)));
    }

    template <typename Operator, typename OpOperator, typename OpLExpr, typename OpRExpr, typename Rhs>
    [[nodiscard]] constexpr auto construct_tree(Operator&& op, binary_operation<OpOperator, OpLExpr, OpRExpr> lhs, Rhs&& rhs)
    {
        auto expr = construct_tree(std::forward<Operator>(op), right_most(lhs), to_expr(std::forward<Rhs>(rhs))); // todo: maybe std::ref(lhs.rhs)
        return binary_operation(op_and{}, std::move(lhs), std::move(expr));
    }

    template <typename Operator, typename OpOperator, typename OpLExpr, typename OpRExpr, typename Lhs>
    [[nodiscard]] constexpr auto construct_tree(Operator&& op, Lhs&& lhs, binary_operation<OpOperator, OpLExpr, OpRExpr> rhs)
    {
        auto expr = construct_tree(std::forward<Operator>(op), to_expr(std::forward<Lhs>(lhs)), left_most(rhs)); // todo: maybe std::ref(rhs.lhs)
        return binary_operation(op_and{}, std::move(expr), std::move(rhs));
    }

    template <typename Operator, typename LOp, typename LLExpr, typename LRExpr, typename ROp, typename RLExpr, typename RRExpr>
    [[nodiscard]] constexpr auto construct_tree(Operator&& op, binary_operation<LOp, LLExpr, LRExpr> lhs, binary_operation<ROp, RLExpr, RRExpr> rhs)
    {
        return binary_operation(std::forward<Operator>(op), std::move(lhs), std::move(rhs));
    }
}
