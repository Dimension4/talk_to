#pragma once

#include "expression.hpp"

#include <utility>

namespace tt::expressions
{
    template <typename Tag, typename Container>
    struct container_expression : value_expression
    {
        using tag_t = Tag;
        Container values;

        constexpr container_expression(Tag, Container values) : values(std::move(values)) {}

        constexpr auto operator()() const
        {
            return values;
        }
    };
}
