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
        struct any_of_tag {};
    }

    namespace detail
    {
        template <typename Lhs, typename Rhs, typename BinaryOp>
        [[nodiscard]] constexpr auto any_of(Lhs&& lhs, Rhs&& rhs, BinaryOp func)
        {
            if constexpr (is_container_v<Lhs> && is_container_v<Rhs>)
            {
                return std::any_of(cbegin(lhs), cend(lhs), [&](auto&& left)
                {
                    return std::any_of(cbegin(rhs), cend(rhs), [&](auto&& right)
                    {
                        return func(left, right);
                    });
                });
            }
            else if constexpr (is_container_v<Lhs>)
            {
                return std::any_of(cbegin(lhs), cend(lhs), [&](auto&& left)
                {
                    return func(std::forward<decltype(left)>(left), rhs);
                });
            }
            else if constexpr (is_container_v<Rhs>)
            {
                return std::any_of(cbegin(rhs), cend(rhs), [&](auto&& right)
                {
                    return func(lhs, std::forward<Rhs>(right));
                });
            }
            else
            {
                return std::apply(
                    [&](auto&& ...vals) { return (func(lhs, std::forward<decltype(vals)>(vals)) && ...); },
                    std::forward<Rhs>(rhs));
            }
        }
    }

    inline namespace operators
    {
        template <typename T, typename ...Args>
        [[nodiscard]] constexpr bool operator==(T&& lhs, tagged_operand<any_of_tag, Args...> const& rhs)
        {
            return detail::any_of(std::forward<T>(lhs), rhs.values, std::equal_to<>{});
        }

        template <typename T, typename ...Args>
        [[nodiscard]] constexpr bool operator==(tagged_operand<any_of_tag, Args...> const& lhs, T&& rhs)
        {
            return detail::any_of(lhs.values, std::forward<T>(rhs), std::equal_to<>{});
        }

        template <typename ...LArgs, typename ...RArgs>
        [[nodiscard]] constexpr bool operator==(tagged_operand<any_of_tag, LArgs...> const& lhs, tagged_operand<any_of_tag, RArgs...> const& rhs)
        {
            return detail::any_of(lhs.values, rhs.values, std::equal_to<>{});
        }
    }

    template <typename ...Args>
    [[nodiscard]] constexpr auto any_of(Args&&... values)
    {
        return make_operand(any_of_tag{}, std::forward<Args>(values)...);
    }
}
