#pragma once

#include "../utility.hpp"
#include "expression_tag.hpp"

namespace tt::expressions
{
    template <typename Operator, typename Lhs, typename Rhs>
    struct binary_operation : expression_tag
    {
        const Lhs lhs;
        const Rhs rhs;
        const Operator op;

        constexpr binary_operation(Operator op, Lhs lhs, Rhs rhs) :
            lhs(std::move(lhs)), rhs(std::move(rhs)), op(std::move(op)) {}

        constexpr auto operator()() const
        {
            return cexpr_invoke(op, cexpr_invoke(lhs), cexpr_invoke(rhs));
        }

        constexpr explicit operator bool() const
        {
            return cexpr_invoke(*this);
        }
    };
}
