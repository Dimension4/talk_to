#pragma once

#include "expression_tags.hpp"
#include "tagged_tuple_expression.hpp"
#include "tagged_container_expression.hpp"

namespace tt::expressions
{
    template <typename BaseComparer>
    struct comparison_dispatcher
    {
        template <typename Lhs, typename Rhs>
        constexpr auto operator()(Lhs&& lhs, Rhs&& rhs) const
        {
            return BaseComparer{}(std::forward<Lhs>(lhs), std::forward<Rhs>(rhs));
        }

        template <typename Rhs, typename ...LTypes>
        constexpr auto operator()(tagged_tuple_expression<any_of_tag, LTypes...> const& lhs, Rhs&& rhs) const
        {

        }
    };
}
