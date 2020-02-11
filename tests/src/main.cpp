#include <talk_to/expressions/value_expression.hpp>
#include <iostream>

using namespace tt::expressions;

int main()
{
    constexpr auto expr = value_expression(1) < 2 < 3 < 2;  // NO, BAD

    static_assert(expr);

    std::cout << typeid(expr).name() << "\n";

    return 0;
}
