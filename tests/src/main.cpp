#include <talk_to/relations.hpp>
#include <talk_to/pretty_output.hpp>

#include <iostream>
#include <string>
#include <string_view>


int main()
{
    std::cout << std::boolalpha;
    auto log = [](auto expr) { std::cout << expr << " -> " << static_cast<bool>(expr) << "\n"; };

    log(tt::rel(1) < 2 < 3 > 2 > 1);
    log(tt::rel("aa") < "ab" <= "ab" >= "aa");


    return 0;
}
