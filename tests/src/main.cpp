#include <talk_to/relations.hpp>
#include <talk_to/pretty_output.hpp>

#include <iostream>
#include <string>
#include <string_view>

std::string remove(std::string str, std::string_view substr)
{
    for(auto pos = str.find(substr); pos != std::string::npos; pos = str.find(substr))
        str.erase(pos, substr.size());
    return str;
}

int main()
{
    std::cout << std::boolalpha;
    auto log = [](auto expr) { std::cout << expr << " -> " << static_cast<bool>(expr) << "\n"; };

    log(tt::rel(1) < 2 < 3 > 2 > 1);
    log(tt::rel("aa") < "ab" <= "ab" >= "aa");


    return 0;
}
