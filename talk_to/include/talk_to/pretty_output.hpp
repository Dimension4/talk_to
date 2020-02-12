#pragma once

#include "expressions/binary_operation.hpp"
#include "expressions/value_expression.hpp"
#include "expressions/op_codes.hpp"

#include <ostream>

namespace tt::expressions
{
    inline std::ostream& operator<<(std::ostream& os, op_and)
    {
        return os << "&&";
    }

    inline std::ostream& operator<<(std::ostream& os, op_less)
    {
        return os << "<";
    }

    inline std::ostream& operator<<(std::ostream& os, op_less_equal)
    {
        return os << "<=";
    }

    inline std::ostream& operator<<(std::ostream& os, op_greater)
    {
        return os << ">";
    }

    inline std::ostream& operator<<(std::ostream& os, op_greater_equal)
    {
        return os << ">=";
    }

    inline std::ostream& operator<<(std::ostream& os, op_equal_to)
    {
        return os << "==";
    }

    inline std::ostream& operator<<(std::ostream& os, op_not_equal_to)
    {
        return os << "!=";
    }

    template <typename Op, typename Lhs, typename Rhs>
    std::ostream& operator<<(std::ostream& os, binary_operation<Op, Lhs, Rhs> const& op)
    {
        return os << '(' << op.lhs << ' ' << op.op << ' ' << op.rhs << ')';
    }

    namespace detail
    {
        template <typename T>
        auto supports_ostream_helper(...) -> std::false_type;

        template <typename T>
        auto supports_ostream_helper(int) -> decltype(
            void(),
            std::declval<std::ostream>() << std::declval<T>(),
            std::true_type{}
        );

        template<typename T>
        constexpr bool supports_ostream_v = decltype(supports_ostream_helper<T>(0))::value;


        template <typename T>
        auto is_string_like_helper(...)->std::false_type;

        template <typename T>
        auto is_string_like_helper(int) -> decltype(
            void(),
            std::string_view{std::declval<T>()},
            std::true_type{}
        );

        template<typename T>
        constexpr bool is_string_like_v = decltype(is_string_like_helper<T>(0))::value;
    }

    template <typename ValType>
    std::ostream& operator<<(std::ostream& os, value_expression<ValType> const& val)
    {
        if constexpr (detail::is_string_like_v<ValType>)
            return os << '"' << val.value << '"';
        else if constexpr (detail::supports_ostream_v<ValType>)
            return os << val.value;
        else
            return os << '[' << typeid(ValType).name() << ']';
    }
}
