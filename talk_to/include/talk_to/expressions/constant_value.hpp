#pragma once

#include <type_traits>
#include <string_view>

#include "expression.hpp"

namespace tt::expressions
{
    template <typename ValueType>
    struct constant_value : value_expression
    {
        const ValueType value;

        constexpr explicit constant_value(ValueType value) : value(std::move(value)) {}

        constexpr auto operator()() const noexcept // todo: maybe add support for callables
        {
            return value;
        }
    };

    template <std::size_t N>
    constexpr auto to_expr(const char(&value)[N])
    {
        return constant_value(std::string_view(value));
    }

    template <typename T>
    constexpr auto to_expr(T&& value)
    {
        if constexpr (is_expression_v<T>)
            return std::forward<T>(value);
        else
            return constant_value(std::forward<T>(value));
    }
}
