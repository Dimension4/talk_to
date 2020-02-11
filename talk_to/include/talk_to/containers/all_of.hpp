#pragma once

#include "../type_traits.hpp"
#include "../tagged_operand.hpp"

#include <type_traits>
#include <tuple>
#include <algorithm>
#include <functional>

namespace tt
{
    inline namespace tags
    {
        struct all_of_tag {};
    }

    namespace detail
    {
        template <typename Lhs, typename Rhs, typename BinaryOp>
        [[nodiscard]] constexpr auto all_of(Lhs&& lhs, Rhs&& rhs, BinaryOp func)
        {
            if constexpr (is_container_v<Rhs>)
            {
                return std::all_of(cbegin(rhs), cend(rhs), [&](auto&& val)
                {
                    return func(lhs, std::forward<decltype(val)>(val));
                });
            }
            else
            {
                return std::apply(
                    [&](auto&& ...vals) { return (func(lhs, std::forward<decltype(vals)>(vals)) && ...); }, std::forward<Rhs>(rhs));
            }
        }
    }

    inline namespace operators
    {
        template <typename T, typename ...Args>
        [[nodiscard]] constexpr bool operator==(T&& lhs, tagged_operand<all_of_tag, Args...> const& rhs)
        {
            return detail::all_of(std::forward<T>(lhs), rhs.values, std::equal_to<>{});
        }
    }

    template <typename ...Args>
    [[nodiscard]] constexpr auto all_of(Args&&... values)
    {
        return make_operand(all_of_tag{}, std::forward<Args>(values)...);
    }
}
