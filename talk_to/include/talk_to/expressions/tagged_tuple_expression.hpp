#pragma once

#include "expression.hpp"
#include "../utility.hpp"

#include <tuple>

namespace tt::expressions
{
    template <typename Tag, typename ...Types>
    struct tagged_tuple_expression : value_expression
    {
        using tag_t = Tag;
        std::tuple<Types...> values;

        constexpr explicit tagged_tuple_expression(Tag, Types... values) : values(std::move(values)...) { }
        constexpr tagged_tuple_expression(Tag, std::tuple<Types> values) : values(std::move(values)) {}

        constexpr auto operator()() const
        {
            return values;
        }
    };
}
