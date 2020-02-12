#pragma once

#include <functional>

namespace tt::expressions
{
    using op_and = std::logical_and<>;
    using op_less = std::less<>;
    using op_less_equal = std::less_equal<>;
    using op_greater = std::greater<>;
    using op_greater_equal = std::greater_equal<>;
    using op_equal_to = std::equal_to<>;
    using op_not_equal_to = std::not_equal_to<>;
}
