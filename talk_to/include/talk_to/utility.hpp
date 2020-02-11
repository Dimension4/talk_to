#pragma once

#include <type_traits>
#include <utility>
#include <tuple>

namespace tt
{
    // temporary hack because std::invoke is not constexpr, ffs

    template <typename Callable, typename ...Args>
    constexpr auto cexpr_invoke(Callable&& func, Args&& ...args) noexcept(std::is_nothrow_invocable_v<Callable, Args...>)
    {
        return std::forward<Callable>(func)(std::forward<Args>(args)...);
    }
}
