#pragma once

#include "../type_traits.hpp"
#include "../utility.hpp"

#include <tuple>

namespace tt::expressions
{
    struct expression_base {};

    template <typename T>
    constexpr bool is_expression_v = std::is_base_of_v<expression_base, T>;

    namespace detail
    {
        template <typename Operation, typename Relation>
        struct op_is_relation_helper : std::false_type {};

        template <template <typename...> typename Operation, typename Relation, typename ...Operands>
        struct op_is_relation_helper<Operation<Relation, Operands...>, Relation> : std::true_type {};
    }

    template <typename ValueType>
    struct value_expression : expression_base
    {
        const ValueType value;

        constexpr explicit value_expression(ValueType value) : value(std::move(value)) {}

        constexpr operator ValueType() const noexcept // todo: probably remove
        {
            return value;
        }

        constexpr auto operator()() const noexcept
        {
            return value;
        }

        constexpr auto left() const noexcept
        {
            return value;
        }

        constexpr auto right() const noexcept
        {
            return value;
        }
    };

    template <typename T>
    constexpr bool is_value_expr_v = is_instantiation_of_v<T, value_expression>;

    template <typename T>
    constexpr bool is_value_expr_or_value_v = is_value_expr_v<T> || !std::is_base_of_v<expression_base, T>;

    struct relational_operation_base : expression_base {};

    template <typename Relation, typename ...Operands>
    struct relational_operation : relational_operation_base // todo: maybe make a binary tree instead
    {
        using comparer_t = Relation;

        Relation relation{};
        std::tuple<Operands...> operands{};

        constexpr explicit relational_operation(Relation relation, Operands ...operands) :
            relation(std::move(relation)), operands(std::move(operands)...) {}

        constexpr explicit relational_operation(Relation relation, std::tuple<Operands...> operands) :
            relation(std::move(relation)), operands(std::move(operands)) {}

        constexpr auto left() const noexcept
        {
            return get<0>(operands);
        }

        constexpr auto right() const noexcept
        {
            return get<sizeof...(Operands) - 1>(operands);
        }

    private:
        template <std::size_t ...I>
        [[nodiscard]] constexpr auto evaluate(std::index_sequence<I...>) const
        {
            using namespace std;
            return (cexpr_invoke(relation, cexpr_invoke(get<I>(operands)), cexpr_invoke(get<I + 1>(operands))) && ...);
        }

    public:
        [[nodiscard]] constexpr auto operator()() const
        {
            static_assert(sizeof...(Operands) >= 2, "A relational operation requires at least 2 operands.");
            return evaluate(std::make_index_sequence<sizeof...(Operands) - 1>{});
        }

        [[nodiscard]] constexpr operator bool() const noexcept(std::is_nothrow_invocable_v<relational_operation>)
        {
            return cexpr_invoke(*this);
        }
    };

    template <typename ...Operations>
    struct relational_operation_conjunction : relational_operation_base
    {
        std::tuple<Operations...> operations;

        constexpr explicit relational_operation_conjunction(Operations ...operations) :
            operations(std::move(operations)...) {}

        constexpr explicit relational_operation_conjunction(std::tuple<Operations...> operations) :
            operations(std::move(operations)) {}

        constexpr auto left() const noexcept
        {
            return get<0>(operations).left();
        }

        constexpr auto right() const noexcept
        {
            return get<sizeof...(Operations) - 1>(operations).right();
        }

        [[nodiscard]] constexpr auto operator()() const
        {
            return std::apply([](auto&& ...ops) { return (cexpr_invoke(std::forward<decltype(ops)>(ops)) && ...); }, operations);
        }

        [[nodiscard]] constexpr operator bool() const noexcept(std::is_nothrow_invocable_v<relational_operation_conjunction>)
        {
            return cexpr_invoke(*this);
        }
    };

    template <typename T>
    constexpr bool is_relational_op_conjunction_v = is_instantiation_of_v<T, relational_operation>;

    template <typename T>
    using is_relational_op = std::is_base_of<relational_operation_base, T>;

    template <typename T>
    constexpr bool is_relational_op_v = is_relational_op<T>::value;

    template <typename Operation, typename Relation>
    constexpr bool op_is_relation_v = detail::op_is_relation_helper<Operation, Relation>::value;

    namespace detail
    {
        template <typename T, typename U>
        auto have_same_relations_helper(...) -> std::false_type;

        template <typename T, typename U>
        auto have_same_relations_helper(int) -> std::is_same<typename T::comparer_t, typename U::comparer_t>;
    }

    template <typename T, typename U>
    constexpr bool have_same_relations_v = decltype(detail::have_same_relations_helper<T, U>(0))::value;

    //
    // expression tree construction
    //

    template <typename Lhs, typename Rhs, typename = std::enable_if_t<std::is_same_v<typename Lhs::comparer_t, typename Rhs::comparer_t>>>
    constexpr auto merge_same_kinded_relational_ops(Lhs&& lhs, Rhs&& rhs)
    {
        using relation_t = typename Lhs::comparer_t;
        return relational_operation(relation_t{}, std::tuple_cat(std::move(lhs.operands), std::move(rhs.operands)));
    }

    template <typename Operation, typename Operand>
    constexpr auto append_operand(Operation&& operation, Operand&& operand)
    {
        using relation_t = typename Operation::comparer_t;
        return relational_operation(relation_t{}, std::tuple_cat(std::move(operation.operands), std::tuple(std::forward<Operand>(operand))));
    }

    template <typename Operation, typename Operand>
    constexpr auto prepend_operand(Operation&& operation, Operand&& operand)
    {
        using relation_t = typename Operation::comparer_t;
        return relational_operation(relation_t{}, std::tuple_cat(std::tuple(std::forward<Operand>(operand))), std::move(operation.operands));
    }

    template <typename T>
    constexpr auto wrap_if_not_expression(T&& value)
    {
        if constexpr (is_expression_v<T>)
        {
            return value;
        }
        else
        {
            return value_expression(std::forward<T>(value));
        }
    }

    template <typename Relation, typename Lhs, typename Rhs>
    constexpr auto make_operation_conjunction(Relation&& relation, Lhs&& lhs, Rhs&& rhs)
    {
        auto op = relational_operation(std::forward<Relation>(relation), lhs.right(), wrap_if_not_expression(std::forward<Rhs>(rhs)));
        return relational_operation_conjunction(std::forward<Lhs>(lhs), std::move(op));
    }

    template <typename Relation, typename Lhs, typename Rhs>
    constexpr auto construct_relational_op(Relation&& relation, Lhs&& lhs, Rhs&& rhs)
    {
        if constexpr (have_same_relations_v<Lhs, Rhs>)
        {
            return merge_same_kinded_relational_ops(std::forward<Lhs>(lhs), std::forward<Rhs>(rhs));
        }
        else if constexpr (op_is_relation_v<Lhs, Relation> && is_value_expr_or_value_v<Rhs>)
        {
            return append_operand(std::forward<Lhs>(lhs), wrap_if_not_expression(std::forward<Rhs>(rhs)));
        }
        else if constexpr (is_value_expr_or_value_v<Lhs> && op_is_relation_v<Rhs, Relation>)
        {
            return prepend_operand(std::forward<Rhs>(lhs), wrap_if_not_expression(std::forward<Lhs>(rhs)));
        }
        else
        {
            return relational_operation_conjunction(wrap_if_not_expression(std::forward<Lhs>(lhs)), wrap_if_not_expression(std::forward<Rhs>(rhs)));
        }
    }

    //
    // operators
    //

    namespace detail
    {
        constexpr bool restrict_to_expressions = true;

        template <typename Lhs, typename Rhs>
        using allow_operator_t = std::enable_if_t<!restrict_to_expressions || is_expression_v<Lhs> || is_expression_v<Rhs>>;
    }

    template <typename Lhs, typename Rhs, typename = detail::allow_operator_t<Lhs, Rhs>>
    constexpr auto operator==(Lhs&& lhs, Rhs&& rhs)
    {
        return construct_relational_op(std::equal_to<>{}, std::forward<Lhs>(lhs), std::forward<Rhs>(rhs));
    }

    template <typename Lhs, typename Rhs, typename = detail::allow_operator_t<Lhs, Rhs>>
    constexpr auto operator!=(Lhs&& lhs, Rhs&& rhs)
    {
        return construct_relational_op(std::not_equal_to<>{}, std::forward<Lhs>(lhs), std::forward<Rhs>(rhs));
    }

    template <typename Lhs, typename Rhs, typename = detail::allow_operator_t<Lhs, Rhs>>
    constexpr auto operator<(Lhs&& lhs, Rhs&& rhs)
    {
        return construct_relational_op(std::less<>{}, std::forward<Lhs>(lhs), std::forward<Rhs>(rhs));
    }

    template <typename Lhs, typename Rhs, typename = detail::allow_operator_t<Lhs, Rhs>>
    constexpr auto operator<=(Lhs&& lhs, Rhs&& rhs)
    {
        return construct_relational_op(std::less_equal<>{}, std::forward<Lhs>(lhs), std::forward<Rhs>(rhs));
    }

    template <typename Lhs, typename Rhs, typename = detail::allow_operator_t<Lhs, Rhs>>
    constexpr auto operator>(Lhs&& lhs, Rhs&& rhs)
    {
        return construct_relational_op(std::greater<>{}, std::forward<Lhs>(lhs), std::forward<Rhs>(rhs));
    }

    template <typename Lhs, typename Rhs, typename = detail::allow_operator_t<Lhs, Rhs>>
    constexpr auto operator>=(Lhs&& lhs, Rhs&& rhs)
    {
        return construct_relational_op(std::greater_equal<>{}, std::forward<Lhs>(lhs), std::forward<Rhs>(rhs));
    }
}
