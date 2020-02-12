#pragma once

#include <type_traits>
#include <string_view>

#include "expression_tag.hpp"

namespace tt::expressions
{
    template <typename ValueType>
    struct value_expression : expression_tag
    {
        const ValueType value;

        constexpr explicit value_expression(ValueType value) : value(std::move(value)) {}

        constexpr auto operator()() const noexcept // todo: maybe add support for callables
        {
            return value;
        }
    };

    template <std::size_t N>
    constexpr auto to_expr(const char(&value)[N])
    {
        return value_expression(std::string_view(value));
    }

    constexpr auto to_expr(expression_tag&& value)
    {
        return value;
    }

    template <typename T>
    constexpr auto to_expr(T&& value)
    {
        return value_expression(std::forward<T>(value));
    }
}
