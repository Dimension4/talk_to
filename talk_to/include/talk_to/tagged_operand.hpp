#pragma once

#include "type_traits.hpp"

#include <type_traits>
#include <tuple>

namespace tt
{
    template <typename Tag, typename Container>
    struct tagged_operand
    {
        using tag_t = Tag;
        Container values;

        constexpr explicit tagged_operand(Container operand) :
            values(std::move(operand)) { }

        tagged_operand(tagged_operand const&) = delete;
        tagged_operand(tagged_operand&&) = delete;
        tagged_operand& operator=(tagged_operand const&) = delete;
        tagged_operand& operator=(tagged_operand&&) = delete;
    };

    template <typename Tag, typename Container>
    using tagged_container = tagged_operand<Tag, Container>;

    template <typename Tag, typename ...Args>
    using tagged_tuple = tagged_operand<Tag, std::tuple<Args...>>;


    template <typename Tag, typename ...Args, std::enable_if_t<(sizeof...(Args) > 1) || !is_container_v<std::tuple_element_t<0, std::tuple<Args...>>>>* = nullptr>
    [[nodiscard]] constexpr auto make_operand(Tag, Args&& ...args)
    {
        return tagged_tuple<Tag, std::decay_t<Args>...>(std::tuple(std::forward<Args>(args)...));
    }

    template <typename Tag, typename Container, std::enable_if_t<is_container_v<Container>>* = nullptr>
    [[nodiscard]] constexpr auto make_operand(Tag, Container container)
    {
        return tagged_container<Tag, Container>(std::forward<Container>(container));
    }
}
